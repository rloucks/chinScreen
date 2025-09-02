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
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_handle_t panel_handle;
    lv_color_t *trans_buf_1;
    lv_color_t *trans_buf_2;
    SemaphoreHandle_t trans_done_sem;
} lvgl_port_display_ctx_t;

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
typedef struct {
    esp_lcd_touch_handle_t handle;
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

static void lvgl_port_flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
static void lvgl_port_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
#endif

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
    if (!touch_cfg) return NULL;

    lvgl_port_touch_ctx_t *touch_ctx = malloc(sizeof(lvgl_port_touch_ctx_t));
    if (!touch_ctx) return NULL;
    touch_ctx->handle = touch_cfg->handle;

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvgl_port_touchpad_read;
    indev_drv.user_data = touch_ctx;

    return lv_indev_drv_register(&indev_drv);
}

esp_err_t lvgl_port_remove_touch(lv_indev_t *touch)
{
    if (!touch) return ESP_ERR_INVALID_ARG;

    lv_indev_drv_t *drv = touch->driver;
    lvgl_port_touch_ctx_t *touch_ctx = (lvgl_port_touch_ctx_t *)drv->user_data;

    lv_indev_delete(touch);
    free(touch_ctx);

    return ESP_OK;
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
