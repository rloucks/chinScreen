/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "lv_port.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_interface.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_lcd_panel_vendor.h"

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
#include "esp_lcd_touch.h"
#endif

static const char *TAG = "LVGL";

/*----------------------------------------------------------
 * Display context - UPDATED for LVGL 9.x
 *---------------------------------------------------------*/
typedef struct {
    esp_lcd_panel_io_handle_t io_handle;    /* LCD panel IO handle */
    esp_lcd_panel_handle_t    panel_handle; /* LCD panel handle */
    lv_display_t             *disp;         /* LVGL display */

    /* main LVGL draw buffers (stored so we can free them on remove) */
    void                     *buf1_ptr;     /* allocated main draw buffer 1 */
    void                     *buf2_ptr;     /* allocated main draw buffer 2 (optional) */

    uint32_t                  trans_size;       /* Maximum size for one transport (pixels) */
    lv_color_t               *trans_buf_1;     /* transport buffer 1 */
    lv_color_t               *trans_buf_2;     /* transport buffer 2 */
    lv_color_t               *trans_act;       /* Active transport buffer */
    SemaphoreHandle_t         trans_done_sem;   /* Semaphore for signaling idle transfer */
    lv_display_rotation_t     sw_rotate;        /* Panel software rotation mask */

    lvgl_port_wait_cb         draw_wait_cb;     /* Callback function for drawing */
} lvgl_port_display_ctx_t;

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
typedef struct {
    esp_lcd_touch_handle_t  handle;        /* LCD touch IO handle */
    lv_indev_t             *indev;         /* LVGL input device */
    lvgl_port_wait_cb       touch_wait_cb;  /* Callback function for touch */
} lvgl_port_touch_ctx_t;
#endif

/*----------------------------------------------------------
 * globals
 *---------------------------------------------------------*/
static SemaphoreHandle_t lvgl_mutex = NULL;
static esp_timer_handle_t lvgl_tick_timer = NULL;
static int lvgl_tick_period_ms = 5;
static bool lvgl_task_running = false;
static int lvgl_task_sleep_ms = 5;

/*----------------------------------------------------------
 * Forward declarations
 *---------------------------------------------------------*/
static void lvgl_task(void *arg);
static void lvgl_tick_cb(void *arg);
static esp_err_t lvgl_tick_init(void);

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
static void lvgl_port_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data);
#endif

/*----------------------------------------------------------
 * Flush callback for LVGL 9.x
 * - Signature: (lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
 *   LVGL passes a pointer to pixel data (uint8_t*). We cast to lv_color_t*.
 *---------------------------------------------------------*/
static void lvgl_port_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    assert(disp != NULL);
    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)lv_display_get_user_data(disp);
    assert(disp_ctx != NULL);

    const int x_start = area->x1;
    const int x_end   = area->x2;
    const int y_start = area->y1;
    const int y_end   = area->y2;
    const int width   = x_end - x_start + 1;
    const int height  = y_end - y_start + 1;

    lv_color_t *from = (lv_color_t *)px_map;
    lv_color_t *to = NULL;

    if (disp_ctx->trans_size && disp_ctx->trans_buf_1 && disp_ctx->trans_buf_2) {
        /* We will split the update into transport-sized chunks, possibly rotating into trans_bufs */
        int x_draw_start = 0, x_draw_end = 0, y_draw_start = 0, y_draw_end = 0;
        int trans_count = 0;

        disp_ctx->trans_act = disp_ctx->trans_buf_1;
        lv_display_rotation_t rotate = disp_ctx->sw_rotate;

        int x_start_tmp = 0, x_end_tmp = 0;
        int max_width = 0, trans_width = 0;
        int y_start_tmp = 0, y_end_tmp = 0;
        int max_height = 0, trans_height = 0;

        if (rotate == LV_DISPLAY_ROTATION_270 || rotate == LV_DISPLAY_ROTATION_90) {
            max_width = (int)( (disp_ctx->trans_size / height) > width ? width : (disp_ctx->trans_size / height) );
            trans_count = width / max_width + (width % max_width ? 1 : 0);
            x_start_tmp = x_start;
            x_end_tmp = x_end;
        } else {
            max_height = (int)( (disp_ctx->trans_size / width) > height ? height : (disp_ctx->trans_size / width) );
            trans_count = height / max_height + (height % max_height ? 1 : 0);
            y_start_tmp = y_start;
            y_end_tmp = y_end;
        }

        for (int i = 0; i < trans_count; i++) {
            if (rotate == LV_DISPLAY_ROTATION_90) {
                trans_width = ((x_end - x_start_tmp + 1) > max_width) ? max_width : (x_end - x_start_tmp + 1);
                x_end_tmp = ((x_end - x_start_tmp + 1) > max_width) ? (x_start_tmp + max_width - 1) : x_end;
            } else if (rotate == LV_DISPLAY_ROTATION_270) {
                trans_width = ((x_end_tmp - x_start + 1) > max_width) ? max_width : (x_end_tmp - x_start + 1);
                x_start_tmp = ((x_end_tmp - x_start + 1) > max_width) ? (x_end_tmp - trans_width + 1) : x_start;
            } else if (rotate == LV_DISPLAY_ROTATION_0) {
                trans_height = ((y_end - y_start_tmp + 1) > max_height) ? max_height : (y_end - y_start_tmp + 1);
                y_end_tmp = ((y_end - y_start_tmp + 1) > max_height) ? (y_start_tmp + max_height - 1) : y_end;
            } else /* 180 */ {
                trans_height = ((y_end_tmp - y_start + 1) > max_height) ? max_height : (y_end_tmp - y_start + 1);
                y_start_tmp = ((y_end_tmp - y_start + 1) > max_height) ? (y_end_tmp - max_height + 1) : y_start;
            }

            /* Toggle active transport buffer */
            disp_ctx->trans_act = (disp_ctx->trans_act == disp_ctx->trans_buf_1) ? disp_ctx->trans_buf_2 : disp_ctx->trans_buf_1;
            to = disp_ctx->trans_act;

            /* Prepare rotated/straight block into `to` */
            switch (rotate) {
                case LV_DISPLAY_ROTATION_90:
                    /* width->trans_width, height->height */
                    for (int yy = 0; yy < height; yy++) {
                        for (int xx = 0; xx < trans_width; xx++) {
                            /* dest index: x * height + (height - y - 1) */
                            to[ xx * height + (height - yy - 1) ] = from[ yy * width + x_start_tmp + xx ];
                        }
                    }
                    x_draw_start = lv_display_get_vertical_resolution(disp) - y_end - 1;
                    x_draw_end   = lv_display_get_vertical_resolution(disp) - y_start - 1;
                    y_draw_start = x_start_tmp;
                    y_draw_end   = x_end_tmp;
                    break;

                case LV_DISPLAY_ROTATION_270:
                    for (int yy = 0; yy < height; yy++) {
                        for (int xx = 0; xx < trans_width; xx++) {
                            to[ (trans_width - xx - 1) * height + yy ] = from[ yy * width + x_start_tmp + xx ];
                        }
                    }
                    x_draw_start = y_start;
                    x_draw_end   = y_end;
                    y_draw_start = lv_display_get_horizontal_resolution(disp) - x_end_tmp - 1;
                    y_draw_end   = lv_display_get_horizontal_resolution(disp) - x_start_tmp - 1;
                    break;

                case LV_DISPLAY_ROTATION_180:
                    for (int yy = 0; yy < trans_height; yy++) {
                        for (int xx = 0; xx < width; xx++) {
                            to[ (trans_height - yy - 1) * width + (width - xx - 1) ] =
                                from[ (y_start_tmp * width) + yy * width + xx ];
                        }
                    }
                    x_draw_start = lv_display_get_horizontal_resolution(disp) - x_end - 1;
                    x_draw_end   = lv_display_get_horizontal_resolution(disp) - x_start - 1;
                    y_draw_start = lv_display_get_vertical_resolution(disp) - y_end_tmp - 1;
                    y_draw_end   = lv_display_get_vertical_resolution(disp) - y_start_tmp - 1;
                    break;

                case LV_DISPLAY_ROTATION_0:
                default:
                    for (int yy = 0; yy < trans_height; yy++) {
                        for (int xx = 0; xx < width; xx++) {
                            to[ yy * width + xx ] = from[ (y_start_tmp * width) + yy * width + xx ];
                        }
                    }
                    x_draw_start = x_start;
                    x_draw_end   = x_end;
                    y_draw_start = y_start_tmp;
                    y_draw_end   = y_end_tmp;
                    break;
            }

            /* For first chunk, call draw_wait_cb and release transport semaphore if used */
            if (i == 0) {
                if (disp_ctx->draw_wait_cb) {
                    /* draw_wait_cb expects a void* handle; pass panel_handle pointer */
                    disp_ctx->draw_wait_cb((void *)disp_ctx->panel_handle);
                }
                if (disp_ctx->trans_done_sem) {
                    xSemaphoreGive(disp_ctx->trans_done_sem);
                }
            }

            /* If a trans_done_sem exists, wait for it before issuing the transfer */
            if (disp_ctx->trans_done_sem) {
                xSemaphoreTake(disp_ctx->trans_done_sem, portMAX_DELAY);
            }

            /* Draw the prepared block to panel (uses ESP-IDF panel API) */
            esp_lcd_panel_draw_bitmap(disp_ctx->panel_handle,
                                      x_draw_start, y_draw_start,
                                      x_draw_end + 1, y_draw_end + 1,
                                      (uint16_t *)to);

            /* advance tmp window */
            if (rotate == LV_DISPLAY_ROTATION_90) {
                x_start_tmp += max_width;
            } else if (rotate == LV_DISPLAY_ROTATION_270) {
                x_end_tmp -= max_width;
            } else if (rotate == LV_DISPLAY_ROTATION_0) {
                y_start_tmp += max_height;
            } else {
                y_end_tmp -= max_height;
            }
        } /* for trans_count */
    } else {
        /* No transport buffers — direct draw */
        esp_lcd_panel_draw_bitmap(disp_ctx->panel_handle,
                                  x_start, y_start,
                                  x_end + 1, y_end + 1,
                                  (uint16_t *)px_map);
    }

    /* Inform LVGL the flush is done */
    lv_display_flush_ready(disp);
}

/*----------------------------------------------------------
 * Public API - UPDATED for LVGL 9.x
 *---------------------------------------------------------*/
esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg)
{
    if (!cfg) return ESP_ERR_INVALID_ARG;

    /* Initialize LVGL core */
    lv_init();

    if (cfg->timer_period_ms > 0) lvgl_tick_period_ms = cfg->timer_period_ms;

    if (lvgl_tick_init() != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init lvgl tick timer");
        return ESP_ERR_INVALID_STATE;
    }

    if (cfg->task_max_sleep_ms > 0) lvgl_task_sleep_ms = cfg->task_max_sleep_ms;

    lvgl_mutex = xSemaphoreCreateRecursiveMutex();
    if (!lvgl_mutex) {
        ESP_LOGE(TAG, "Failed to create LVGL mutex");
        return ESP_ERR_NO_MEM;
    }

    BaseType_t res;
    if (cfg->task_affinity < 0) {
        res = xTaskCreate(lvgl_task, "LVGL Task", cfg->task_stack, NULL, cfg->task_priority, NULL);
    } else {
        res = xTaskCreatePinnedToCore(lvgl_task, "LVGL Task", cfg->task_stack, NULL, cfg->task_priority, NULL, cfg->task_affinity);
    }
    if (res != pdPASS) {
        vSemaphoreDelete(lvgl_mutex);
        lvgl_mutex = NULL;
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t lvgl_port_deinit(void)
{
    if (lvgl_tick_timer) {
        esp_timer_stop(lvgl_tick_timer);
        esp_timer_delete(lvgl_tick_timer);
        lvgl_tick_timer = NULL;
    }

    lvgl_task_running = false;

    if (lvgl_mutex) {
        vSemaphoreDelete(lvgl_mutex);
        lvgl_mutex = NULL;
    }

    return ESP_OK;
}

/*----------------------------------------------------------
 * Display add function for LVGL 9.x
 *---------------------------------------------------------*/
lv_display_t *lvgl_port_add_disp(const lvgl_port_display_cfg_t *disp_cfg)
{
    if (!disp_cfg) return NULL;

    lvgl_port_display_ctx_t *disp_ctx = malloc(sizeof(lvgl_port_display_ctx_t));
    if (!disp_ctx) return NULL;
    memset(disp_ctx, 0, sizeof(lvgl_port_display_ctx_t));

    disp_ctx->io_handle = disp_cfg->io_handle;
    disp_ctx->panel_handle = disp_cfg->panel_handle;
    disp_ctx->trans_size = disp_cfg->trans_size;
    disp_ctx->sw_rotate = (lv_display_rotation_t)disp_cfg->sw_rotate;
    disp_ctx->draw_wait_cb = disp_cfg->draw_wait_cb;

    /* Allocate primary draw buffer (in pixels) */
    uint32_t buff_caps = MALLOC_CAP_DEFAULT;
    if (disp_cfg->flags.buff_dma) {
        buff_caps = MALLOC_CAP_DMA;
    } else if (disp_cfg->flags.buff_spiram) {
        buff_caps = MALLOC_CAP_SPIRAM;
    }

    /* Allocate buffer sized in pixels (LVGL expects number of pixels) */
    void *buf1 = heap_caps_malloc((size_t)disp_cfg->buffer_size * sizeof(lv_color_t), buff_caps);
    if (!buf1) {
        free(disp_ctx);
        return NULL;
    }
    disp_ctx->buf1_ptr = buf1;
    disp_ctx->buf2_ptr = NULL;

    /* If user provided a second buffer size (optional) they could be set here - we keep single buffer */
    if (disp_cfg->trans_size > 0) {
        /* allocate transport buffers (DMA-capable) */
        disp_ctx->trans_buf_1 = heap_caps_malloc((size_t)disp_cfg->trans_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
        disp_ctx->trans_buf_2 = heap_caps_malloc((size_t)disp_cfg->trans_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
        disp_ctx->trans_done_sem = xSemaphoreCreateCounting(1, 0);

        if (!disp_ctx->trans_buf_1 || !disp_ctx->trans_buf_2 || !disp_ctx->trans_done_sem) {
            if (disp_ctx->trans_buf_1) free(disp_ctx->trans_buf_1);
            if (disp_ctx->trans_buf_2) free(disp_ctx->trans_buf_2);
            if (disp_ctx->trans_done_sem) vSemaphoreDelete(disp_ctx->trans_done_sem);
            free(disp_ctx->buf1_ptr);
            free(disp_ctx);
            return NULL;
        }
    }

    /* Create LVGL display with 9.x API */
    lv_display_t *disp = lv_display_create(disp_cfg->hres, disp_cfg->vres);
    if (!disp) {
        if (disp_ctx->trans_buf_1) free(disp_ctx->trans_buf_1);
        if (disp_ctx->trans_buf_2) free(disp_ctx->trans_buf_2);
        if (disp_ctx->trans_done_sem) vSemaphoreDelete(disp_ctx->trans_done_sem);
        free(disp_ctx->buf1_ptr);
        free(disp_ctx);
        return NULL;
    }

    /* set rotation and buffers (buffer_size is number of pixels) */
    lv_display_set_rotation(disp, disp_ctx->sw_rotate);
    /* Use single main buffer (buf1) — mode partial is typical for buffered drivers */
    lv_display_set_buffers(disp, disp_ctx->buf1_ptr, NULL, (uint32_t)disp_cfg->buffer_size, LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, lvgl_port_flush_callback);
    lv_display_set_user_data(disp, disp_ctx);

    disp_ctx->disp = disp;

    return disp;
}

/*----------------------------------------------------------
 * Display remove function for LVGL 9.x
 *---------------------------------------------------------*/
esp_err_t lvgl_port_remove_disp(lv_display_t *disp)
{
    if (!disp) return ESP_ERR_INVALID_ARG;

    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)lv_display_get_user_data(disp);

    /* Free the main draw buffers allocated in add_disp */
    if (disp_ctx) {
        if (disp_ctx->buf1_ptr) free(disp_ctx->buf1_ptr);
        if (disp_ctx->buf2_ptr) free(disp_ctx->buf2_ptr);

        /* free transport buffers */
        if (disp_ctx->trans_buf_1) free(disp_ctx->trans_buf_1);
        if (disp_ctx->trans_buf_2) free(disp_ctx->trans_buf_2);
        if (disp_ctx->trans_done_sem) vSemaphoreDelete(disp_ctx->trans_done_sem);

        free(disp_ctx);
    }

    /* delete LVGL display */
    lv_display_delete(disp);

    return ESP_OK;
}

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
/*----------------------------------------------------------
 * Touch handling (LVGL 9.x)
 *---------------------------------------------------------*/
lv_indev_t *lvgl_port_add_touch(const lvgl_port_touch_cfg_t *touch_cfg)
{
    if (!touch_cfg || !touch_cfg->disp || !touch_cfg->handle) return NULL;

    lvgl_port_touch_ctx_t *touch_ctx = malloc(sizeof(lvgl_port_touch_ctx_t));
    if (!touch_ctx) {
        ESP_LOGE(TAG, "Not enough memory for touch context allocation!");
        return NULL;
    }
    memset(touch_ctx, 0, sizeof(lvgl_port_touch_ctx_t));
    touch_ctx->handle = touch_cfg->handle;
    touch_ctx->touch_wait_cb = touch_cfg->touch_wait_cb;

    /* create LVGL input device and attach callbacks */
    lv_indev_t *indev = lv_indev_create();
    if (!indev) {
        free(touch_ctx);
        return NULL;
    }

    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, touch_cfg->disp);
    lv_indev_set_read_cb(indev, lvgl_port_touchpad_read);
    lv_indev_set_user_data(indev, touch_ctx);

    touch_ctx->indev = indev;
    return indev;
}

esp_err_t lvgl_port_remove_touch(lv_indev_t *touch)
{
    if (!touch) return ESP_ERR_INVALID_ARG;

    lvgl_port_touch_ctx_t *touch_ctx = (lvgl_port_touch_ctx_t *)lv_indev_get_user_data(touch);

    lv_indev_delete(touch);

    if (touch_ctx) free(touch_ctx);

    return ESP_OK;
}

/* Touch read callback */
static void lvgl_port_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    lvgl_port_touch_ctx_t *touch_ctx = (lvgl_port_touch_ctx_t *)lv_indev_get_user_data(indev);
    if (!touch_ctx || !touch_ctx->handle) {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    uint16_t touch_x[1] = {0};
    uint16_t touch_y[1] = {0};
    uint8_t touch_cnt = 0;

    bool touch_int = false;
    if (touch_ctx->touch_wait_cb) {
        touch_int = touch_ctx->touch_wait_cb(touch_ctx->handle->config.user_data);
    }

    if (touch_int) {
        esp_lcd_touch_read_data(touch_ctx->handle);
        bool pressed = esp_lcd_touch_get_coordinates(touch_ctx->handle, touch_x, touch_y, NULL, &touch_cnt, 1);
        if (pressed && touch_cnt > 0) {
            data->point.x = touch_x[0];
            data->point.y = touch_y[0];
            data->state = LV_INDEV_STATE_PRESSED;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
#endif /* ESP_LVGL_PORT_TOUCH_COMPONENT */

/*----------------------------------------------------------
 * Mutex helpers
 *---------------------------------------------------------*/
bool lvgl_port_lock(uint32_t timeout_ms)
{
    if (!lvgl_mutex) return false;
    TickType_t ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mutex, ticks) == pdTRUE;
}

void lvgl_port_unlock(void)
{
    if (lvgl_mutex) xSemaphoreGiveRecursive(lvgl_mutex);
}

/*----------------------------------------------------------
 * Flush ready helper
 *---------------------------------------------------------*/
void lvgl_port_flush_ready(lv_display_t *disp)
{
    if (disp) {
        lv_display_flush_ready(disp);
    }
}

/*----------------------------------------------------------
 * Private functions
 *---------------------------------------------------------*/
static void lvgl_task(void *arg)
{
    lvgl_task_running = true;
    while (lvgl_task_running) {
        if (lvgl_port_lock(0)) {
            lv_timer_handler();
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(lvgl_task_sleep_ms));
    }
    vTaskDelete(NULL);
}

static void lvgl_tick_cb(void *arg)
{
    (void)arg;
    lv_tick_inc(lvgl_tick_period_ms);
}

static esp_err_t lvgl_tick_init(void)
{
    esp_timer_create_args_t args = {
        .callback = &lvgl_tick_cb,
        .name = "lvgl_tick"
    };
    esp_err_t err = esp_timer_create(&args, &lvgl_tick_timer);
    if (err != ESP_OK) return err;
    return esp_timer_start_periodic(lvgl_tick_timer, (uint64_t)lvgl_tick_period_ms * 1000ULL);
}
