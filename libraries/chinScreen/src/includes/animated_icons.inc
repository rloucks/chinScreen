// icons.h - Icon management for chinScreen library
// animated_icons.h - Fixed version
#pragma once

// Include files AND declare extern variables
#ifdef CHINSCREEN_ICON_AVATAR
#include "includes/icons/ani/avatar-small.inc"
extern const unsigned char avatar_small_gif[];
extern const unsigned int avatar_small_gif_len;
#endif

#ifdef CHINSCREEN_ICON_BOOK
#include "includes/icons/ani/book.inc"
extern const unsigned char book_gif[];
extern const unsigned int book_gif_len;
#endif

#ifdef CHINSCREEN_ICON_CLOSED
#include "includes/icons/ani/closed-small.inc"
extern const unsigned char closed_small_gif[];
extern const unsigned int closed_small_len;
#endif

#ifdef CHINSCREEN_ICON_GEAR
#include "includes/icons/ani/gear-1.inc"
#include "includes/icons/ani/gear-2.inc"
#include "includes/icons/ani/gear-3.inc"
extern const unsigned char gear_1_gif[];
extern const unsigned int gear_1_gif_len;
extern const unsigned char gear_2_gif[];
extern const unsigned int gear_2_gif_len;
extern const unsigned char gear_3_gif[];
extern const unsigned int gear_3_gif_len;

#endif

#ifdef CHINSCREEN_ICON_LOADING
#include "includes/icons/ani/loading-2.inc"
#include "includes/icons/ani/loading-3.inc"
#include "includes/icons/ani/loading-5.inc"
extern const unsigned char loading_2_gif[];
extern const unsigned int loading_2_gif_len;
extern const unsigned char loading_3_gif[];
extern const unsigned int loading_3_gif_len;
extern const unsigned char loading_5_gif[];
extern const unsigned int loading_5_gif_len;
// Add more loading extern declarations as needed
#endif

#ifdef CHINSCREEN_ICON_OPEN
#include "includes/icons/ani/open-small.inc"
extern const unsigned char open_small_gif[];
extern const unsigned int open_small_gif_len;
#endif

#ifdef CHINSCREEN_ICON_TOUCH
#include "includes/icons/ani/touch.inc"
extern const unsigned char touch_gif[];
extern const unsigned int touch_gif_len;
#endif

#ifdef CHINSCREEN_ICON_WIFI
#include "includes/icons/ani/wifi-1.inc"
extern const unsigned char wifi_1_gif[];
extern const unsigned int wifi_1_gif_len;
#endif

typedef struct {
    const char* name;
    const unsigned char* data;
    const unsigned int* size;
} chinScreen_icon_t;

// Fixed icon registry
static const chinScreen_icon_t chinScreen_icons[] = {
#ifdef CHINSCREEN_ICON_AVATAR
    {"avatar-small", avatar_small_gif, &avatar_small_gif_len},
#endif
#ifdef CHINSCREEN_ICON_BOOK
    {"book", book_gif, &book_gif_len},
#endif
#ifdef CHINSCREEN_ICON_CLOSED
    {"closed-small", closed_small_gif, &closed_small_gif_len},
#endif
#ifdef CHINSCREEN_ICON_GEAR
    {"gear-1", gear_1_gif, &gear_1_gif_len},
    {"gear-2", gear_2_gif, &gear_2_gif_len},
    {"gear-3", gear_3_gif, &gear_3_gif_len},
#endif
#ifdef CHINSCREEN_ICON_LOADING
    {"loading-2", loading_2_gif, &loading_2_gif_len},
    {"loading-3", loading_3_gif, &loading_3_gif_len},
    {"loading-5", loading_5_gif, &loading_5_gif_len},
#endif
#ifdef CHINSCREEN_ICON_OPEN
    {"open-small", open_small_gif, &open_small_gif_len},
#endif
#ifdef CHINSCREEN_ICON_TOUCH
    {"touch", touch_gif, &touch_gif_len},
#endif
#ifdef CHINSCREEN_ICON_WIFI
    {"wifi-1", wifi_1_gif, &wifi_1_gif_len},
#endif
    // End marker to prevent empty array
    {nullptr, nullptr, nullptr}
};



static const int chinScreen_icon_count = sizeof(chinScreen_icons) / sizeof(chinScreen_icons[0]);

// Function declarations
lv_obj_t* chinScreen_icon(const char* icon_name, 
                         float zoom = 1.0f,
                         const char* vAlign = "middle", 
                         const char* hAlign = "center");

const chinScreen_icon_t* chinScreen_find_icon(const char* name);
void chinScreen_list_available_icons();

// Icon function implementation
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
        Serial.printf("  - %s (%d bytes)\n", 
                     chinScreen_icons[i].name, 
                     *chinScreen_icons[i].size);
    }
}

inline lv_obj_t* chinScreen_icon(const char* icon_name, 
                                float zoom,
                                const char* vAlign, 
                                const char* hAlign) {
    // Find the icon
    const chinScreen_icon_t* icon = chinScreen_find_icon(icon_name);
    if (!icon) {
        Serial.printf("Error: Icon '%s' not found!\n", icon_name);
        chinScreen_list_available_icons();
        return nullptr;
    }
    
    Serial.printf("Loading icon '%s' with zoom %.2fx\n", icon_name, zoom);
    
    bsp_display_lock(0);

    // Create image descriptor
    static lv_img_dsc_t img_dsc;
    memset(&img_dsc, 0, sizeof(lv_img_dsc_t));
    img_dsc.data = icon->data;
    img_dsc.data_size = *icon->size;

    // Create the image object (assuming these are GIFs, adjust if needed)
    lv_obj_t *img = lv_gif_create(lv_scr_act());
    if (img) {
        lv_gif_set_src(img, &img_dsc);
        
        // Apply zoom if not 1.0
        if (zoom != 1.0f) {
            int16_t zoom_lvgl = (int16_t)(zoom * 256);
            lv_obj_set_style_transform_zoom(img, zoom_lvgl, LV_PART_MAIN);
        }
        
        // Handle alignment
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
    } else {
        Serial.printf("Error: Failed to create image object for icon '%s'\n", icon_name);
    }

    bsp_display_unlock();
    return img;
}