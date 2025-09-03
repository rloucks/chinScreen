//       _     _       __                        
//   ___| |__ (_)_ __ / _\ ___ _ __ ___  ___ _ __   
//  / __| '_ \| | '_ \\ \ / __| '__/ _ \/ _ \ '_ \ 
// | (__| | | | | | | |\ \ (__| | |  __/  __/ | | |
//  \___|_| |_|_|_| |_\__/\___|_|  \___|\___|_| |_| 
//
//   Modular library for the JC3248W525EN using LVGL 9.3.0
//   Improved architecture with optional module loading
// 
//   Author: Richard Loucks
//   Version: 0.1.1
//   Updated: Sept 2025 - Modular Architecture
//   For updates, see git commits
//   https://github.com/rloucks/chinScreen
//
//   Hardware based for JC3248W525EN touch screen ESP32-S3 Panel
//

#pragma once
#include <Arduino.h>
#include <lvgl.h>

// Use your existing display and BSP headers
#include "display.h"
#include "esp_bsp.h" 
#include "lv_port.h"
#include <ctype.h>

// Library version
#define CHINSCREEN_VERSION_MAJOR 1
#define CHINSCREEN_VERSION_MINOR 0
#define CHINSCREEN_VERSION_PATCH 0

// Module availability flags
static bool chinScreen_commands_loaded = false;
static bool chinScreen_shapes_loaded = false;
static bool chinScreen_interface_loaded = false;
static bool chinScreen_images_loaded = false;
static bool chinScreen_video_loaded = false;

static lv_display_t *chinScreen_display = NULL;

// Common definitions used across modules
#define CHINSCREEN_MAX_COLORS 15

// Font size constants - use whatever fonts are available in your LVGL
#ifndef FONT_SMALL
#define FONT_SMALL   &lv_font_montserrat_12
#endif
#ifndef FONT_MEDIUM
#define FONT_MEDIUM  &lv_font_montserrat_16
#endif
#ifndef FONT_LARGE
#define FONT_LARGE   &lv_font_montserrat_20
#endif
#ifndef FONT_XLARGE
#define FONT_XLARGE  &lv_font_montserrat_24
#endif

// User can define this in their sketch before including chinScreen.h
#ifndef LVGL_PORT_ROTATION_DEGREE
#define LVGL_PORT_ROTATION_DEGREE 0
#endif

// Button callback type
typedef void (*chinScreen_btn_callback_t)(lv_event_t* e);

//=============================================================================
// CORE COLOR SYSTEM (Always Available)
//=============================================================================

static const char* colorNames[CHINSCREEN_MAX_COLORS] = {
    "black", "white", "red", "green", "blue",
    "yellow", "cyan", "magenta", "gray", "orange",
    "purple", "pink", "brown", "navy", "lime"
};

static const lv_color_t colorValues[CHINSCREEN_MAX_COLORS] = {
    lv_color_make(0,0,0),       // black
    lv_color_make(255,255,255), // white
    lv_color_make(255,0,0),     // red
    lv_color_make(0,255,0),     // green
    lv_color_make(0,0,255),     // blue
    lv_color_make(255,255,0),   // yellow
    lv_color_make(0,255,255),   // cyan
    lv_color_make(255,0,255),   // magenta
    lv_color_make(128,128,128), // gray
    lv_color_make(255,165,0),   // orange
    lv_color_make(128,0,128),   // purple
    lv_color_make(255,192,203), // pink
    lv_color_make(139,69,19),   // brown
    lv_color_make(0,0,128),     // navy
    lv_color_make(50,205,50)    // lime
};

inline lv_color_t chinScreen_get_color(const char* name) {
    for (uint8_t i = 0; i < CHINSCREEN_MAX_COLORS; i++) {
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
    return lv_color_make(0,0,0); // default black if not found
}

inline const lv_font_t* chinScreen_get_font(const char* size) {
    if (strcmp(size, "small") == 0) return FONT_SMALL;
    if (strcmp(size, "medium") == 0) return FONT_MEDIUM;
    if (strcmp(size, "large") == 0) return FONT_LARGE;
    if (strcmp(size, "xlarge") == 0) return FONT_XLARGE;
    return FONT_MEDIUM; // default
}

//=============================================================================
// CORE DISPLAY INITIALIZATION - Use your original working code
//=============================================================================


inline void init_display() {
    Serial.println("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = 320 * 40,  // Use smaller buffer that worked before
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISPLAY_ROTATION_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISPLAY_ROTATION_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISPLAY_ROTATION_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISPLAY_ROTATION_0,
#endif
    };

    lv_display_t* disp = bsp_display_start_with_config(&cfg);  // LVGL 9.x uses lv_display_t*
    if (disp == NULL) {
        Serial.println("Failed to initialize display!");
        return;
    }
    
    bsp_display_backlight_on();
    Serial.println("Display init done");
}

// Optional helper function
inline void chinScreen_init_display() {
    init_display();
}

inline void init_display_simple() {
    Serial.println("Initialize panel device - Simple version");

    // Initialize LVGL first
    lv_init();
    
    Serial.println("LVGL initialized");
    
    // Create a basic display (this is a simplified approach)
    lv_display_t *disp = lv_display_create(320, 480);
    if (disp == NULL) {
        Serial.println("Failed to create LVGL display!");
        return;
    }
    
    // Allocate draw buffer
    static lv_color_t buf1[320 * 40]; // 40 lines buffer
    lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
    
    // Set a dummy flush callback for now
    lv_display_set_flush_cb(disp, [](lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
        // Dummy flush - just tell LVGL we're done
        lv_display_flush_ready(disp);
    });

    Serial.println("Display init done - Simple version");
}
//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

// Helper for thread-safe display operations
inline void chinScreen_lock_display() {
    bsp_display_lock(0);
}

inline void chinScreen_unlock_display() {
    bsp_display_unlock();
}

// Alignment helper
inline lv_align_t chinScreen_get_alignment(const char* vAlign, const char* hAlign) {
    if (strcmp(vAlign, "top") == 0) {
        if (strcmp(hAlign, "left") == 0) return LV_ALIGN_TOP_LEFT;
        if (strcmp(hAlign, "center") == 0) return LV_ALIGN_TOP_MID;
        if (strcmp(hAlign, "right") == 0) return LV_ALIGN_TOP_RIGHT;
    }
    else if (strcmp(vAlign, "middle") == 0) {
        if (strcmp(hAlign, "left") == 0) return LV_ALIGN_LEFT_MID;
        if (strcmp(hAlign, "center") == 0) return LV_ALIGN_CENTER;
        if (strcmp(hAlign, "right") == 0) return LV_ALIGN_RIGHT_MID;
    }
    else if (strcmp(vAlign, "bottom") == 0) {
        if (strcmp(hAlign, "left") == 0) return LV_ALIGN_BOTTOM_LEFT;
        if (strcmp(hAlign, "center") == 0) return LV_ALIGN_BOTTOM_MID;
        if (strcmp(hAlign, "right") == 0) return LV_ALIGN_BOTTOM_RIGHT;
    }
    return LV_ALIGN_CENTER; // default
}

//=============================================================================
// MODULE INITIALIZATION FUNCTIONS (Simple flag setting)
//=============================================================================

inline bool chinScreen_init_commands() {
    if (chinScreen_commands_loaded) {
        Serial.println("chinScreen: Commands module already loaded");
        return true;
    }
    chinScreen_commands_loaded = true;
    Serial.println("chinScreen: Commands module loaded");
    return true;
}

inline bool chinScreen_init_shapes() {
    if (chinScreen_shapes_loaded) {
        Serial.println("chinScreen: Shapes module already loaded");
        return true;
    }
    chinScreen_shapes_loaded = true;
    Serial.println("chinScreen: Shapes module loaded");
    return true;
}

inline bool chinScreen_init_interface() {
    if (chinScreen_interface_loaded) {
        Serial.println("chinScreen: Interface module already loaded");
        return true;
    }
    chinScreen_interface_loaded = true;
    Serial.println("chinScreen: Interface module loaded");
    return true;
}

inline bool chinScreen_init_images() {
    if (chinScreen_images_loaded) {
        Serial.println("chinScreen: Images module already loaded");
        return true;
    }
    chinScreen_images_loaded = true;
    Serial.println("chinScreen: Images module loaded");
    return true;
}

inline bool chinScreen_init_video() {
    if (chinScreen_video_loaded) {
        Serial.println("chinScreen: Video module already loaded");
        return true;
    }
    chinScreen_video_loaded = true;
    Serial.println("chinScreen: Video module loaded");
    return true;
}

//=============================================================================
// BASIC SHAPE FUNCTIONS (Compatible with your LVGL version)
//=============================================================================

// Clear screen
inline void chinScreen_clear() {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    
    // Reset background to black
    lv_obj_set_style_bg_color(scr, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    chinScreen_unlock_display();
}

// Set background color
inline void chinScreen_background_solid(const char* colorName) {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    chinScreen_unlock_display();
}

// Simple rectangle function
inline lv_obj_t* chinScreen_rectangle(const char* fillColor,
                                     const char* borderColor,
                                     int width, int height,
                                     const char* vAlign = "middle",
                                     const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t *rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, width, height);
    
    // Apply colors
    lv_obj_set_style_bg_color(rect, chinScreen_get_color(fillColor), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(rect, chinScreen_get_color(borderColor), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 3, LV_PART_MAIN);
    lv_obj_set_style_border_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

    // Position the rectangle
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(rect, align, 0, 0);

    chinScreen_unlock_display();
    return rect;
}

// Simple circle function
inline lv_obj_t* chinScreen_circle(const char* fillColor, 
                                  const char* borderColor, 
                                  int radius, 
                                  const char* vAlign = "middle", 
                                  const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t *circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, radius * 2, radius * 2);
    
    lv_obj_set_style_bg_color(circle, chinScreen_get_color(fillColor), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(circle, chinScreen_get_color(borderColor), LV_PART_MAIN);
    lv_obj_set_style_border_width(circle, 3, LV_PART_MAIN);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(circle, align, 0, 0);

    chinScreen_unlock_display();
    return circle;
}

// Simple text function
inline lv_obj_t* chinScreen_text(const char* text, 
                                int x, int y, 
                                const char* colorName = "white", 
                                const char* fontSize = "medium") {
    chinScreen_lock_display();

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_get_font(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    chinScreen_unlock_display();
    return label;
}

// Simple button function
inline lv_obj_t* chinScreen_button(const char* text,
                                  chinScreen_btn_callback_t callback,
                                  const char* vAlign = "middle",
                                  const char* hAlign = "center",
                                  int width = 100,
                                  int height = 50) {
    chinScreen_lock_display();

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, width, height);

    // Add text label if provided
    if (text && strlen(text) > 0) {
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, text);
        lv_obj_center(label);
    }

    // Add event callback
    if (callback) {
        lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
    }

    // Position button
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(btn, align, 0, 0);

    chinScreen_unlock_display();
    return btn;
}

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

// Get library version as string
inline const char* chinScreen_get_version() {
    static char version_str[16];
    snprintf(version_str, sizeof(version_str), "%d.%d.%d", 
             CHINSCREEN_VERSION_MAJOR, 
             CHINSCREEN_VERSION_MINOR, 
             CHINSCREEN_VERSION_PATCH);
    return version_str;
}

// Print module status
inline void chinScreen_print_status() {
    Serial.println("=== chinScreen Library Status ===");
    Serial.printf("Version: %s\n", chinScreen_get_version());
    Serial.println("Loaded Modules:");
    Serial.printf("  Commands: %s\n", chinScreen_commands_loaded ? "YES" : "NO");
    Serial.printf("  Shapes: %s\n", chinScreen_shapes_loaded ? "YES" : "NO");
    Serial.printf("  Interface: %s\n", chinScreen_interface_loaded ? "YES" : "NO");
    Serial.printf("  Images: %s\n", chinScreen_images_loaded ? "YES" : "NO");
    Serial.printf("  Video: %s\n", chinScreen_video_loaded ? "YES" : "NO");
    Serial.println("================================");
}