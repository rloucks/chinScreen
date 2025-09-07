/////////////////////////////////////////////////////////////
// Function: Clear Screen
/////////////////////////////////////////////////////////////
inline void chinScreen_clear() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_invalidate(scr);
    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Function: Set solid background
/////////////////////////////////////////////////////////////
inline void chinScreen_background_solid(const char* colorName) {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    bsp_display_unlock();
}


/////////////////////////////////////////////////////////////
// Function: Turn the backlight on and off
/////////////////////////////////////////////////////////////
inline void chinScreen_backlight(int bLight) {   
    if (bLight == 0) {
        bsp_display_backlight_off(); 
    } else {
        bsp_display_backlight_on();
    }
}

/////////////////////////////////////////////////////////////
// Function: Set brightness
/////////////////////////////////////////////////////////////
inline void chinScreen_brightness(int howBright) {   
    bsp_display_brightness_set(howBright);
}


/////////////////////////////////////////////////////////////
// Function: add text
/////////////////////////////////////////////////////////////
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

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    bsp_display_unlock();
}


/////////////////////////////////////////////////////////////
// Function: Set object position (useful for manual positioning)
/////////////////////////////////////////////////////////////
inline void chinScreen_set_position(lv_obj_t* obj, int x, int y) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_set_pos(obj, x, y);
    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Function: Delete an object
/////////////////////////////////////////////////////////////
inline void chinScreen_delete_object(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_del(obj);
    bsp_display_unlock();
}