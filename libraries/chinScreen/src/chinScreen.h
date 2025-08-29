//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Stupid library for the JC3248W525EN using lvgl because it works
//
//


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

inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
	if (debugChin == 1) {
		lv_obj_t *label = lv_label_create(lv_scr_act());
		lv_label_set_text(label, "chinScreen Works!");
		lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
		}
	
	if (debugChin == 2) {
	   // Create a full-screen container
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr); // clear screen before test

    // Array of test colors
    lv_color_t colors[] = {
        lv_palette_main(LV_PALETTE_RED),
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_BLUE),
        lv_palette_main(LV_PALETTE_YELLOW),
        lv_palette_main(LV_PALETTE_ORANGE),
        lv_palette_main(LV_PALETTE_PURPLE),
        lv_palette_main(LV_PALETTE_CYAN),
        lv_palette_main(LV_PALETTE_GREY),
    };

    const char* names[] = {
        "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "Cyan", "Grey"
    };

    // Create colored rectangles with labels
    for (int i = 0; i < 8; i++) {
        lv_obj_t *rect = lv_obj_create(scr);
        lv_obj_set_size(rect, 80, 60);
        lv_obj_set_style_bg_color(rect, colors[i], LV_PART_MAIN);
        lv_obj_align(rect, LV_ALIGN_TOP_LEFT, (i % 4) * 85, (i / 4) * 65);

        lv_obj_t *label = lv_label_create(rect);
        lv_label_set_text(label, names[i]);
        lv_obj_center(label);
    }

	}
    bsp_display_unlock();

    Serial.println("UI created");
}

void chinScreen_backlight(int bLight) {   // this needs some testing
	if (bLight == 0) {
    bsp_display_backlight_off(); 
	} else {
	bsp_display_backlight_on();
	}
}

inline void chinScreen_text(char *m) {
   bsp_display_lock(0);  

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, m);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    bsp_display_unlock();	
}