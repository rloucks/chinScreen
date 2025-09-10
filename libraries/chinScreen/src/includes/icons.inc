//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Improved library for the JC3248W525EN using LVGL based on 
//   too many attempts on the internet. This should be easy to useful
// 
//   Author: Richard Loucks
//   Inital Release: Sept 2025
//   For updates, see git commits
//   https://github.com/rloucks/chinScreen
//
//   Version: 0.0.2b
//
//   Hardware based for JC3248W525EN touch screen ESP32-S3 Panel
//
//   File: icons.h
//   Purpose: icons calls and library - WIP
//   Required: NO



// icons.inc - Enhanced Icon management for chinScreen library with color support
#pragma once

// Conditional icon includes - your existing system
#ifdef CHINSCREEN_ICON_HOME
#include "includes/icons/png/home.inc"
extern const unsigned char home_png[];
extern const unsigned int home_png_len;
#endif

#ifdef CHINSCREEN_ICON_SETTINGS
#include "includes/icons/png/settings.inc"
extern const unsigned char settings_png[];
extern const unsigned int settings_png_len;
#endif

#ifdef CHINSCREEN_ICON_USER
#include "includes/icons/png/user.inc"
extern const unsigned char user_png[];
extern const unsigned int user_png_len;
#endif

#ifdef CHINSCREEN_ICON_MENU
#include "includes/icons/png/menu.inc"
extern const unsigned char menu_png[];
extern const unsigned int menu_png_len;
#endif

#ifdef CHINSCREEN_ICON_BACK
#include "includes/icons/png/back.inc"
extern const unsigned char back_png[];
extern const unsigned int back_png_len;
#endif

#ifdef CHINSCREEN_ICON_FORWARD
#include "includes/icons/png/forward.inc"
extern const unsigned char forward_png[];
extern const unsigned int forward_png_len;
#endif

#ifdef CHINSCREEN_ICON_PLAY
#include "includes/icons/png/play.inc"
extern const unsigned char play_png[];
extern const unsigned int play_png_len;
#endif

#ifdef CHINSCREEN_ICON_PAUSE
#include "includes/icons/png/pause.inc"
extern const unsigned char pause_png[];
extern const unsigned int pause_png_len;
#endif

#ifdef CHINSCREEN_ICON_STOP
#include "includes/icons/png/stop.inc"
extern const unsigned char stop_png[];
extern const unsigned int stop_png_len;
#endif

#ifdef CHINSCREEN_ICON_VOLUME
#include "includes/icons/png/volume.inc"
extern const unsigned char volume_png[];
extern const unsigned int volume_png_len;
#endif

#ifdef CHINSCREEN_ICON_POWER
#include "includes/icons/png/power.inc"
extern const unsigned char power_png[];
extern const unsigned int power_png_len;
#endif

#ifdef CHINSCREEN_ICON_BATTERY
#include "includes/icons/png/battery.inc"
extern const unsigned char battery_png[];
extern const unsigned int battery_png_len;

#include "includes/icons/png/battery_charging.inc"
extern const unsigned char battery_charging_png[];
extern const unsigned int battery_charging_png_len;

#include "includes/icons/png/battery_low.inc"
extern const unsigned char battery_low_png[];
extern const unsigned int battery_low_png_len;

#include "includes/icons/png/battery_medium.inc"
extern const unsigned char battery_medium_png[];
extern const unsigned int battery_medium_png_len;

#include "includes/icons/png/battery_full.inc"
extern const unsigned char battery_full_png[];
extern const unsigned int battery_full_png_len;
#endif

#ifdef CHINSCREEN_ICON_SIGNAL
#include "includes/icons/png/signal.inc"
extern const unsigned char signal_png[];
extern const unsigned int signal_png_len;
#endif

#ifdef CHINSCREEN_ICON_WIFI
#include "includes/icons/png/wifi.inc"
extern const unsigned char wifi_png[];
extern const unsigned int wifi_png_len;

#include "includes/icons/png/wifi_low.inc"
extern const unsigned char wifi_low_png[];
extern const unsigned int wifi_low_png_len;

#include "includes/icons/png/wifi_medium.inc"
extern const unsigned char wifi_medium_png[];
extern const unsigned int wifi_medium_png_len;

#include "includes/icons/png/wifi_high.inc"
extern const unsigned char wifi_high_png[];
extern const unsigned int wifi_high_png_len;

#include "includes/icons/png/wifi_disabled.inc"
extern const unsigned char wifi_disabled_png[];
extern const unsigned int wifi_disabled_png_len;
#endif

#ifdef CHINSCREEN_ICON_DOWNLOAD
#include "includes/icons/png/download.inc"
extern const unsigned char download_png[];
extern const unsigned int download_png_len;
#endif

#ifdef CHINSCREEN_ICON_UPLOAD
#include "includes/icons/png/upload.inc"
extern const unsigned char upload_png[];
extern const unsigned int upload_png_len;
#endif

#ifdef CHINSCREEN_ICON_REFRESH
#include "includes/icons/png/refresh.inc"
extern const unsigned char refresh_png[];
extern const unsigned int refresh_png_len;
#endif

#ifdef CHINSCREEN_ICON_DELETE
#include "includes/icons/png/delete.inc"
extern const unsigned char delete_png[];
extern const unsigned int delete_png_len;
#endif

#ifdef CHINSCREEN_ICON_SAVE
#include "includes/icons/png/save.inc"
extern const unsigned char save_png[];
extern const unsigned int save_png_len;
#endif

#ifdef CHINSCREEN_ICON_EDIT
#include "includes/icons/png/edit.inc"
extern const unsigned char edit_png[];
extern const unsigned int edit_png_len;
#endif

#ifdef CHINSCREEN_ICON_SEARCH
#include "includes/icons/png/search.inc"
extern const unsigned char search_png[];
extern const unsigned int search_png_len;
#endif

// Enhanced icon definitions structure
typedef struct {
    const char* name;
    const unsigned char* data;
    const unsigned int* size;
    bool colorable;  // Whether this icon supports recoloring
} chinScreen_icon_t;

// Icon registry - dynamically built based on enabled icons
static const chinScreen_icon_t chinScreen_icons[] = {
    // Conditionally include PNG icons based on defines
#ifdef CHINSCREEN_ICON_HOME
    {"home", home_png, &home_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_SETTINGS
    {"settings", settings_png, &settings_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_USER
    {"user", user_png, &user_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_MENU
    {"menu", menu_png, &menu_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_BACK
    {"back", back_png, &back_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_FORWARD
    {"forward", forward_png, &forward_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_PLAY
    {"play", play_png, &play_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_PAUSE
    {"pause", pause_png, &pause_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_STOP
    {"stop", stop_png, &stop_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_VOLUME
    {"volume", volume_png, &volume_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_POWER
    {"power", power_png, &power_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_BATTERY
    {"battery", battery_png, &battery_png_len, true},
    {"battery_charging", battery_charging_png, &battery_charging_png_len, true},
    {"battery_low", battery_low_png, &battery_low_png_len, true},
    {"battery_medium", battery_medium_png, &battery_medium_png_len, true},
    {"battery_full", battery_full_png, &battery_full_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_SIGNAL
    {"signal", signal_png, &signal_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_WIFI
    {"wifi", wifi_png, &wifi_png_len, true},
    {"wifi_low", wifi_low_png, &wifi_low_png_len, true},
    {"wifi_medium", wifi_medium_png, &wifi_medium_png_len, true},
    {"wifi_high", wifi_high_png, &wifi_high_png_len, true},
    {"wifi_disabled", wifi_disabled_png, &wifi_disabled_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_DOWNLOAD
    {"download", download_png, &download_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_UPLOAD
    {"upload", upload_png, &upload_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_REFRESH
    {"refresh", refresh_png, &refresh_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_DELETE
    {"delete", delete_png, &delete_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_SAVE
    {"save", save_png, &save_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_EDIT
    {"edit", edit_png, &edit_png_len, true},
#endif

#ifdef CHINSCREEN_ICON_SEARCH
    {"search", search_png, &search_png_len, true},
#endif
};

static const int chinScreen_icon_count = sizeof(chinScreen_icons) / sizeof(chinScreen_icons[0]);

// Function declarations
lv_obj_t* chinScreen_icon(const char* icon_name, 
                         float zoom,
                         const char* vAlign, 
                         const char* hAlign,
                         const char* color,
                         int x, int y);

lv_obj_t* chinScreen_icon_colored(const char* icon_name,
                                 const char* colorName,
                                 float zoom = 1.0f,
                                 const char* vAlign = "middle", 
                                 const char* hAlign = "center");

const chinScreen_icon_t* chinScreen_find_icon(const char* name);
void chinScreen_list_available_icons();
void chinScreen_icon_set_color(lv_obj_t* icon_obj, const char* colorName);

// Icon function implementations
inline const chinScreen_icon_t* chinScreen_find_icon(const char* name) {
    for (int i = 0; i < chinScreen_icon_count; i++) {
        if (strcmp(chinScreen_icons[i].name, name) == 0) {
            return &chinScreen_icons[i];
        }
    }
    return nullptr;
}

inline void chinScreen_list_available_icons() {
    Serial.println("Available icons:");
    for (int i = 0; i < chinScreen_icon_count; i++) {
        Serial.printf("  - %s (%d bytes, %s)\n", 
                     chinScreen_icons[i].name, 
                     *chinScreen_icons[i].size,
                     chinScreen_icons[i].colorable ? "colorable" : "fixed color");
    }
}

inline void chinScreen_icon_set_color(lv_obj_t* icon_obj, const char* colorName) {
    if (!icon_obj || !colorName) return;
    
    bsp_display_lock(0);
    
    lv_color_t color = getColorByName(colorName);
    
    // Use LVGL's built-in recoloring (works best with white/light icons)
    lv_obj_set_style_img_recolor_opa(icon_obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(icon_obj, color, LV_PART_MAIN);
    
    bsp_display_unlock();
}

// Advanced color method for better control
inline void chinScreen_icon_set_color_advanced(lv_obj_t* icon_obj, const char* colorName, uint8_t opacity = 255) {
    if (!icon_obj || !colorName) return;
    
    bsp_display_lock(0);
    
    lv_color_t color = getColorByName(colorName);
    lv_opa_t lv_opacity = (lv_opa_t)opacity;
    
    lv_obj_set_style_img_recolor_opa(icon_obj, lv_opacity, LV_PART_MAIN);
    lv_obj_set_style_img_recolor(icon_obj, color, LV_PART_MAIN);
    
    bsp_display_unlock();
}

inline lv_obj_t* chinScreen_icon(const char* icon_name, 
                                float zoom = 1.0f,
                                const char* vAlign = "middle", 
                                const char* hAlign = "center",
                                const char* color = nullptr,
                                int x = -1, int y = -1) {
    // Find the icon
    const chinScreen_icon_t* icon = chinScreen_find_icon(icon_name);
    if (!icon) {
        Serial.printf("Error: Icon '%s' not found!\n", icon_name);
        chinScreen_list_available_icons();
        return nullptr;
    }
    
    Serial.printf("Loading icon '%s' with zoom %.2fx", icon_name, zoom);
    if (color) {
        Serial.printf(" and color '%s'", color);
    }
    if (x >= 0 && y >= 0) {
        Serial.printf(" at position (%d, %d)", x, y);
    }
    Serial.println();
    
    bsp_display_lock(0);

    // Create unique image descriptor for each icon (max 10 simultaneous icons)
    static lv_img_dsc_t img_dsc_pool[10];
    static int dsc_index = 0;
    
    lv_img_dsc_t* img_dsc = &img_dsc_pool[dsc_index];
    dsc_index = (dsc_index + 1) % 10; // Cycle through pool
    
    memset(img_dsc, 0, sizeof(lv_img_dsc_t));
    img_dsc->data = icon->data;
    img_dsc->data_size = *icon->size;
    img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA; // PNG with alpha channel
    
    // Create PNG image object
    lv_obj_t *img = lv_img_create(lv_scr_act());
    if (img) {
        lv_img_set_src(img, img_dsc);
        
        // Apply zoom if not 1.0
        if (zoom != 1.0f) {
            int16_t zoom_lvgl = (int16_t)(zoom * 256);
            lv_obj_set_style_transform_zoom(img, zoom_lvgl, LV_PART_MAIN);
        }
        
        // Apply color if specified and icon supports it
        if (color && icon->colorable) {
            chinScreen_icon_set_color(img, color);
        } else if (color && !icon->colorable) {
            Serial.printf("Warning: Icon '%s' does not support color changes\n", icon_name);
        }
        
        // Choose positioning method: X/Y coordinates OR named alignment
        if (x >= 0 && y >= 0) {
            // Use X/Y positioning with zoom adjustment
            if (zoom != 1.0f) {
                lv_coord_t width = lv_obj_get_width(img);
                lv_coord_t height = lv_obj_get_height(img);
                lv_coord_t x_offset = (lv_coord_t)((width * (1.0f - zoom)) / 2);
                lv_coord_t y_offset = (lv_coord_t)((height * (1.0f - zoom)) / 2);
                lv_obj_set_pos(img, x + x_offset, y + y_offset);
            } else {
                lv_obj_set_pos(img, x, y);
            }
        } else {
            // Use named alignment with zoom adjustment
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
            
            // Auto-adjust position for zoom if using edge alignments
            if (zoom != 1.0f && (align == LV_ALIGN_TOP_LEFT || align == LV_ALIGN_TOP_RIGHT || 
                                 align == LV_ALIGN_BOTTOM_LEFT || align == LV_ALIGN_BOTTOM_RIGHT)) {
                lv_coord_t current_x = lv_obj_get_x(img);
                lv_coord_t current_y = lv_obj_get_y(img);
                
                // Get original size
                lv_coord_t width = lv_obj_get_width(img);
                lv_coord_t height = lv_obj_get_height(img);
                
                // Calculate offset based on zoom
                lv_coord_t x_offset = (lv_coord_t)((width * (1.0f - zoom)) / 2);
                lv_coord_t y_offset = (lv_coord_t)((height * (1.0f - zoom)) / 2);
                
                // Adjust position to account for visual scaling
                lv_obj_set_pos(img, current_x + x_offset, current_y + y_offset);
            }
        }
    } else {
        Serial.printf("Error: Failed to create image object for icon '%s'\n", icon_name);
    }

    bsp_display_unlock();
    return img;
}

inline lv_obj_t* chinScreen_icon_colored(const char* icon_name,
                                        const char* colorName,
                                        float zoom,
                                        const char* vAlign, 
                                        const char* hAlign) {
    return chinScreen_icon(icon_name, zoom, vAlign, hAlign, colorName, -1, -1);
}


// Convenience functions for common colors
inline lv_obj_t* chinScreen_icon_red(const char* icon_name, float zoom = 1.0f, 
                                     const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "red", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_blue(const char* icon_name, float zoom = 1.0f,
                                      const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "blue", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_green(const char* icon_name, float zoom = 1.0f,
                                       const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "green", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_white(const char* icon_name, float zoom = 1.0f,
                                       const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "white", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_black(const char* icon_name, float zoom = 1.0f,
                                       const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "black", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_yellow(const char* icon_name, float zoom = 1.0f,
                                        const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "yellow", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_orange(const char* icon_name, float zoom = 1.0f,
                                        const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "orange", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_purple(const char* icon_name, float zoom = 1.0f,
                                        const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "purple", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_cyan(const char* icon_name, float zoom = 1.0f,
                                      const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "cyan", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_magenta(const char* icon_name, float zoom = 1.0f,
                                         const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "magenta", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_lime(const char* icon_name, float zoom = 1.0f,
                                      const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "lime", zoom, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_icon_pink(const char* icon_name, float zoom = 1.0f,
                                      const char* vAlign = "middle", const char* hAlign = "center") {
    return chinScreen_icon_colored(icon_name, "pink", zoom, vAlign, hAlign);
}

// Function to change icon color after creation
inline void chinScreen_icon_change_color(lv_obj_t* icon_obj, const char* newColorName) {
    chinScreen_icon_set_color(icon_obj, newColorName);
}

inline lv_obj_t* chinScreen_icon_at(const char* icon_name,
                                    int x, int y,
                                    float zoom = 1.0f,
                                    const char* color = nullptr) {
    return chinScreen_icon(icon_name, zoom, "middle", "center", color, x, y);
}

// Enhanced function: Choose between named alignment OR X/Y coordinates
inline lv_obj_t* chinScreen_icon_positioned(const char* icon_name,
                                            float zoom = 1.0f,
                                            const char* color = nullptr,
                                            // Named positioning (use these OR x/y, not both)
                                            const char* vAlign = nullptr,
                                            const char* hAlign = nullptr,
                                            // Manual positioning (use these OR vAlign/hAlign, not both)
                                            int x = -1, int y = -1) {
    
    if (x >= 0 && y >= 0) {
        // Use manual X/Y positioning
        return chinScreen_icon_at(icon_name, x, y, zoom, color);
    } else {
        // Use named alignment positioning
        const char* v = vAlign ? vAlign : "middle";
        const char* h = hAlign ? hAlign : "center";
        return chinScreen_icon(icon_name, zoom, v, h, color);
    }
}

// Grid positioning helper
inline lv_obj_t* chinScreen_icon_grid(const char* icon_name,
                                      int grid_x, int grid_y,  // Grid position (0,0 = top-left)
                                      int grid_cols = 3, int grid_rows = 2,  // Grid size
                                      int start_x = 80, int start_y = 80,    // Grid start position
                                      int spacing_x = 120, int spacing_y = 80, // Grid spacing
                                      float zoom = 1.0f,
                                      const char* color = nullptr) {
    
    int pixel_x = start_x + (grid_x * spacing_x);
    int pixel_y = start_y + (grid_y * spacing_y);
    
    return chinScreen_icon_at(icon_name, pixel_x, pixel_y, zoom, color);
}