/*
 * CORRECTED lv_port.c for LVGL 9.x
 * This properly bridges your working BSP hardware setup to LVGL 9.x APIs
 * Based on analyzing your working LVGL 8.6 version
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
 * Display context - CORRECTED for LVGL 9.x
 *---------------------------------------------------------*/
typedef struct {
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_handle_t    panel_handle;
    lv_display_t             *disp;         /* LVGL 9.x display */

    void                     *buf1_ptr;
    void                     *buf2_ptr;

    uint32_t                  trans_size;
    lv_color_t               *trans_buf_1;
    lv_color_t               *trans_buf_2;
    lv_color_t               *trans_act;
    SemaphoreHandle_t         trans_done_sem;
    lv_display_rotation_t     sw_rotate;        /* LVGL 9.x rotation */

    lvgl_port_wait_cb         draw_wait_cb;
} lvgl_port_display_ctx_t;

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
typedef struct {
    esp_lcd_touch_handle_t  handle;
    lv_indev_t             *indev;
    lvgl_port_wait_cb       touch_wait_cb;
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
 * CORRECTED Flush callback for LVGL 9.x
 * This matches your working LVGL 8.6 logic but uses LVGL 9.x APIs
 *---------------------------------------------------------*/
static void lvgl_port_flush_callback(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    ESP_LOGI(TAG, "FLUSH CALLBACK: (%d,%d) to (%d,%d)", area->x1, area->y1, area->x2, area->y2);
    
    assert(disp != NULL);
    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)lv_display_get_user_data(disp);
    assert(disp_ctx != NULL);

    const int x_start = area->x1;
    const int x_end   = area->x2;
    const int y_start = area->y1;
    const int y_end   = area->y2;
    const int width   = x_end - x_start + 1;
    const int height  = y_end - y_start + 1;

    ESP_LOGI(TAG, "Drawing %dx%d area", width, height);

    // For simplicity, let's start with direct drawing (no transport buffers)
    // This matches the working approach from your LVGL 8.6 version
    
    // Call the draw wait callback if it exists (from your working version)
    if (disp_ctx->draw_wait_cb) {
        ESP_LOGI(TAG, "Calling draw_wait_cb");
        disp_ctx->draw_wait_cb((void *)disp_ctx->panel_handle);
    }

    // Direct draw to panel hardware (this should work based on your working BSP)
    ESP_LOGI(TAG, "Calling esp_lcd_panel_draw_bitmap");
	esp_lcd_panel_draw_bitmap(disp_ctx->panel_handle,
                         x_start, y_start,
                         x_end + 1, y_end + 1,
                         (void *)px_map);  // Remove uint16_t cast
    


    /* Inform LVGL the flush is done */
    lv_display_flush_ready(disp);
    ESP_LOGI(TAG, "Flush complete");
}

/*----------------------------------------------------------
 * Public API - CORRECTED for LVGL 9.x
 *---------------------------------------------------------*/
esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg)
{
    ESP_LOGI(TAG, "LVGL port init");
    
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

    ESP_LOGI(TAG, "LVGL port init SUCCESS");
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
 * CORRECTED Display add function for LVGL 9.x
 * This follows your working LVGL 8.6 approach
 *---------------------------------------------------------*/
lv_display_t *lvgl_port_add_disp(const lvgl_port_display_cfg_t *disp_cfg)
{
    ESP_LOGI(TAG, "Adding display to LVGL 9.x");
    
    if (!disp_cfg) {
        ESP_LOGE(TAG, "Display config is NULL");
        return NULL;
    }

    ESP_LOGI(TAG, "Buffer size: %d, HRes: %d, VRes: %d", 
             disp_cfg->buffer_size, disp_cfg->hres, disp_cfg->vres);

    lvgl_port_display_ctx_t *disp_ctx = malloc(sizeof(lvgl_port_display_ctx_t));
    if (!disp_ctx) {
        ESP_LOGE(TAG, "Failed to allocate display context");
        return NULL;
    }
    memset(disp_ctx, 0, sizeof(lvgl_port_display_ctx_t));

    disp_ctx->io_handle = disp_cfg->io_handle;
    disp_ctx->panel_handle = disp_cfg->panel_handle;
    disp_ctx->trans_size = disp_cfg->trans_size;
    disp_ctx->sw_rotate = (lv_display_rotation_t)disp_cfg->sw_rotate;
    disp_ctx->draw_wait_cb = disp_cfg->draw_wait_cb;

    /* Allocate primary draw buffer using the same approach as your working version */
    uint32_t buff_caps = MALLOC_CAP_DEFAULT;
    if (disp_cfg->flags.buff_dma) {
        buff_caps = MALLOC_CAP_DMA;
        ESP_LOGI(TAG, "Using DMA capable buffer");
    } else if (disp_cfg->flags.buff_spiram) {
        buff_caps = MALLOC_CAP_SPIRAM;
        ESP_LOGI(TAG, "Using SPIRAM buffer");
    }

    // Allocate buffer (this should match your working version)
    void *buf1 = heap_caps_malloc((size_t)disp_cfg->buffer_size * sizeof(lv_color_t), buff_caps);
    if (!buf1) {
        ESP_LOGE(TAG, "Failed to allocate display buffer");
        free(disp_ctx);
        return NULL;
    }
    disp_ctx->buf1_ptr = buf1;
    ESP_LOGI(TAG, "Allocated %d byte buffer", (int)(disp_cfg->buffer_size * sizeof(lv_color_t)));

    /* Transport buffers - simplified for now, can add back later if needed */
    disp_ctx->trans_buf_1 = NULL;
    disp_ctx->trans_buf_2 = NULL;
    disp_ctx->trans_done_sem = NULL;

    /* Create LVGL 9.x display */
    lv_display_t *disp = lv_display_create(disp_cfg->hres, disp_cfg->vres);
    if (!disp) {
        ESP_LOGE(TAG, "Failed to create LVGL 9.x display");
        free(disp_ctx->buf1_ptr);
        free(disp_ctx);
        return NULL;
    }
    ESP_LOGI(TAG, "LVGL 9.x display created");

    /* Configure display */
    lv_display_set_rotation(disp, disp_ctx->sw_rotate);
    
    /* Set buffers - CORRECTED for LVGL 9.x */
	lv_display_set_buffers(disp, disp_ctx->buf1_ptr, NULL, 
                      (uint32_t)(disp_cfg->buffer_size * sizeof(lv_color_t)), 
                      LV_DISPLAY_RENDER_MODE_PARTIAL);
    ESP_LOGI(TAG, "Display buffers configured");
    
    /* Set flush callback */
    lv_display_set_flush_cb(disp, lvgl_port_flush_callback);
    ESP_LOGI(TAG, "Flush callback set");
    
    /* Set user data */
    lv_display_set_user_data(disp, disp_ctx);

    disp_ctx->disp = disp;

    ESP_LOGI(TAG, "Display successfully added to LVGL 9.x");
    return disp;
}

/*----------------------------------------------------------
 * Display remove function for LVGL 9.x
 *---------------------------------------------------------*/
esp_err_t lvgl_port_remove_disp(lv_display_t *disp)
{
    if (!disp) return ESP_ERR_INVALID_ARG;

    lvgl_port_display_ctx_t *disp_ctx = (lvgl_port_display_ctx_t *)lv_display_get_user_data(disp);

    if (disp_ctx) {
        if (disp_ctx->buf1_ptr) free(disp_ctx->buf1_ptr);
        if (disp_ctx->buf2_ptr) free(disp_ctx->buf2_ptr);
        if (disp_ctx->trans_buf_1) free(disp_ctx->trans_buf_1);
        if (disp_ctx->trans_buf_2) free(disp_ctx->trans_buf_2);
        if (disp_ctx->trans_done_sem) vSemaphoreDelete(disp_ctx->trans_done_sem);
        free(disp_ctx);
    }

    lv_display_delete(disp);
    return ESP_OK;
}

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
    ESP_LOGI(TAG, "LVGL task started");
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