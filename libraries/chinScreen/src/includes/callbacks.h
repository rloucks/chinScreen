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

