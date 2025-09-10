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
//   File: callbacks.h
//   Purpose: inital callbacks for library
//   Required: YES



/////////////////////////////////////////////////////////////
// Get text from keyboard
/////////////////////////////////////////////////////////////
inline const char* chinScreen_keyboard_get_text() {
    if (keyboard_textarea) {
        return lv_textarea_get_text(keyboard_textarea);
    }
    return "";
}

/////////////////////////////////////////////////////////////
// Get number from numpad
/////////////////////////////////////////////////////////////
inline const char* chinScreen_numpad_get_result() {
    return numpad_result;
}

inline int chinScreen_numpad_get_int() {
    return atoi(numpad_result);
}

inline float chinScreen_numpad_get_float() {
    return atof(numpad_result);
}

/////////////////////////////////////////////////////////////
// Function: Update progress bar value
/////////////////////////////////////////////////////////////
inline void chinScreen_progress_set_value(lv_obj_t* bar, int value) {
    if (!bar) return;
    bsp_display_lock(0);
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    bsp_display_unlock();
}

