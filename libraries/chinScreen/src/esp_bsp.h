/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ESP BSP: ESP-BOX-3 (LVGL 9.3.0 compatible)
 */

#pragma once

#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "lvgl.h"
#include "lv_port.h"

/**************************************************************************************************
 *  Pinout
 **************************************************************************************************/
#define BSP_I2C_NUM                     (I2C_NUM_0)
#define BSP_I2C_CLK_SPEED_HZ            400000

#define EXAMPLE_LCD_QSPI_HOST           (SPI2_HOST)

/* LCD QSPI pins */
#define EXAMPLE_PIN_NUM_QSPI_CS         (GPIO_NUM_45)
#define EXAMPLE_PIN_NUM_QSPI_PCLK       (GPIO_NUM_47)
#define EXAMPLE_PIN_NUM_QSPI_DATA0      (GPIO_NUM_21)
#define EXAMPLE_PIN_NUM_QSPI_DATA1      (GPIO_NUM_48)
#define EXAMPLE_PIN_NUM_QSPI_DATA2      (GPIO_NUM_40)
#define EXAMPLE_PIN_NUM_QSPI_DATA3      (GPIO_NUM_39)
#define EXAMPLE_PIN_NUM_QSPI_RST        (GPIO_NUM_NC)
#define EXAMPLE_PIN_NUM_QSPI_DC         (GPIO_NUM_8)
#define EXAMPLE_PIN_NUM_QSPI_TE         (GPIO_NUM_38)
#define EXAMPLE_PIN_NUM_QSPI_BL         (GPIO_NUM_1)

/* Touch (I2C over QSPI pins if available) */
#define EXAMPLE_PIN_NUM_QSPI_TOUCH_SCL  (GPIO_NUM_8)
#define EXAMPLE_PIN_NUM_QSPI_TOUCH_SDA  (GPIO_NUM_4)
#define EXAMPLE_PIN_NUM_QSPI_TOUCH_RST  (-1)
#define EXAMPLE_PIN_NUM_QSPI_TOUCH_INT  (-1)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief BSP display configuration structure (LVGL 9.x compatible)
 *
 * Rotation is no longer in this struct; use lv_display_set_rotation()
 * after bsp_display_start_with_config() returns.
 */
typedef struct {
    lvgl_port_cfg_t lvgl_port_cfg;  /*!< Configuration for LVGL port */
    uint32_t buffer_size;           /*!< Buffer size in pixels */
} bsp_display_cfg_t;

/**
 * @brief Init I2C driver
 *
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_INVALID_ARG   I2C parameter error
 *      - ESP_FAIL              I2C driver installation error
 */
esp_err_t bsp_i2c_init(void);

/**
 * @brief Deinit I2C driver and free its resources
 *
 * @return
 *      - ESP_OK                On success
 *      - ESP_ERR_INVALID_ARG   I2C parameter error
 */
esp_err_t bsp_i2c_deinit(void);

/**
 * @brief Initialize display
 *
 * Initializes QSPI, display controller and starts LVGL handling task.
 * LCD backlight must be enabled separately by calling bsp_display_brightness_set()
 *
 * @param cfg display configuration
 * @return Pointer to LVGL display or NULL on error
 */
lv_disp_t *bsp_display_start_with_config(const bsp_display_cfg_t *cfg);

/**
 * @brief Get pointer to LVGL input device (touch, buttons, ...)
 *
 * @note LVGL indev is initialized in bsp_display_start().
 * @return Pointer to LVGL input device or NULL if not initialized
 */
lv_indev_t *bsp_display_get_input_dev(void);

/**
 * @brief Take LVGL mutex
 *
 * @param timeout_ms Timeout in ms. 0 will block indefinitely.
 * @return true if mutex was taken, false otherwise
 */
bool bsp_display_lock(uint32_t timeout_ms);

/**
 * @brief Give LVGL mutex
 */
void bsp_display_unlock(void);

#ifdef __cplusplus
}
#endif
