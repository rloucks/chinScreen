// icons.h - Icon management for chinScreen library
#pragma once

// Include all your icon files
#include "includes/icons/ani/avatar-small.c"

// Add more as needed

// Icon definitions structure
typedef struct {
    const char* name;
    const unsigned char* data;
    const unsigned int* size;
} chinScreen_icon_t;

// Icon registry - add your icons here
static const chinScreen_icon_t chinScreen_icons[] = {
{"avatar_small", avatar_small_gif, &avatar_small_gif_len},
    // Add more icons here as you create them
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