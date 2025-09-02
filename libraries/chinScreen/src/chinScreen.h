//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Improved library for the JC3248W525EN using LVGL 9.3.0
//   Updated for LVGL 9.x compatibility
// 
//   Author: Richard Loucks
//   Updated: Sept 2025 for LVGL 9.3.0
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
#include <SD.h>
#include <FS.h>
#include <SPI.h>

// SD Card Pins for JC3248WS35
#define SD_CS   10
#define SD_MOSI 11
#define SD_CLK  12
#define SD_MISO 13

// Font size constants - LVGL 9.x fonts
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24

// Animation types
typedef enum {
    ANIM_MOVE_X,
    ANIM_MOVE_Y,
    ANIM_SCALE,
    ANIM_ROTATE,
    ANIM_FADE
} chinScreen_anim_type_t;

// Button callback type - updated for LVGL 9.x
typedef void (*chinScreen_btn_callback_t)(lv_event_t* e);

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
    Serial.println("Display init done");
}

// Color system - updated for LVGL 9.x
static const char* colorNames[] = {
    "black", "white", "red", "green", "blue",
    "yellow", "cyan", "magenta", "gray", "orange",
    "purple", "pink", "brown", "navy", "lime"
};

static const lv_color_t colorValues[] = {
    lv_color_hex(0x000000),    // black
    lv_color_hex(0xFFFFFF),    // white
    lv_color_hex(0xFF0000),    // red
    lv_color_hex(0x00FF00),    // green
    lv_color_hex(0x0000FF),    // blue
    lv_color_hex(0xFFFF00),    // yellow
    lv_color_hex(0x00FFFF),    // cyan
    lv_color_hex(0xFF00FF),    // magenta
    lv_color_hex(0x808080),    // gray
    lv_color_hex(0xFFA500),    // orange
    lv_color_hex(0x800080),    // purple
    lv_color_hex(0xFFC0CB),    // pink
    lv_color_hex(0x8B4513),    // brown
    lv_color_hex(0x000080),    // navy
    lv_color_hex(0x32CD32)     // lime
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
    return lv_color_hex(0x000000); // default black if not found
}

//=============================================================================
// SCREEN MANAGEMENT FUNCTIONS
//=============================================================================

// Safe screen clearing - updated for LVGL 9.x
inline void chinScreen_clear() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_screen_active();
    lv_obj_clean(scr);
    // Reset background to black
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_invalidate(scr);
    bsp_display_unlock();
}

// Set solid background color
inline void chinScreen_background_solid(const char* colorName) {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    bsp_display_unlock();
}

// Set gradient background
inline void chinScreen_background_gradient(const char* color1Name, const char* color2Name, 
                                          const char* direction = "vertical") {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_screen_active();
    
    lv_color_t color1 = getColorByName(color1Name);
    lv_color_t color2 = getColorByName(color2Name);
    
    lv_grad_dir_t grad_dir = LV_GRAD_DIR_VER;
    if (strcmp(direction, "horizontal") == 0) grad_dir = LV_GRAD_DIR_HOR;
    
    lv_obj_set_style_bg_color(scr, color1, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(scr, color2, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(scr, grad_dir, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
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
// TEXT FUNCTIONS
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

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    bsp_display_unlock();
}

// Add text inside a shape
inline void chinScreen_shape_text(lv_obj_t* shape, const char* text, 
                                 const char* colorName = "white", 
                                 const char* fontSize = "medium") {
    if (!shape) return;
    
    bsp_display_lock(0);

    lv_obj_t *label = lv_label_create(shape);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), LV_PART_MAIN);
    lv_obj_center(label);

    bsp_display_unlock();
}

//=============================================================================
// SHAPE FUNCTIONS - Updated for LVGL 9.x
//=============================================================================

// Rectangle with optional gradient
inline lv_obj_t* chinScreen_rectangle(const char* bgColorName,
                                     const char* borderColorName,
                                     int width, int height,
                                     const char* vAlign = "middle",
                                     const char* hAlign = "center",
                                     const char* gradientColor = nullptr,
                                     const char* gradientDir = "vertical") {
    bsp_display_lock(0);

    lv_obj_t *rect = lv_obj_create(lv_screen_active());
    lv_obj_set_size(rect, width, height);

    // Background color or gradient
    if (gradientColor != nullptr) {
        lv_color_t color1 = getColorByName(bgColorName);
        lv_color_t color2 = getColorByName(gradientColor);
        lv_grad_dir_t grad_dir = strcmp(gradientDir, "horizontal") == 0 ? LV_GRAD_DIR_HOR : LV_GRAD_DIR_VER;
        
        lv_obj_set_style_bg_color(rect, color1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(rect, color2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(rect, grad_dir, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(rect, getColorByName(bgColorName), LV_PART_MAIN);
    }
    
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(rect, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 3, LV_PART_MAIN);
    lv_obj_set_style_border_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

    // Positioning - updated alignment constants
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(rect, align, 0, 0);

    bsp_display_unlock();
    return rect;
}

// Circle with optional gradient
inline lv_obj_t* chinScreen_circle(const char* fillColorName, const char* borderColorName, 
                                  uint16_t radius, const char* verticalPos = "middle", 
                                  const char* horizontalPos = "center",
                                  const char* gradientColor = nullptr,
                                  const char* gradientDir = "vertical") {
    bsp_display_lock(0);

    lv_obj_t *circle = lv_obj_create(lv_screen_active());
    lv_obj_set_size(circle, radius * 2, radius * 2);

    // Background
    if (gradientColor != nullptr) {
        lv_color_t color1 = getColorByName(fillColorName);
        lv_color_t color2 = getColorByName(gradientColor);
        lv_grad_dir_t grad_dir = strcmp(gradientDir, "horizontal") == 0 ? LV_GRAD_DIR_HOR : LV_GRAD_DIR_VER;
        
        lv_obj_set_style_bg_color(circle, color1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(circle, color2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(circle, grad_dir, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(circle, getColorByName(fillColorName), LV_PART_MAIN);
    }
    
    lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(circle, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(circle, 3, LV_PART_MAIN);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(circle, align, 0, 0);

    bsp_display_unlock();
    return circle;
}

// Continue with other shape functions...
// [Rest of the shapes would follow similar pattern with LVGL 9.x updates]

//=============================================================================
// BUTTON AND CONTROL FUNCTIONS - Updated for LVGL 9.x
//=============================================================================

// Create button
inline lv_obj_t* chinScreen_button(const char* bgColorName, const char* borderColorName,
                                   int width, int height, const char* text,
                                   chinScreen_btn_callback_t callback,
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *btn = lv_button_create(lv_screen_active());  // Updated function name
    lv_obj_set_size(btn, width, height);
    lv_obj_set_style_bg_color(btn, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_border_color(btn, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 3, LV_PART_MAIN);

    // Add text if provided
    if (text && strlen(text) > 0) {
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, text);
        lv_obj_center(label);
    }

    // Add callback if provided
    if (callback) {
        lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
    }

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(btn, align, 0, 0);

    bsp_display_unlock();
    return btn;
}

//=============================================================================
// IMAGE FUNCTIONS - Updated for LVGL 9.x
//=============================================================================

// Display image using LVGL filesystem (supports PNG, BMP, GIF)
inline lv_obj_t* chinScreen_image(const char* filepath, int x = -1, int y = -1,
                                  const char* vAlign = "middle", 
                                  const char* hAlign = "center") {
    if (!SD.exists(filepath)) {
        Serial.printf("Image file not found: %s\n", filepath);
        return nullptr;
    }
    
    bsp_display_lock(0);
    
    lv_obj_t *img = lv_image_create(lv_screen_active());  // Updated function name
    
    // Convert path to LVGL filesystem format
    char lvgl_path[64];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", filepath);
    
    // Let LVGL handle the image loading with its decoders
    lv_image_set_src(img, lvgl_path);  // Updated function name
    
    // Position the image
    if (x >= 0 && y >= 0) {
        lv_obj_set_pos(img, x, y);
    } else {
        lv_align_t align = LV_ALIGN_CENTER;
        if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
        else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
        else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

        lv_obj_align(img, align, 0, 0);
    }

    bsp_display_unlock();
    return img;
}

//=============================================================================
// ANIMATION FUNCTIONS - Updated for LVGL 9.x
//=============================================================================

static void anim_ready_cb(lv_anim_t* a) {
    Serial.println("Animation completed");
}

// Basic animation - updated for LVGL 9.x
inline void chinScreen_animate(lv_obj_t* obj, chinScreen_anim_type_t anim_type, 
                              int32_t start_value, int32_t end_value, 
                              uint32_t duration = 1000, bool repeat = false) {
    if (!obj) return;

    bsp_display_lock(0);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_values(&anim, start_value, end_value);

    switch (anim_type) {
        case ANIM_MOVE_X:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
            break;
        case ANIM_MOVE_Y:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
            break;
        case ANIM_SCALE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_width);
            break;
        case ANIM_ROTATE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_rotation);
            start_value *= 10;
            end_value *= 10;
            break;
        case ANIM_FADE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
            break;
    }

    if (repeat) {
        lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_playback_time(&anim, duration);
    }

    lv_anim_start(&anim);
    bsp_display_unlock();
}

//=============================================================================
// UTILITY FUNCTIONS
//=============================================================================

// Delete an object
inline void chinScreen_delete_object(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_delete(obj);  // Updated function name
    bsp_display_unlock();
}

//=============================================================================
// DEBUG AND LEGACY FUNCTIONS - Updated for LVGL 9.x
//=============================================================================

// Debug function
inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
    
    if (debugChin == 1) {
        lv_obj_clean(lv_screen_active());
        
        lv_obj_t *label = lv_label_create(lv_screen_active());
        lv_label_set_text(label, "chinScreen Works!");
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
    
    if (debugChin == 2) {
        lv_obj_clean(lv_screen_active());

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

        for (int i = 0; i < 8; i++) {
            lv_obj_t *rect = lv_obj_create(lv_screen_active());
            lv_obj_set_size(rect, 80, 60);
            lv_obj_set_style_bg_color(rect, colors[i], LV_PART_MAIN);
            lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_align(rect, LV_ALIGN_TOP_LEFT, (i % 4) * 85, (i / 4) * 65);

            lv_obj_t *label = lv_label_create(rect);
            lv_label_set_text(label, names[i]);
            lv_obj_center(label);
        }
    }
    
    bsp_display_unlock();
    Serial.println("UI created");
}

// Example button callback
inline void example_button_callback(lv_event_t* e) {
    Serial.println("Button clicked!");
}