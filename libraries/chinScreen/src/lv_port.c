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

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
#include "esp_lcd_touch.h"
#endif

static const char *TAG = "LVGL";

/*----------------------------------------------------------
 * Display context
 *---------------------------------------------------------*/
typedef struct {
    esp_lcd_panel_io_handle_t io_handle;    /* LCD panel IO handle */
    esp_lcd_panel_handle_t    panel_handle; /* LCD panel handle */
    lv_display_t             *disp;         /* LVGL display - UPDATED for 9.x */

    uint32_t                  trans_size;       /* Maximum size for one transport */
    lv_color_t                *trans_buf_1;     /* Buffer send to driver */
    lv_color_t                *trans_buf_2;     /* Buffer send to driver */
    lv_color_t                *trans_act;       /* Active buffer for sending to driver */
    SemaphoreHandle_t         trans_done_sem;   /* Semaphore for signaling idle transfer */
    lv_display_rotation_t     sw_rotate;        /* Panel software rotation mask - UPDATED */

    lvgl_port_wait_cb         draw_wait_cb;     /* Callback function for drawing */
} lvgl_port_display_ctx_t;

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
typedef struct {
    esp_lcd_touch_handle_t  handle;        /* LCD touch IO handle */
    lv_indev_t             *indev;         /* LVGL input device - UPDATED for 9.x */
    lvgl_port_wait_cb       touch_wait_cb;  /* Callback function for touch */
} lvgl_port_touch_ctx_t;
#endif

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

static void lvgl_port_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    assert(disp != NULL);
    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)lv_display_get_user_data(disp);
    assert(disp_ctx != NULL);

    const int x_start = area->x1;
    const int x_end = area->x2;
    const int y_start = area->y1;
    const int y_end = area->y2;
    const int width = x_end - x_start + 1;
    const int height = y_end - y_start + 1;

    lv_color_t *from = (lv_color_t*)px_map;
    lv_color_t *to = NULL;

    if (disp_ctx->trans_size) {
        assert(disp_ctx->trans_buf_1 != NULL);

        int x_draw_start = 0;
        int x_draw_end = 0;
        int y_draw_start = 0;
        int y_draw_end = 0;
        int trans_count = 0;

        disp_ctx->trans_act = disp_ctx->trans_buf_1;
        int rotate = disp_ctx->sw_rotate;

        int x_start_tmp = 0;
        int x_end_tmp = 0;
        int max_width = 0;
        int trans_width = 0;

        int y_start_tmp = 0;
        int y_end_tmp = 0;
        int max_height = 0;
        int trans_height = 0;

        if (LV_DISPLAY_ROTATION_270 == rotate || LV_DISPLAY_ROTATION_90 == rotate) {
            max_width = ((disp_ctx->trans_size / height) > width) ? (width) : (disp_ctx->trans_size / height);
            trans_count = width / max_width + (width % max_width ? (1) : (0));

            x_start_tmp = x_start;
            x_end_tmp = x_end;
        } else {
            max_height = ((disp_ctx->trans_size / width) > height) ? (height) : (disp_ctx->trans_size / width);
            trans_count = height / max_height + (height % max_height ? (1) : (0));

            y_start_tmp = y_start;
            y_end_tmp = y_end;
        }

        for (int i = 0; i < trans_count; i++) {

            if (LV_DISPLAY_ROTATION_90 == rotate) {
                trans_width = (x_end - x_start_tmp + 1) > max_width ? max_width : (x_end - x_start_tmp + 1);
                x_end_tmp = (x_end - x_start_tmp + 1) > max_width ? (x_start_tmp + max_width - 1) : x_end;
            } else if (LV_DISPLAY_ROTATION_270 == rotate) {
                trans_width = (x_end_tmp - x_start + 1) > max_width ? max_width : (x_end_tmp - x_start + 1);
                x_start_tmp = (x_end_tmp - x_start + 1) > max_width ? (x_end_tmp - trans_width + 1) : x_start;
            } else if (LV_DISPLAY_ROTATION_0 == rotate) {
                trans_height = (y_end - y_start_tmp + 1) > max_height ? max_height : (y_end - y_start_tmp + 1);
                y_end_tmp = (y_end - y_start_tmp + 1) > max_height ? (y_start_tmp + max_height - 1) : y_end;
            } else {
                trans_height = (y_end_tmp - y_start + 1) > max_height ? max_height : (y_end_tmp - y_start + 1);
                y_start_tmp = (y_end_tmp - y_start + 1) > max_height ? (y_end_tmp - max_height + 1) : y_start;
            }

            disp_ctx->trans_act = (disp_ctx->trans_act == disp_ctx->trans_buf_1) ? (disp_ctx->trans_buf_2) : (disp_ctx->trans_buf_1);
            to = disp_ctx->trans_act;

            switch (rotate) {
            case LV_DISPLAY_ROTATION_90:
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < trans_width; x++) {
                        *(to + x * height + (height - y - 1)) = *(from + y * width + x_start_tmp + x);
                    }
                }
                x_draw_start = lv_display_get_vertical_resolution(disp) - y_end - 1;
                x_draw_end = lv_display_get_vertical_resolution(disp) - y_start - 1;
                y_draw_start = x_start_tmp;
                y_draw_end = x_end_tmp;
                break;
            case LV_DISPLAY_ROTATION_270:
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < trans_width; x++) {
                        *(to + (trans_width - x - 1) * height + y) = *(from + y * width + x_start_tmp + x);
                    }
                }
                x_draw_start = y_start;
                x_draw_end = y_end;
                y_draw_start = lv_display_get_horizontal_resolution(disp) - x_end_tmp - 1;
                y_draw_end = lv_display_get_horizontal_resolution(disp) - x_start_tmp - 1;
                break;
            case LV_DISPLAY_ROTATION_180:
                for (int y = 0; y < trans_height; y++) {
                    for (int x = 0; x < width; x++) {
                        *(to + (trans_height - y - 1)*width + (width - x - 1)) = *(from + y_start_tmp * width + y * (width) + x);
                    }
                }
                x_draw_start = lv_display_get_horizontal_resolution(disp) - x_end - 1;
                x_draw_end = lv_display_get_horizontal_resolution(disp) - x_start - 1;
                y_draw_start = lv_display_get_vertical_resolution(disp) - y_end_tmp - 1;
                y_draw_end = lv_display_get_vertical_resolution(disp) - y_start_tmp - 1;
                break;
            case LV_DISPLAY_ROTATION_0:
                for (int y = 0; y < trans_height; y++) {
                    for (int x = 0; x < width; x++) {
                        *(to + y * (width) + x) = *(from + y_start_tmp * width + y * (width) + x);
                    }
                }
                x_draw_start = x_start;
                x_draw_end = x_end;
                y_draw_start = y_start_tmp;
                y_draw_end = y_end_tmp;
                break;
            default:
                break;
            }

            if (0 == i) {
                if (disp_ctx->draw_wait_cb) {
                    disp_ctx->draw_wait_cb(disp_ctx->panel_handle->user_data);
                }
                xSemaphoreGive(disp_ctx->trans_done_sem);
            }

            xSemaphoreTake(disp_ctx->trans_done_sem, portMAX_DELAY);
            esp_lcd_panel_draw_bitmap(disp_ctx->panel_handle, x_draw_start, y_draw_start, x_draw_end + 1, y_draw_end + 1, to);

            if (LV_DISPLAY_ROTATION_90 == rotate) {
                x_start_tmp += max_width;
            } else if (LV_DISPLAY_ROTATION_270 == rotate) {
                x_end_tmp -= max_width;
            } if (LV_DISPLAY_ROTATION_0 == rotate) {
                y_start_tmp += max_height;
            } else {
                y_end_tmp -= max_height;
            }
        }
    } else {
        esp_lcd_panel_draw_bitmap(disp_ctx->panel_handle, x_start, y_start, x_end + 1, y_end + 1, (uint16_t*)px_map);
    }
    lv_display_flush_ready(disp);  // UPDATED function name
}

/*----------------------------------------------------------
 * Public API
 *---------------------------------------------------------*/
esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg)
{
    if (!cfg) return ESP_ERR_INVALID_ARG;

    lv_init();

    if (cfg->timer_period_ms > 0) lvgl_tick_period_ms = cfg->timer_period_ms;
    lvgl_tick_init();

    if (cfg->task_max_sleep_ms > 0) lvgl_task_sleep_ms = cfg->task_max_sleep_ms;

    lvgl_mutex = xSemaphoreCreateRecursiveMutex();
    if (!lvgl_mutex) return ESP_ERR_NO_MEM;

    BaseType_t res;
    if (cfg->task_affinity < 0) {
        res = xTaskCreate(lvgl_task, "LVGL Task", cfg->task_stack, NULL, cfg->task_priority, NULL);
    } else {
        res = xTaskCreatePinnedToCore(lvgl_task, "LVGL Task", cfg->task_stack, NULL, cfg->task_priority, NULL, cfg->task_affinity);
    }
    if (res != pdPASS) return ESP_FAIL;

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

lv_disp_t *lvgl_port_add_disp(const lvgl_port_display_cfg_t *disp_cfg)
{
    if (!disp_cfg) return NULL;

    lvgl_port_display_ctx_t *disp_ctx = malloc(sizeof(lvgl_port_display_ctx_t));
    if (!disp_ctx) return NULL;

    lv_color_t *buf1 = malloc(disp_cfg->buffer_size * sizeof(lv_color_t));
    if (!buf1) { free(disp_ctx); return NULL; }

    lv_color_t *buf2 = NULL;
    if (disp_cfg->trans_size) {
        buf2 = malloc(disp_cfg->trans_size * sizeof(lv_color_t));
        if (!buf2) { free(buf1); free(disp_ctx); return NULL; }
        disp_ctx->trans_buf_1 = buf2;
    }

    lv_disp_draw_buf_t *disp_buf = malloc(sizeof(lv_disp_draw_buf_t));
    if (!disp_buf) { if(buf2) free(buf2); free(buf1); free(disp_ctx); return NULL; }

    lv_disp_draw_buf_init(disp_buf, buf1, NULL, disp_cfg->buffer_size);

    lv_disp_drv_t drv;
    lv_disp_drv_init(&drv);
    drv.hor_res = disp_cfg->hres;
    drv.ver_res = disp_cfg->vres;
    drv.flush_cb = lvgl_port_flush_callback;
    drv.draw_buf = disp_buf;
    drv.user_data = disp_ctx;
    drv.full_refresh = true;

    return lv_disp_drv_register(&drv);
}

esp_err_t lvgl_port_remove_disp(lv_disp_t *disp)
{
    if (!disp) return ESP_ERR_INVALID_ARG;

    lv_disp_drv_t *drv = disp->driver;
    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)drv->user_data;

    lv_disp_remove(disp);

    if (drv->draw_buf) {
        if (drv->draw_buf->buf1) free(drv->draw_buf->buf1);
        if (drv->draw_buf->buf2) free(drv->draw_buf->buf2);
        free(drv->draw_buf);
    }

    free(disp_ctx);

    return ESP_OK;
}

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
lv_indev_t *lvgl_port_add_touch(const lvgl_port_touch_cfg_t *touch_cfg)
{
    assert(touch_cfg != NULL);
    assert(touch_cfg->disp != NULL);
    assert(touch_cfg->handle != NULL);

    /* Touch context */
    lvgl_port_touch_ctx_t *touch_ctx = malloc(sizeof(lvgl_port_touch_ctx_t));
    if (touch_ctx == NULL) {
        ESP_LOGE(TAG, "Not enough memory for touch context allocation!");
        return NULL;
    }
    touch_ctx->handle = touch_cfg->handle;
    touch_ctx->touch_wait_cb = touch_cfg->touch_wait_cb;

    /* Register a touchpad input device - UPDATED for LVGL 9.x */
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, touch_cfg->disp);
    lv_indev_set_read_cb(indev, lvgl_port_touchpad_read);
    lv_indev_set_user_data(indev, touch_ctx);
    
    touch_ctx->indev = indev;
    return indev;
}
#endif

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

void lvgl_port_flush_ready(lv_disp_t *disp)
{
    lv_disp_flush_ready(disp);
}

/*----------------------------------------------------------
 * Private
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

static void lvgl_port_flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    /* User implementation: push color_map to your display */
    lv_disp_flush_ready(drv);
}

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
static void lvgl_port_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    /* User implementation: read touch data here */
    data->state = LV_INDEV_STATE_REL;
    data->point.x = 0;
    data->point.y = 0;
}
#endif

static void lvgl_tick_cb(void *arg)
{
    lv_tick_inc(lvgl_tick_period_ms);
}

static esp_err_t lvgl_tick_init(void)
{
    esp_timer_create_args_t args = {
        .callback = &lvgl_tick_cb,
        .name = "lvgl_tick"
    };
    esp_timer_create(&args, &lvgl_tick_timer);
    return esp_timer_start_periodic(lvgl_tick_timer, lvgl_tick_period_ms * 1000);
}
