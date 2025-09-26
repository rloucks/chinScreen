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
//   File: inputs.inc
//   Purpose: library for inputs, buttons, and so on
//   Required: NO

/////////////////////////////////////////////////////////////
// Button callback type
/////////////////////////////////////////////////////////////
typedef void (*chinScreen_btn_callback_t)(lv_event_t* e);

// Initialize the display (panel, rotation, backlight)
inline void init_display() {
    Serial.println("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    Serial.println("Display init done");
}


/////////////////////////////////////////////////////////////
// Virtual Keyboard
/////////////////////////////////////////////////////////////
static lv_obj_t* keyboard_obj = nullptr;
static lv_obj_t* keyboard_textarea = nullptr;

static void keyboard_event_cb(lv_event_t* e) {
    lv_obj_t* kb = lv_event_get_target(e);
    lv_obj_t* ta = (lv_obj_t*)lv_event_get_user_data(e);
    
    if (lv_event_get_code(e) == LV_EVENT_READY || lv_event_get_code(e) == LV_EVENT_CANCEL) {
        // Keyboard closed
        if (keyboard_obj) {
            lv_obj_del(keyboard_obj);
            keyboard_obj = nullptr;
        }
    }
}

inline lv_obj_t* chinScreen_keyboard(const char* placeholder = "Type here...",
                                     const char* vAlign = "bottom",
                                     const char* hAlign = "center") {
    bsp_display_lock(0);

    // Create text area first
    keyboard_textarea = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(keyboard_textarea, 300, 40);
    lv_textarea_set_placeholder_text(keyboard_textarea, placeholder);
    lv_obj_align(keyboard_textarea, LV_ALIGN_TOP_MID, 0, 20);

    // Create keyboard
    keyboard_obj = lv_keyboard_create(lv_scr_act());
    lv_keyboard_set_textarea(keyboard_obj, keyboard_textarea);
    
    // Position keyboard
    lv_align_t align = LV_ALIGN_BOTTOM_MID;
    if (strcmp(vAlign, "top") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(vAlign, "middle") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(vAlign, "bottom") == 0) align = LV_ALIGN_BOTTOM_MID;
    
    lv_obj_align(keyboard_obj, align, 0, 0);
    
    // Add event callback
    lv_obj_add_event_cb(keyboard_obj, keyboard_event_cb, LV_EVENT_ALL, keyboard_textarea);

    bsp_display_unlock();
    return keyboard_obj;
}



/////////////////////////////////////////////////////////////
// Close keyboard
/////////////////////////////////////////////////////////////
inline void chinScreen_keyboard_close() {
    if (keyboard_obj) {
        bsp_display_lock(0);
        lv_obj_del(keyboard_obj);
        keyboard_obj = nullptr;
        if (keyboard_textarea) {
            lv_obj_del(keyboard_textarea);
            keyboard_textarea = nullptr;
        }
        bsp_display_unlock();
    }
}

/////////////////////////////////////////////////////////////
// Number pad
/////////////////////////////////////////////////////////////
static lv_obj_t* numpad_obj = nullptr;
static lv_obj_t* numpad_textarea = nullptr;
static char numpad_result[32] = {0};

static void numpad_event_cb(lv_event_t* e) {
    lv_obj_t* btn = lv_event_get_target(e);
    const char* txt = lv_btnmatrix_get_btn_text(numpad_obj, lv_btnmatrix_get_selected_btn(numpad_obj));
    
    if (strcmp(txt, "Clear") == 0) {
        lv_textarea_set_text(numpad_textarea, "");
        strcpy(numpad_result, "");
    } else if (strcmp(txt, "Enter") == 0) {
        strcpy(numpad_result, lv_textarea_get_text(numpad_textarea));
        // Close numpad
        if (numpad_obj) {
            bsp_display_lock(0);
            lv_obj_del(numpad_obj);
            numpad_obj = nullptr;
            if (numpad_textarea) {
                lv_obj_del(numpad_textarea);
                numpad_textarea = nullptr;
            }
            bsp_display_unlock();
        }
    } else if (strcmp(txt, "Del") == 0) {
        lv_textarea_del_char(numpad_textarea);
    } else {
        lv_textarea_add_text(numpad_textarea, txt);
    }
}

inline lv_obj_t* chinScreen_numpad(const char* placeholder = "Enter number...",
                                   const char* vAlign = "center",
                                   const char* hAlign = "center") {
    bsp_display_lock(0);

    // Create text area
    numpad_textarea = lv_textarea_create(lv_scr_act());
    lv_obj_set_size(numpad_textarea, 200, 40);
    lv_textarea_set_placeholder_text(numpad_textarea, placeholder);
    lv_textarea_set_accepted_chars(numpad_textarea, "0123456789.-");
    lv_obj_align(numpad_textarea, LV_ALIGN_CENTER, 0, -80);

    // Number pad button map
    static const char* numpad_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n", 
        "7", "8", "9", "\n",
        "Clear", "0", "Del", "\n",
        "Enter", ""
    };

    // Create button matrix for numpad
    numpad_obj = lv_btnmatrix_create(lv_scr_act());
    lv_btnmatrix_set_map(numpad_obj, numpad_map);
    lv_obj_set_size(numpad_obj, 200, 200);
    
    // Position numpad
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
    
    lv_obj_align(numpad_obj, align, 0, 20);
    
    // Add event callback
    lv_obj_add_event_cb(numpad_obj, numpad_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    bsp_display_unlock();
    return numpad_obj;
}

/////////////////////////////////////////////////////////////
// Image Button
/////////////////////////////////////////////////////////////
inline lv_obj_t* chinScreen_image_button(const char* imagePath, 
                                         chinScreen_btn_callback_t callback,
                                         const char* vAlign = "middle", 
                                         const char* hAlign = "center",
                                         int width = -1, int height = -1) {
    bsp_display_lock(0);

    lv_obj_t* imgBtn = lv_imgbtn_create(lv_scr_act());
    
    // Load image from SD card
    if (SD.exists(imagePath)) {
        File imageFile = SD.open(imagePath, FILE_READ);
        if (imageFile) {
            size_t fileSize = imageFile.size();
            uint8_t* imageData = (uint8_t*)malloc(fileSize);
            
            if (imageData) {
                imageFile.read(imageData, fileSize);
                imageFile.close();
                
                // Create image descriptor
                static lv_img_dsc_t img_dsc;
                img_dsc.data = imageData;
                img_dsc.data_size = fileSize;
                img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
                
                // Set image for different button states
                lv_imgbtn_set_src(imgBtn, LV_IMGBTN_STATE_RELEASED, NULL, &img_dsc, NULL);
                lv_imgbtn_set_src(imgBtn, LV_IMGBTN_STATE_PRESSED, NULL, &img_dsc, NULL);
            }
        }
    } else {
        Serial.println("Image button file not found");
        lv_obj_del(imgBtn);
        bsp_display_unlock();
        return nullptr;
    }
    
    // Set size if specified
    if (width > 0 && height > 0) {
        lv_obj_set_size(imgBtn, width, height);
    }
    
    // Add callback if provided
    if (callback) {
        lv_obj_add_event_cb(imgBtn, callback, LV_EVENT_CLICKED, NULL);
    }
    
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

    lv_obj_align(imgBtn, align, 0, 0);

    bsp_display_unlock();
    return imgBtn;
}

/////////////////////////////////////////////////////////////
// Fucntion: Create slider
/////////////////////////////////////////////////////////////
inline lv_obj_t* chinScreen_slider(int width, int height, int min_val, int max_val, int initial_val,
                                   const char* bgColorName = "gray", const char* knobColorName = "blue",
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, width, height);
    lv_slider_set_range(slider, min_val, max_val);
    lv_slider_set_value(slider, initial_val, LV_ANIM_OFF);

    // Styling
    lv_obj_set_style_bg_color(slider, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, getColorByName(knobColorName), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, getColorByName(knobColorName), LV_PART_INDICATOR);

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

    lv_obj_align(slider, align, 0, 0);

    bsp_display_unlock();
    return slider;
}

/////////////////////////////////////////////////////////////
// Function: Create toggle switch
/////////////////////////////////////////////////////////////
inline lv_obj_t* chinScreen_toggle(bool initial_state = false,
                                   const char* bgColorName = "gray", const char* knobColorName = "white",
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *sw = lv_switch_create(lv_scr_act());
    
    if (initial_state) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }

    // Styling
    lv_obj_set_style_bg_color(sw, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(sw, getColorByName(knobColorName), LV_PART_KNOB);

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

    lv_obj_align(sw, align, 0, 0);

    bsp_display_unlock();
    return sw;
}
/////////////////////////////////////////////////////////////
// Function Get slider value
/////////////////////////////////////////////////////////////
inline int chinScreen_slider_get_value(lv_obj_t* slider) {
    if (!slider) return 0;
    return lv_slider_get_value(slider);
}

/////////////////////////////////////////////////////////////
// Function: Get toggle state
/////////////////////////////////////////////////////////////
inline bool chinScreen_toggle_get_state(lv_obj_t* toggle) {
    if (!toggle) return false;
    return lv_obj_has_state(toggle, LV_STATE_CHECKED);
}