/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_lcd_types.h"
#include "lvgl.h"

#if __has_include("esp_lcd_touch.h")
#include "esp_lcd_touch.h"
#define ESP_LVGL_PORT_TOUCH_COMPONENT 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*lvgl_port_wait_cb)(void *handle);

/**
 * @brief LVGL port initialization configuration
 */
typedef struct {
    int task_priority;      /*!< LVGL task priority */
    int task_stack;         /*!< LVGL task stack size in bytes */
    int task_affinity;      /*!< LVGL task pinned to core (-1 = no affinity) */
    int task_max_sleep_ms;  /*!< Maximum sleep in LVGL task loop */
    int timer_period_ms;    /*!< LVGL tick timer period in milliseconds */
} lvgl_port_cfg_t;

/**
 * @brief LVGL display configuration structure
 */
typedef struct {
    esp_lcd_panel_io_handle_t io_handle;    /*!< LCD panel IO handle */
    esp_lcd_panel_handle_t panel_handle;    /*!< LCD panel handle */
    lvgl_port_wait_cb draw_wait_cb;         /*!< Optional callback to wait for drawing completion */

    uint32_t buffer_size;    /*!< LVGL draw buffer size (pixels) */
    uint32_t trans_size;     /*!< Size for transfer buffer (pixels) */
    uint32_t hres;           /*!< Horizontal resolution of the display */
    uint32_t vres;           /*!< Vertical resolution of the display */
    lv_disp_rot_t sw_rotate; /*!< Panel software rotation */

    struct {
        unsigned int buff_dma: 1;    /*!< Allocate LVGL buffer as DMA-capable */
        unsigned int buff_spiram: 1; /*!< Allocate LVGL buffer in PSRAM */
    } flags;
} lvgl_port_display_cfg_t;

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
/**
 * @brief LVGL touch configuration structure
 */
typedef struct {
    lv_disp_t *disp;                   /*!< LVGL display handle returned from lvgl_port_add_disp() */
    esp_lcd_touch_handle_t handle;     /*!< Touch IO handle */
    lvgl_port_wait_cb touch_wait_cb;   /*!< Optional callback to wait for touch reading */
} lvgl_port_touch_cfg_t;
#endif

/**
 * @brief Default LVGL port initialization configuration macro
 */
#define ESP_LVGL_PORT_INIT_CONFIG()       \
    {                                     \
        .task_priority = 4,               \
        .task_stack = 4096,               \
        .task_affinity = -1,              \
        .task_max_sleep_ms = 5,           \
        .timer_period_ms = 5,             \
    }

/**
 * @brief Initialize LVGL port
 *
 * @param cfg Pointer to configuration structure
 *
 * @return
 *      - ESP_OK
 *      - ESP_ERR_INVALID_ARG
 *      - ESP_ERR_INVALID_STATE
 *      - ESP_ERR_NO_MEM
 */
esp_err_t lvgl_port_init(const lvgl_port_cfg_t *cfg);

/**
 * @brief Deinitialize LVGL port
 *
 * @return ESP_OK
 */
esp_err_t lvgl_port_deinit(void);

/**
 * @brief Add a display to LVGL
 *
 * @param disp_cfg Display configuration
 * @return Pointer to LVGL display handle or NULL on failure
 */
lv_disp_t *lvgl_port_add_disp(const lvgl_port_display_cfg_t *disp_cfg);

/**
 * @brief Remove a display from LVGL
 *
 * @param disp LVGL display handle
 * @return ESP_OK
 */
esp_err_t lvgl_port_remove_disp(lv_disp_t *disp);

#ifdef ESP_LVGL_PORT_TOUCH_COMPONENT
/**
 * @brief Add a touch input device to LVGL
 *
 * @param touch_cfg Touch configuration
 * @return Pointer to LVGL input device handle or NULL on failure
 */
lv_indev_t *lvgl_port_add_touch(const lvgl_port_touch_cfg_t *touch_cfg);

/**
 * @brief Remove a touch input device from LVGL
 *
 * @param touch LVGL input device handle
 * @return ESP_OK
 */
esp_err_t lvgl_port_remove_touch(lv_indev_t *touch);
#endif

/**
 * @brief Take LVGL mutex
 *
 * @param timeout_ms Timeout in milliseconds (0 = block indefinitely)
 * @return true if mutex was successfully taken, false otherwise
 */
bool lvgl_port_lock(uint32_t timeout_ms);

/**
 * @brief Release LVGL mutex
 */
void lvgl_port_unlock(void);

#ifdef __cplusplus
}
#endif
