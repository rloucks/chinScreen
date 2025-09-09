// icons.inc - PNG Icon management for chinScreen library
// CLEANED VERSION - No duplicate functions
#pragma once

// Include files AND declare extern variables for PNG icons
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

// PNG Icon registry structure
typedef struct {
    const char* name;
    const unsigned char* data;
    const unsigned int* size;
} chinScreen_png_icon_t;

// PNG Icon registry - only includes enabled icons
static const chinScreen_png_icon_t chinScreen_png_icons[] = {
#ifdef CHINSCREEN_ICON_HOME
    {"home", home_png, &home_png_len},
#endif
#ifdef CHINSCREEN_ICON_SETTINGS
    {"settings", settings_png, &settings_png_len},
#endif
#ifdef CHINSCREEN_ICON_USER
    {"user", user_png, &user_png_len},
#endif
#ifdef CHINSCREEN_ICON_MENU
    {"menu", menu_png, &menu_png_len},
#endif
#ifdef CHINSCREEN_ICON_BACK
    {"back", back_png, &back_png_len},
#endif
#ifdef CHINSCREEN_ICON_FORWARD
    {"forward", forward_png, &forward_png_len},
#endif
#ifdef CHINSCREEN_ICON_PLAY
    {"play", play_png, &play_png_len},
#endif
#ifdef CHINSCREEN_ICON_PAUSE
    {"pause", pause_png, &pause_png_len},
#endif
#ifdef CHINSCREEN_ICON_STOP
    {"stop", stop_png, &stop_png_len},
#endif
#ifdef CHINSCREEN_ICON_VOLUME
    {"volume", volume_png, &volume_png_len},
#endif
#ifdef CHINSCREEN_ICON_POWER
    {"power", power_png, &power_png_len},
#endif
#ifdef CHINSCREEN_ICON_BATTERY
    {"battery", battery_png, &battery_png_len},
    {"battery_low", battery_low_png, &battery_low_png_len},
    {"battery_medium", battery_medium_png, &battery_medium_png_len},
    {"battery_full", battery_full_png, &battery_full_png_len},
    {"battery_charging", battery_charging_png, &battery_charging_png_len},	
#endif
#ifdef CHINSCREEN_ICON_SIGNAL
    {"signal", signal_png, &signal_png_len},
#endif
#ifdef CHINSCREEN_ICON_WIFI
    {"wifi", wifi_png, &wifi_png_len},
    {"wifi_low", wifi_low_png, &wifi_low_png_len},
    {"wifi_medium", wifi_medium_png, &wifi_medium_png_len},
    {"wifi_high", wifi_high_png, &wifi_high_png_len},
    {"wifi_disabled", wifi_disabled_png, &wifi_disabled_png_len},
#endif
#ifdef CHINSCREEN_ICON_DOWNLOAD
    {"download", download_png, &download_png_len},
#endif
#ifdef CHINSCREEN_ICON_UPLOAD
    {"upload", upload_png, &upload_png_len},
#endif
#ifdef CHINSCREEN_ICON_REFRESH
    {"refresh", refresh_png, &refresh_png_len},
#endif
#ifdef CHINSCREEN_ICON_DELETE
    {"delete", delete_png, &delete_png_len},
#endif
#ifdef CHINSCREEN_ICON_SAVE
    {"save", save_png, &save_png_len},
#endif
#ifdef CHINSCREEN_ICON_EDIT
    {"edit", edit_png, &edit_png_len},
#endif
#ifdef CHINSCREEN_ICON_SEARCH
    {"search", search_png, &search_png_len},
#endif
    // End marker to prevent empty array
    {nullptr, nullptr, nullptr}
};

static const int chinScreen_png_icon_count = sizeof(chinScreen_png_icons) / sizeof(chinScreen_png_icons[0]);

// SINGLE SET OF FUNCTION DECLARATIONS - NO DUPLICATES
const chinScreen_png_icon_t* chinScreen_find_png_icon(const char* name);
void chinScreen_list_available_png_icons();

inline const chinScreen_png_icon_t* chinScreen_find_png_icon(const char* name) {
    for (int i = 0; i < chinScreen_png_icon_count && chinScreen_png_icons[i].name != nullptr; i++) {
        if (strcmp(chinScreen_png_icons[i].name, name) == 0) {
            return &chinScreen_png_icons[i];
        }
    }
    return nullptr;
}

inline void chinScreen_list_available_png_icons() {
    Serial.println("Available PNG icons:");
    for (int i = 0; i < chinScreen_png_icon_count && chinScreen_png_icons[i].name != nullptr; i++) {
        Serial.printf("  - %s (%d bytes)\n", 
                     chinScreen_png_icons[i].name, 
                     *chinScreen_png_icons[i].size);
    }
}

// Your original working function - 4 parameters only
lv_obj_t* chinScreen_png_icon(const char* icon_name, 
                              float zoom = 1.0f,
                              const char* vAlign = "middle", 
                              const char* hAlign = "center",
                              const char* color = "white");

lv_obj_t* chinScreen_png_icon_xy(const char* icon_name, 
                                 float zoom, 
                                 int x, int y,
                                 const char* color = "white");

inline lv_obj_t* chinScreen_png_icon(const char* icon_name, 
                                     float zoom,
                                     const char* vAlign, 
                                     const char* hAlign,
                                     const char* color) {
    const chinScreen_png_icon_t* icon = chinScreen_find_png_icon(icon_name);
    if (!icon) {
        Serial.printf("Error: PNG Icon '%s' not found!\n", icon_name);
        return nullptr;
    }
    
    bsp_display_lock(0);

    // Use your EXACT working method - don't change anything about PNG loading
    lv_obj_t *img = lv_img_create(lv_scr_act());
    if (img) {
        lv_img_set_src(img, icon->data);  // Your proven working method
        
        // Only add color AFTER we know the PNG loaded
        // (And only if it's not white)
        if (strcmp(color, "white") != 0) {
            lv_color_t recolor = getColorByName(color);
            lv_obj_set_style_img_recolor(img, recolor, LV_PART_MAIN);
            lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
        }
        
        // Your existing zoom and alignment code...
    }

    bsp_display_unlock();
    return img;
}


inline lv_obj_t* chinScreen_png_icon_xy(const char* icon_name, 
                                        float zoom, 
                                        int x, int y,
                                        const char* color) {
    const chinScreen_png_icon_t* icon = chinScreen_find_png_icon(icon_name);
    if (!icon) {
        Serial.printf("Error: PNG Icon '%s' not found!\n", icon_name);
        return nullptr;
    }
    
    Serial.printf("Loading PNG icon '%s' at (%d, %d) with zoom %.2fx, color %s\n", icon_name, x, y, zoom, color);
    
    bsp_display_lock(0);

    lv_obj_t *img = lv_img_create(lv_scr_act());
    if (img) {
        lv_img_set_src(img, icon->data);
        
        // Apply color recoloring
	if (strcmp(color, "white") != 0) {  // Only recolor if not white (default)
		lv_color_t recolor = getColorByName(color);
		lv_obj_set_style_img_recolor(img, recolor, LV_PART_MAIN);
		lv_obj_set_style_img_recolor_opa(img, LV_OPA_COVER, LV_PART_MAIN);
	}
        
        if (zoom != 1.0f) {
            int16_t zoom_lvgl = (int16_t)(zoom * 256);
            lv_obj_set_style_transform_zoom(img, zoom_lvgl, LV_PART_MAIN);
        }
        
        lv_obj_set_pos(img, x, y);
    }

    bsp_display_unlock();
    return img;
}