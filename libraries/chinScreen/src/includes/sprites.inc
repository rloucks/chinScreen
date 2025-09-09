// sprites.h - sprite management for chinScreen library
#pragma once

// Include files AND declare extern variables
#ifdef CHINSCREEN_SPRITE_MAN
#include "includes/sprites/m_f_1.inc"
#include "includes/sprites/m_f_2.inc"
#include "includes/sprites/m_f_3.inc"
extern const unsigned char m_f_1_gif[];
extern const unsigned int m_f_1_gif_len;
extern const unsigned char m_f_2_gif[];
extern const unsigned int m_f_2_gif_len;
extern const unsigned char m_f_3_gif[];
extern const unsigned int m_f_3_gif_len;
#endif

typedef struct {
    const char* name;
    const unsigned char* data;
    const unsigned int* size;
} chinScreen_sprite_t;

// Fixed sprite registry
static const chinScreen_sprite_t chinScreen_sprites[] = {
#ifdef CHINSCREEN_SPRITE_MAN
    {"man_walk_forward-1", m_f_1_gif, &m_f_1_gif_len},
    {"man_walk_forward-2", m_f_2_gif, &m_f_2_gif_len},
    {"man_forward", m_f_3_gif, &m_f_3_gif_len},
#endif
    {nullptr, nullptr, nullptr}
};

static const int chinScreen_sprite_count = sizeof(chinScreen_sprites) / sizeof(chinScreen_sprites[0]) - 1;

// Function declarations
lv_obj_t* chinScreen_sprite(const char* sprite_name, 
                         float zoom = 1.0f,
                         const char* vAlign = "middle", 
                         const char* hAlign = "center");

const chinScreen_sprite_t* chinScreen_find_sprite(const char* name);
void chinScreen_list_available_sprites();
bool chinScreen_sprite_change(lv_obj_t* sprite_obj, const char* new_sprite_name);

// Implementation
inline const chinScreen_sprite_t* chinScreen_find_sprite(const char* name) {
    for (int i = 0; i < chinScreen_sprite_count; i++) {
        if (chinScreen_sprites[i].name && strcmp(chinScreen_sprites[i].name, name) == 0) {
            return &chinScreen_sprites[i];
        }
    }
    return nullptr;
}

inline void chinScreen_list_available_sprites() {
    Serial.println("Available sprites:");
    for (int i = 0; i < chinScreen_sprite_count; i++) {
        if (chinScreen_sprites[i].name) {
            Serial.printf("  - %s (%d bytes)\n", 
                         chinScreen_sprites[i].name, 
                         *chinScreen_sprites[i].size);
        }
    }
}

inline lv_obj_t* chinScreen_sprite(const char* sprite_name, 
                                float zoom,
                                const char* vAlign, 
                                const char* hAlign) {
    const chinScreen_sprite_t* sprite = chinScreen_find_sprite(sprite_name);
    if (!sprite) {
        Serial.printf("Error: Sprite '%s' not found!\n", sprite_name);
        chinScreen_list_available_sprites();
        return nullptr;
    }
    
    Serial.printf("Loading sprite '%s' with zoom %.2fx\n", sprite_name, zoom);
    
    bsp_display_lock(0);

    static lv_img_dsc_t img_dsc;
    memset(&img_dsc, 0, sizeof(lv_img_dsc_t));
    img_dsc.data = sprite->data;
    img_dsc.data_size = *sprite->size;

    lv_obj_t *img = lv_gif_create(lv_scr_act());
    if (img) {
        lv_gif_set_src(img, &img_dsc);
        
        if (zoom != 1.0f) {
            int16_t zoom_lvgl = (int16_t)(zoom * 256);
            lv_obj_set_style_transform_zoom(img, zoom_lvgl, LV_PART_MAIN);
        }
        
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
        Serial.printf("Error: Failed to create image object for sprite '%s'\n", sprite_name);
    }

    bsp_display_unlock();
    return img;
}

inline bool chinScreen_sprite_change(lv_obj_t* sprite_obj, const char* new_sprite_name) {
    if (!sprite_obj) return false;
    
    const chinScreen_sprite_t* new_sprite = chinScreen_find_sprite(new_sprite_name);
    if (!new_sprite) {
        Serial.printf("Error: Sprite '%s' not found!\n", new_sprite_name);
        return false;
    }
    
    bsp_display_lock(0);
    
    static lv_img_dsc_t new_dsc;
    memset(&new_dsc, 0, sizeof(lv_img_dsc_t));
    new_dsc.data = new_sprite->data;
    new_dsc.data_size = *new_sprite->size;
    
    lv_gif_set_src(sprite_obj, &new_dsc);
    
    bsp_display_unlock();
    return true;
}