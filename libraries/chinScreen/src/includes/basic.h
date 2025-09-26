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
//   File: basic.h
//   Purpose: inital basic functions for library
//   Required: YES

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
	
	if (strcmp(size, "small-pixel") == 0) return FONT_PIXEL_S;
	if (strcmp(size, "big-pixel") == 0) return FONT_PIXEL_B;
	
	#ifdef CHINSCREEN_ENABLE_CURSIVE
	if (strcmp(size, "cursive") == 0) return FONT_CURSIVE;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_COMIC_SMALL
	if (strcmp(size, "comic_small") == 0) return FONT_COMIC_SMALL;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_COMIC_REGULAR
	if (strcmp(size, "comic_regular") == 0) return FONT_COMIC_REGULAR;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_BRUSH_REGULAR
	if (strcmp(size, "brush_regular") == 0) return FONT_BRUSH_REGULAR;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_PEN_REGULAR
	if (strcmp(size, "pen_regular") == 0) return FONT_PEN_REGULAR;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_ICEBERG
	if (strcmp(size, "iceberg") == 0) return FONT_ICEBERG;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_TYPEWRITER
	if (strcmp(size, "typewriter") == 0) return FONT_TYPEWRITER;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_DOTS
	if (strcmp(size, "dots") == 0) return FONT_DOTS;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_BEAST
	if (strcmp(size, "beast") == 0) return FONT_BEAST;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_MODERN
	if (strcmp(size, "MODERN") == 0) return FONT_MODERN;
	#endif

	#ifdef CHINSCREEN_ENABLE_CREEPY
	if (strcmp(size, "creepy") == 0) return FONT_CREEPY;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_ALADIN
	if (strcmp(size, "aladin") == 0) return FONT_ALADIN;
	#endif
	
	#ifdef CHINSCREEN_ENABLE_CYOA_SMALL
	if (strcmp(size, "cyoa-small") == 0) return FONT_CYOA_SMALL;
	#endif
	#ifdef CHINSCREEN_ENABLE_CYOA_REGULAR
	if (strcmp(size, "cyoa-regular") == 0) return FONT_CYOA_REGULAR;
	#endif
	#ifdef CHINSCREEN_ENABLE_CYOA_LARGE
	if (strcmp(size, "cyoa-large") == 0) return FONT_CYOA_LARGE;
	#endif
			
	
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