/////////////////////////////////////////////////////////////
// animation callback for continuous effects
/////////////////////////////////////////////////////////////
static void anim_ready_cb(lv_anim_t* a) {
    // Animation finished callback
    Serial.println("Animation completed");
}


/////////////////////////////////////////////////////////////
// Function: Advanced animation with easing
/////////////////////////////////////////////////////////////
inline void chinScreen_animate_advanced(lv_obj_t* obj, chinScreen_anim_type_t anim_type,
                                       int32_t start_value, int32_t end_value,
                                       uint32_t duration = 1000, bool repeat = false,
                                       const char* easing = "ease_out") {
    if (!obj) return;

    bsp_display_lock(0);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_values(&anim, start_value, end_value);

    // Set easing path
    if (strcmp(easing, "ease_in") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in);
    } else if (strcmp(easing, "ease_out") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    } else if (strcmp(easing, "ease_in_out") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);
    } else if (strcmp(easing, "bounce") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_bounce);
    } else {
        lv_anim_set_path_cb(&anim, lv_anim_path_linear);
    }

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
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_angle);
            start_value *= 10; // LVGL uses 0.1 degree units
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

    lv_anim_set_ready_cb(&anim, anim_ready_cb);
    lv_anim_start(&anim);

    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Function: Get current animation state
/////////////////////////////////////////////////////////////
inline bool chinScreen_is_animating(lv_obj_t* obj) {
    if (!obj) return false;
    return lv_anim_count_running() > 0;
}

/////////////////////////////////////////////////////////////
// Function: Stop all animations on an object
/////////////////////////////////////////////////////////////
inline void chinScreen_stop_animations(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_anim_del(obj, NULL);
    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Function: Create progress bar
/////////////////////////////////////////////////////////////
inline lv_obj_t* chinScreen_progress_bar(int width, int height, int value,
                                        const char* bgColorName = "gray", 
                                        const char* fgColorName = "green",
                                        const char* vAlign = "middle", 
                                        const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *bar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar, width, height);
    lv_bar_set_value(bar, value, LV_ANIM_OFF);

    // Styling
    lv_obj_set_style_bg_color(bar, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, getColorByName(fgColorName), LV_PART_INDICATOR);

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

    lv_obj_align(bar, align, 0, 0);

    bsp_display_unlock();
    return bar;
}

