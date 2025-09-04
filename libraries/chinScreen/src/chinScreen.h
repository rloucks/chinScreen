//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Minimal LVGL 9.3.0 Compatible Version
//   Basic display and text functionality only
// 
//   Author: Richard Loucks
//   LVGL 9.3.0 Update: Sept 2025
//   https://github.com/rloucks/chinScreen
//
//   Hardware: JC3248W525EN touch screen ESP32-S3 Panel
//

#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <ctype.h>

// Font size constants - Updated for LVGL 9
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24

//=============================================================================
// CORE DISPLAY FUNCTIONS
//=============================================================================

// Initialize the display (panel, rotation, backlight)
inline void init_display() {
    Serial.println("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISPLAY_ROTATION_90,           // UPDATED: enum names
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISPLAY_ROTATION_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISPLAY_ROTATION_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISPLAY_ROTATION_0,            // UPDATED: was LV_DISP_ROT_NONE
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    Serial.println("Display init done");
}

//=============================================================================
// COLOR SYSTEM - UPDATED FOR LVGL 9
//=============================================================================

static const char* colorNames[] = {
    "black", "white", "red", "green", "blue",
    "yellow", "cyan", "magenta", "gray", "orange",
    "purple", "pink", "brown", "navy", "lime"
};

// UPDATED: Color creation for LVGL 9
static const lv_color_t colorValues[] = {
    {.blue = 0,   .green = 0,   .red = 0},      // black
    {.blue = 255, .green = 255, .red = 255},    // white
    {.blue = 0,   .green = 0,   .red = 255},    // red
    {.blue = 0,   .green = 255, .red = 0},      // green
    {.blue = 255, .green = 0,   .red = 0},      // blue
    {.blue = 0,   .green = 255, .red = 255},    // yellow
    {.blue = 255, .green = 255, .red = 0},      // cyan
    {.blue = 255, .green = 0,   .red = 255},    // magenta
    {.blue = 128, .green = 128, .red = 128},    // gray
    {.blue = 0,   .green = 165, .red = 255},    // orange
    {.blue = 128, .green = 0,   .red = 128},    // purple
    {.blue = 203, .green = 192, .red = 255},    // pink
    {.blue = 19,  .green = 69,  .red = 139},    // brown
    {.blue = 128, .green = 0,   .red = 0},      // navy
    {.blue = 50,  .green = 205, .red = 50}      // lime
};

inline lv_color_t getColorByName(const char* name) {
    for (uint8_t i = 0; i < sizeof(colorNames)/sizeof(colorNames[0]); i++) {
        const char* p1 = name;
        const char* p2 = colorNames[i];
        bool match = true;
        while (*p1 && *p2) {
            if (tolower(*p1) != tolower(*p2)) {
                match = false;
                break;
            }
            p1++;
            p2++;
        }
        if (match && *p1 == '\0' && *p2 == '\0') {
            return colorValues[i];
        }
    }
    return colorValues[0]; // default black if not found
}

//=============================================================================
// SCREEN MANAGEMENT FUNCTIONS - UPDATED FOR LVGL 9
//=============================================================================

// Safe screen clearing
inline void chinScreen_clear() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_screen_active();        // UPDATED: lv_scr_act() → lv_screen_active()
    lv_obj_clean(scr);
    
    // Reset background to black - UPDATED: style API
    lv_obj_set_style_bg_color(scr, colorValues[0], 0);  // black
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    lv_obj_invalidate(scr);
    bsp_display_unlock();
}

// Set solid background color
inline void chinScreen_background_solid(const char* colorName) {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_screen_active();        // UPDATED: lv_scr_act() → lv_screen_active()
    lv_obj_set_style_bg_color(scr, getColorByName(colorName), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    bsp_display_unlock();
}

// Backlight control
inline void chinScreen_backlight(int bLight) {   
    if (bLight == 0) {
        bsp_display_backlight_off(); 
    } else {
        bsp_display_backlight_on();
    }
}

// Brightness control
inline void chinScreen_brightness(int howBright) {   
    bsp_display_brightness_set(howBright);
}

//=============================================================================
// TEXT FUNCTIONS - UPDATED FOR LVGL 9
//=============================================================================

inline const lv_font_t* getFontBySize(const char* size) {
    if (strcmp(size, "small") == 0) return FONT_SMALL;
    if (strcmp(size, "medium") == 0) return FONT_MEDIUM;
    if (strcmp(size, "large") == 0) return FONT_LARGE;
    if (strcmp(size, "xlarge") == 0) return FONT_XLARGE;
    return FONT_MEDIUM; // default
}

inline void chinScreen_text(const char* text, int x, int y, 
                           const char* colorName = "white", 
                           const char* fontSize = "medium") {
    bsp_display_lock(0);

    lv_obj_t *label = lv_label_create(lv_screen_active());  // UPDATED: parent parameter
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), 0);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), 0);
    lv_obj_set_pos(label, x, y);

    bsp_display_unlock();
}

//=============================================================================
// BASIC SHAPE FUNCTIONS - MINIMAL IMPLEMENTATIONS
//=============================================================================

// Simple rectangle with basic styling
inline lv_obj_t* chinScreen_rectangle(const char* bgColorName,
                                     const char* borderColorName,
                                     int width, int height,
                                     const char* vAlign = "middle",
                                     const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *rect = lv_obj_create(lv_screen_active());    // UPDATED: parent parameter
    lv_obj_set_size(rect, width, height);

    // UPDATED: Style API - simplified for LVGL 9
    lv_obj_set_style_bg_color(rect, getColorByName(bgColorName), 0);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(rect, getColorByName(borderColorName), 0);
    lv_obj_set_style_border_width(rect, 3, 0);

    // UPDATED: Alignment system 
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);  // Simplified - just center for now

    bsp_display_unlock();
    return rect;
}

// Add text inside a shape
inline void chinScreen_shape_text(lv_obj_t* shape, const char* text, 
                                 const char* colorName = "white", 
                                 const char* fontSize = "medium") {
    if (!shape) return;
    
    bsp_display_lock(0);

    lv_obj_t *label = lv_label_create(shape);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), 0);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), 0);
    lv_obj_center(label);

    bsp_display_unlock();
}

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

// Set object position
inline void chinScreen_set_position(lv_obj_t* obj, int x, int y) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_set_pos(obj, x, y);
    bsp_display_unlock();
}

// Delete an object
inline void chinScreen_delete_object(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_delete(obj);                        // UPDATED: lv_obj_del() → lv_obj_delete()
    bsp_display_unlock();
}

//=============================================================================
// DEBUG AND LEGACY FUNCTIONS - UPDATED
//=============================================================================

// Debug function - simplified
inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
    
    if (debugChin == 1) {
        lv_obj_clean(lv_screen_active());      // UPDATED: lv_scr_act() → lv_screen_active()
        
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "chinScreen Works!");
        lv_obj_center(label);
    }
    
    if (debugChin == 2) {
        lv_obj_clean(lv_screen_active());

        // Simple color test - create a few rectangles
        for (int i = 0; i < 4; i++) {
            lv_obj_t *rect = lv_obj_create(lv_screen_active());
            lv_obj_set_size(rect, 80, 60);
            
            // Use different colors
            switch(i) {
                case 0: lv_obj_set_style_bg_color(rect, colorValues[2], 0); break; // red
                case 1: lv_obj_set_style_bg_color(rect, colorValues[3], 0); break; // green  
                case 2: lv_obj_set_style_bg_color(rect, colorValues[4], 0); break; // blue
                case 3: lv_obj_set_style_bg_color(rect, colorValues[5], 0); break; // yellow
            }
            
            lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);
            lv_obj_set_pos(rect, (i % 2) * 100 + 50, (i / 2) * 80 + 50);
        }
    }
    
    bsp_display_unlock();
    Serial.println("UI created");
}