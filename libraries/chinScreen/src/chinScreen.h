#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"



// Initialize the display (panel, rotation, backlight)
inline void init_display() {
    Serial.println("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    Serial.println("Display init done");
}

// Create the UI elements (labels, buttons, etc.)
inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  // lock because LVGL APIs are not thread-safe
	if (debugChin == 1) {
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "chinScreen Works!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	}
    bsp_display_unlock();

    Serial.println("UI created");
}

void chinScreen_backlight_on() {
    bsp_display_backlight_on();
}

void chinScreen_backlight_off() {
    bsp_display_backlight_off();
}

inline void chinScreen_text(char *m) {
   bsp_display_lock(0);  // lock because LVGL APIs are not thread-safe

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, m);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    bsp_display_unlock();	
}