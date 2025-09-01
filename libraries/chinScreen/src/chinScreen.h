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

// Common definitions used across modules
#define CHINSCREEN_MAX_COLORS 15

// Font size constants (LVGL 9.3.0 compatible)
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24

// Animation types
typedef enum {
    ANIM_MOVE_X,
    ANIM_MOVE_Y,
    ANIM_SCALE_X,
    ANIM_SCALE_Y,
    ANIM_ROTATE,
    ANIM_FADE
} chinScreen_anim_type_t;

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
// CORE DISPLAY INITIALIZATION (Always Available)
//=============================================================================

inline void chinScreen_init_display() {
    Serial.println("chinScreen: Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
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

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    Serial.println("chinScreen: Display initialization complete");
}

// Backward compatibility alias
inline void init_display() {
    chinScreen_init_display();
}

//=============================================================================
// MODULE INITIALIZATION FUNCTIONS
//=============================================================================

// Initialize commands module (screen management, backgrounds, etc.)
inline bool chinScreen_init_commands() {
    if (chinScreen_commands_loaded) {
        Serial.println("chinScreen: Commands module already loaded");
        return true;
    }
    
    #include "chinScreen_commands.h"
    chinScreen_commands_loaded = true;
    Serial.println("chinScreen: Commands module loaded");
    return true;
}

// Initialize shapes module (rectangles, circles, polygons, etc.)
inline bool chinScreen_init_shapes() {
    if (chinScreen_shapes_loaded) {
        Serial.println("chinScreen: Shapes module already loaded");
        return true;
    }
    
    #include "chinScreen_shapes.h"
    chinScreen_shapes_loaded = true;
    Serial.println("chinScreen: Shapes module loaded");
    return true;
}

// Initialize interface module (buttons, sliders, keyboards, etc.)
inline bool chinScreen_init_interface() {
    if (chinScreen_interface_loaded) {
        Serial.println("chinScreen: Interface module already loaded");
        return true;
    }
    
    #include "chinScreen_interface.h"
    chinScreen_interface_loaded = true;
    Serial.println("chinScreen: Interface module loaded");
    return true;
}

// Initialize images module (PNG, JPG, BMP support, etc.)
inline bool chinScreen_init_images() {
    if (chinScreen_images_loaded) {
        Serial.println("chinScreen: Images module already loaded");
        return true;
    }
    
    #include "chinScreen_images.h"
    chinScreen_images_loaded = true;
    Serial.println("chinScreen: Images module loaded");
    return true;
}

// Initialize video module (MJPEG support, etc.)
inline bool chinScreen_init_video() {
    if (chinScreen_video_loaded) {
        Serial.println("chinScreen: Video module already loaded");
        return true;
    }
    
    #include "chinScreen_video.h"
    chinScreen_video_loaded = true;
    Serial.println("chinScreen: Video module loaded");
    return true;
}

// Initialize all modules at once
inline void chinScreen_init_all() {
    Serial.println("chinScreen: Loading all modules...");
    chinScreen_init_commands();
    chinScreen_init_shapes();
    chinScreen_init_interface();
    chinScreen_init_images();
    chinScreen_init_video();
    Serial.println("chinScreen: All modules loaded");
}

//=============================================================================
// MODULE STATUS AND UTILITY FUNCTIONS
//=============================================================================

// Check if a specific module is loaded
inline bool chinScreen_is_module_loaded(const char* module_name) {
    if (strcmp(module_name, "commands") == 0) return chinScreen_commands_loaded;
    if (strcmp(module_name, "shapes") == 0) return chinScreen_shapes_loaded;
    if (strcmp(module_name, "interface") == 0) return chinScreen_interface_loaded;
    if (strcmp(module_name, "images") == 0) return chinScreen_images_loaded;
    if (strcmp(module_name, "video") == 0) return chinScreen_video_loaded;
    return false;
}

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
    Serial.printf("LVGL Version: %d.%d.%d\n", 
                  LVGL_VERSION_MAJOR, LVGL_VERSION_MINOR, LVGL_VERSION_PATCH);
    Serial.println("Loaded Modules:");
    Serial.printf("  Commands: %s\n", chinScreen_commands_loaded ? "YES" : "NO");
    Serial.printf("  Shapes: %s\n", chinScreen_shapes_loaded ? "YES" : "NO");
    Serial.printf("  Interface: %s\n", chinScreen_interface_loaded ? "YES" : "NO");
    Serial.printf("  Images: %s\n", chinScreen_images_loaded ? "YES" : "NO");
    Serial.printf("  Video: %s\n", chinScreen_video_loaded ? "YES" : "NO");
    Serial.println("================================");
}

//=============================================================================
// LVGL 9.3.0 COMPATIBILITY HELPERS
//=============================================================================

// Helper for thread-safe display operations (LVGL 9.3.0 style)
inline void chinScreen_lock_display() {
    bsp_display_lock(0);
}

inline void chinScreen_unlock_display() {
    bsp_display_unlock();
}

// LVGL 9.3.0 alignment helper
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
// BACKWARD COMPATIBILITY WARNING SYSTEM
//=============================================================================

// Warn user about deprecated function usage
inline void chinScreen_warn_deprecated(const char* old_func, const char* new_func) {
    Serial.printf("WARNING: %s is deprecated. Use %s instead.\n", old_func, new_func);
}

// Legacy function detection - these will call the new modular functions
// but with deprecation warnings

inline void getColorByName_DEPRECATED(const char* name) {
    chinScreen_warn_deprecated("getColorByName", "chinScreen_get_color");
}

//=============================================================================
// EXAMPLE USAGE DOCUMENTATION
//=============================================================================

/*
=== chinScreen v1.0.0 - Modular Architecture Usage ===

Basic Setup:
#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    chinScreen_init_display();  // Always required
    
    // Load only the modules you need:
    chinScreen_init_commands();   // For screen management, backgrounds
    chinScreen_init_shapes();     // For rectangles, circles, etc.
    chinScreen_init_interface();  // For buttons, sliders, keyboards
    chinScreen_init_images();     // For PNG/JPG display
    chinScreen_init_video();      // For video playback
    
    // Or load everything:
    // chinScreen_init_all();
    
    // Check status:
    chinScreen_print_status();
}

Module Functions:
- Commands: chinScreen_clear(), chinScreen_background_solid(), chinScreen_brightness()
- Shapes: chinScreen_rectangle(), chinScreen_circle(), chinScreen_triangle()  
- Interface: chinScreen_button(), chinScreen_slider(), chinScreen_keyboard()
- Images: chinScreen_image(), chinScreen_image_button()
- Video: chinScreen_video_load(), chinScreen_video_play()

LVGL 9.3.0 Updates:
- Thread-safe operations with chinScreen_lock_display()/chinScreen_unlock_display()
- Updated rotation constants (LV_DISPLAY_ROTATION_*)
- Improved memory management
- Better alignment system with chinScreen_get_alignment()

========================================================
*/