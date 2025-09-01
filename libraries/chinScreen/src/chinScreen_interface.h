//       _     _       __                        
//   ___| |__ (_)_ __ / _\ ___ _ __ ___  ___ _ __   
//  / __| '_ \| | '_ \\ \ / __| '__/ _ \/ _ \ '_ \ 
// | (__| | | | | | | |\ \ (__| | |  __/  __/ | | |
//  \___|_| |_|_|_| |_\__/\___|_|  \___|\___|_| |_| 
//
//   Modular library for the JC3248W525EN using LVGL 9.3.0
//   Improved architecture with optional module loading
// 
//   Author: Richard Loucks
//   Version: 0.1.1
//   Updated: Sept 2025 - Modular Architecture
//   For updates, see git commits
//   https://github.com/rloucks/chinScreen
//=============================================================================
// chinScreen Interface Module
// Buttons, sliders, switches, keyboards, and UI controls using LVGL widgets
// Compatible with LVGL 9.3.0 - Leverages built-in LVGL UI resources
//=============================================================================

#pragma once

// Module guard - only load if interface hasn't been loaded
#ifndef CHINSCREEN_INTERFACE_H
#define CHINSCREEN_INTERFACE_H

//=============================================================================
// INTERFACE STYLE SYSTEM
//=============================================================================

// UI theme configuration
typedef struct {
    const char* primary_color;
    const char* secondary_color; 
    const char* accent_color;
    const char* text_color;
    const char* background_color;
    int border_width;
    int corner_radius;
    const lv_font_t* font;
} chinScreen_ui_theme_t;

// Default themes
static const chinScreen_ui_theme_t chinScreen_theme_default = {
    .primary_color = "blue",
    .secondary_color = "gray", 
    .accent_color = "white",
    .text_color = "white",
    .background_color = "gray",
    .border_width = 2,
    .corner_radius = 8,
    .font = FONT_MEDIUM
};

static const chinScreen_ui_theme_t chinScreen_theme_dark = {
    .primary_color = "purple",
    .secondary_color = "black",
    .accent_color = "cyan",
    .text_color = "white", 
    .background_color = "black",
    .border_width = 1,
    .corner_radius = 12,
    .font = FONT_MEDIUM
};

static const chinScreen_ui_theme_t chinScreen_theme_light = {
    .primary_color = "blue",
    .secondary_color = "white",
    .accent_color = "orange",
    .text_color = "black",
    .background_color = "white", 
    .border_width = 2,
    .corner_radius = 4,
    .font = FONT_MEDIUM
};

// Current active theme
static const chinScreen_ui_theme_t* chinScreen_current_theme = &chinScreen_theme_default;

// Set UI theme
inline void chinScreen_set_theme(const chinScreen_ui_theme_t* theme) {
    chinScreen_current_theme = theme ? theme : &chinScreen_theme_default;
    Serial.println("chinScreen: UI theme updated");
}

//=============================================================================
// BUTTON CONTROLS (Using LVGL lv_btn)
//=============================================================================

// Enhanced button using LVGL's built-in button widget
inline lv_obj_t* chinScreen_button(const char* text,
                                  chinScreen_btn_callback_t callback,
                                  const char* vAlign = "middle",
                                  const char* hAlign = "center",
                                  int width = 100,
                                  int height = 50,
                                  const char* style = "default") {
    chinScreen_lock_display();

    lv_obj_t *btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, width, height);
    
    // Apply theme-based styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(btn, chinScreen_get_color(theme->primary_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(btn, chinScreen_get_color(theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(btn, theme->corner_radius, LV_PART_MAIN);
    
    // Style variations
    if (strcmp(style, "flat") == 0) {
        lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    } else if (strcmp(style, "outline") == 0) {
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 3, LV_PART_MAIN);
    } else if (strcmp(style, "raised") == 0) {
        lv_obj_set_style_shadow_width(btn, 8, LV_PART_MAIN);
        lv_obj_set_style_shadow_color(btn, lv_color_make(0, 0, 0), LV_PART_MAIN);
    }

    // Add text label if provided
    if (text && strlen(text) > 0) {
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, text);
        lv_obj_set_style_text_color(label, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
        lv_obj_set_style_text_font(label, theme->font, LV_PART_MAIN);
        lv_obj_center(label);
    }

    // Add event callback
    if (callback) {
        lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
    }

    // Position button
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(btn, align, 0, 0);

    chinScreen_unlock_display();
    return btn;
}

// Image button using LVGL's image button widget
inline lv_obj_t* chinScreen_image_button(const char* image_src,
                                        chinScreen_btn_callback_t callback,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center",
                                        int width = 80,
                                        int height = 80) {
    chinScreen_lock_display();

    lv_obj_t* imgbtn = lv_imagebutton_create(lv_screen_active());
    
    // Set image sources for different states (LVGL 9.3.0 format)
    if (image_src && strlen(image_src) > 0) {
        char lvgl_path[64];
        snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", image_src);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, lvgl_path, NULL);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, lvgl_path, NULL);
    }
    
    lv_obj_set_size(imgbtn, width, height);
    
    // Apply theme styling
    lv_obj_set_style_border_color(imgbtn, chinScreen_get_color(chinScreen_current_theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(imgbtn, chinScreen_current_theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(imgbtn, chinScreen_current_theme->corner_radius, LV_PART_MAIN);
    
    // Add callback
    if (callback) {
        lv_obj_add_event_cb(imgbtn, callback, LV_EVENT_CLICKED, NULL);
    }
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(imgbtn, align, 0, 0);

    chinScreen_unlock_display();
    return imgbtn;
}

// Toggle button (using LVGL checkbox as toggle)
inline lv_obj_t* chinScreen_toggle_button(const char* text,
                                         bool initial_state = false,
                                         const char* vAlign = "middle",
                                         const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t* cb = lv_checkbox_create(lv_screen_active());
    lv_checkbox_set_text(cb, text);
    
    if (initial_state) {
        lv_obj_add_state(cb, LV_STATE_CHECKED);
    }
    
    // Apply theme colors
    lv_obj_set_style_bg_color(cb, chinScreen_get_color(chinScreen_current_theme->primary_color), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(cb, chinScreen_get_color(chinScreen_current_theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(cb, chinScreen_current_theme->font, LV_PART_MAIN);
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(cb, align, 0, 0);

    chinScreen_unlock_display();
    return cb;
}

//=============================================================================
// SLIDER CONTROLS (Using LVGL lv_slider)
//=============================================================================

// Enhanced slider with LVGL's built-in slider widget
inline lv_obj_t* chinScreen_slider(int min_val, int max_val, int initial_val,
                                  const char* orientation = "horizontal",
                                  const char* vAlign = "middle",
                                  const char* hAlign = "center",
                                  int width = 200,
                                  int height = 20) {
    chinScreen_lock_display();

    lv_obj_t* slider = lv_slider_create(lv_screen_active());
    
    // Set size based on orientation
    if (strcmp(orientation, "vertical") == 0) {
        lv_obj_set_size(slider, height, width); // Swap dimensions for vertical
    } else {
        lv_obj_set_size(slider, width, height);
    }
    
    // Configure slider
    lv_slider_set_range(slider, min_val, max_val);
    lv_slider_set_value(slider, initial_val, LV_ANIM_OFF);
    
    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->accent_color), LV_PART_KNOB);
    lv_obj_set_style_radius(slider, theme->corner_radius, LV_PART_MAIN);
    lv_obj_set_style_radius(slider, LV_RADIUS_CIRCLE, LV_PART_KNOB);
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(slider, align, 0, 0);

    chinScreen_unlock_display();
    return slider;
}

// Slider with label showing current value
inline lv_obj_t* chinScreen_slider_with_label(int min_val, int max_val, int initial_val,
                                             const char* label_text = "Value",
                                             const char* vAlign = "middle",
                                             const char* hAlign = "center") {
    chinScreen_lock_display();

    // Create container
    lv_obj_t* container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(container, 250, 80);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create label
    lv_obj_t* label = lv_label_create(container);
    lv_label_set_text_fmt(label, "%s: %d", label_text, initial_val);
    lv_obj_set_style_text_color(label, chinScreen_get_color(chinScreen_current_theme->text_color), LV_PART_MAIN);
    
    // Create slider
    lv_obj_t* slider = lv_slider_create(container);
    lv_obj_set_size(slider, 200, 20);
    lv_slider_set_range(slider, min_val, max_val);
    lv_slider_set_value(slider, initial_val, LV_ANIM_OFF);
    
    // Apply styling to slider
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, chinScreen_get_color(theme->accent_color), LV_PART_KNOB);
    
    // Store label reference in user data for value updates
    lv_obj_set_user_data(slider, label);
    
    // Position container
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(container, align, 0, 0);

    chinScreen_unlock_display();
    return slider;
}

//=============================================================================
// SWITCH CONTROLS (Using LVGL lv_switch)
//=============================================================================

// Toggle switch using LVGL's built-in switch widget
inline lv_obj_t* chinScreen_switch(bool initial_state = false,
                                  const char* vAlign = "middle",
                                  const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t* sw = lv_switch_create(lv_screen_active());
    
    if (initial_state) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->accent_color), LV_PART_KNOB);
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(sw, align, 0, 0);

    chinScreen_unlock_display();
    return sw;
}

// Switch with label
inline lv_obj_t* chinScreen_labeled_switch(const char* label_text,
                                          bool initial_state = false,
                                          const char* vAlign = "middle",
                                          const char* hAlign = "center") {
    chinScreen_lock_display();

    // Create container with flex layout
    lv_obj_t* container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(container, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create label
    lv_obj_t* label = lv_label_create(container);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_color(label, chinScreen_get_color(chinScreen_current_theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_current_theme->font, LV_PART_MAIN);
    
    // Create switch
    lv_obj_t* sw = lv_switch_create(container);
    if (initial_state) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    
    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(sw, chinScreen_get_color(theme->accent_color), LV_PART_KNOB);
    
    // Position container
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(container, align, 0, 0);

    chinScreen_unlock_display();
    return sw;
}

//=============================================================================
// PROGRESS INDICATORS (Using LVGL progress widgets)
//=============================================================================

// Progress bar using LVGL's built-in bar widget
inline lv_obj_t* chinScreen_progress_bar(int width, int height,
                                        int min_val = 0, int max_val = 100, int current_val = 0,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center",
                                        bool show_label = false) {
    chinScreen_lock_display();

    lv_obj_t* bar = lv_bar_create(lv_screen_active());
    lv_obj_set_size(bar, width, height);
    lv_bar_set_range(bar, min_val, max_val);
    lv_bar_set_value(bar, current_val, LV_ANIM_ON);

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(bar, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, theme->corner_radius, LV_PART_MAIN);
    
    // Optional percentage label
    if (show_label) {
        lv_obj_t* label = lv_label_create(bar);
        lv_label_set_text_fmt(label, "%d%%", (current_val * 100) / (max_val - min_val));
        lv_obj_set_style_text_color(label, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
        lv_obj_center(label);
        lv_obj_set_user_data(bar, label); // Store for updates
    }

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(bar, align, 0, 0);

    chinScreen_unlock_display();
    return bar;
}

// Circular progress indicator using LVGL arc widget
inline lv_obj_t* chinScreen_progress_arc(int radius, 
                                        int min_val = 0, int max_val = 100, int current_val = 0,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center",
                                        bool show_label = false) {
    chinScreen_lock_display();

    lv_obj_t* arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(arc, radius * 2, radius * 2);
    lv_arc_set_range(arc, min_val, max_val);
    lv_arc_set_value(arc, current_val);
    
    // Configure arc appearance
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);  // Remove knob for progress indicator
    
    // Apply theme styling  
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_arc_color(arc, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
    lv_obj_set_style_arc_color(arc, chinScreen_get_color(theme->primary_color), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_MAIN);
    lv_obj_set_style_arc_width(arc, 8, LV_PART_INDICATOR);
    
    // Optional center label
    if (show_label) {
        lv_obj_t* label = lv_label_create(arc);
        lv_label_set_text_fmt(label, "%d%%", (current_val * 100) / (max_val - min_val));
        lv_obj_set_style_text_color(label, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
        lv_obj_center(label);
        lv_obj_set_user_data(arc, label); // Store for updates
    }

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(arc, align, 0, 0);

    chinScreen_unlock_display();
    return arc;
}

// Loading spinner using LVGL spinner widget
inline lv_obj_t* chinScreen_spinner(int size = 50,
                                   int speed_ms = 1000,
                                   const char* vAlign = "middle",
                                   const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t* spinner = lv_spinner_create(lv_screen_active(), speed_ms, 90);
    lv_obj_set_size(spinner, size, size);
    
    // Apply theme colors
    lv_obj_set_style_arc_color(spinner, chinScreen_get_color(chinScreen_current_theme->primary_color), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(spinner, 6, LV_PART_INDICATOR);
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(spinner, align, 0, 0);

    chinScreen_unlock_display();
    return spinner;
}

//=============================================================================
// INPUT CONTROLS (Using LVGL input widgets)
//=============================================================================

// Text area using LVGL's built-in textarea widget
inline lv_obj_t* chinScreen_textarea(const char* placeholder = "Enter text...",
                                    int width = 200, int height = 100,
                                    const char* vAlign = "middle",
                                    const char* hAlign = "center",
                                    bool multiline = true) {
    chinScreen_lock_display();

    lv_obj_t* ta = lv_textarea_create(lv_screen_active());
    lv_obj_set_size(ta, width, height);
    lv_textarea_set_placeholder_text(ta, placeholder);
    
    if (!multiline) {
        lv_textarea_set_one_line(ta, true);
    }

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(ta, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(ta, chinScreen_get_color(theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(ta, theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(ta, theme->corner_radius, LV_PART_MAIN);
    lv_obj_set_style_text_color(ta, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(ta, theme->font, LV_PART_MAIN);

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(ta, align, 0, 0);

    chinScreen_unlock_display();
    return ta;
}

// Virtual keyboard using LVGL's built-in keyboard widget
static lv_obj_t* chinScreen_active_keyboard = nullptr;

static void chinScreen_keyboard_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        if (chinScreen_active_keyboard) {
            lv_obj_delete(chinScreen_active_keyboard);
            chinScreen_active_keyboard = nullptr;
        }
    }
}

inline lv_obj_t* chinScreen_keyboard(lv_obj_t* target_textarea = nullptr,
                                    const char* vAlign = "bottom",
                                    const char* hAlign = "center") {
    if (chinScreen_active_keyboard) {
        Serial.println("chinScreen: Keyboard already active");
        return chinScreen_active_keyboard;
    }
    
    chinScreen_lock_display();

    chinScreen_active_keyboard = lv_keyboard_create(lv_screen_active());
    
    if (target_textarea) {
        lv_keyboard_set_textarea(chinScreen_active_keyboard, target_textarea);
    }
    
    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(chinScreen_active_keyboard, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(chinScreen_active_keyboard, chinScreen_get_color(theme->primary_color), LV_PART_ITEMS);
    lv_obj_set_style_text_color(chinScreen_active_keyboard, chinScreen_get_color(theme->text_color), LV_PART_ITEMS);
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(chinScreen_active_keyboard, align, 0, 0);
    
    // Add event handler for auto-close
    lv_obj_add_event_cb(chinScreen_active_keyboard, chinScreen_keyboard_event_cb, LV_EVENT_ALL, NULL);

    chinScreen_unlock_display();
    return chinScreen_active_keyboard;
}

// Number pad using LVGL button matrix widget
static lv_obj_t* chinScreen_active_numpad = nullptr;
static char chinScreen_numpad_result[32] = {0};

static const char* chinScreen_numpad_map[] = {
    "1", "2", "3", "\n",
    "4", "5", "6", "\n", 
    "7", "8", "9", "\n",
    "C", "0", "Del", "\n",
    "OK", ""
};

static void chinScreen_numpad_event_cb(lv_event_t* e) {
    lv_obj_t* obj = (lv_obj_t*)lv_event_get_target(e);
    const char* txt = lv_buttonmatrix_get_button_text(obj, lv_buttonmatrix_get_selected_button(obj));
    
    if (strcmp(txt, "C") == 0) {
        strcpy(chinScreen_numpad_result, "");
    } else if (strcmp(txt, "OK") == 0) {
        if (chinScreen_active_numpad) {
            lv_obj_delete(chinScreen_active_numpad);
            chinScreen_active_numpad = nullptr;
        }
    } else if (strcmp(txt, "Del") == 0) {
        int len = strlen(chinScreen_numpad_result);
        if (len > 0) {
            chinScreen_numpad_result[len - 1] = '\0';
        }
    } else {
        strcat(chinScreen_numpad_result, txt);
    }
    
    Serial.printf("chinScreen: Numpad value: %s\n", chinScreen_numpad_result);
}

inline lv_obj_t* chinScreen_numpad(const char* vAlign = "center",
                                  const char* hAlign = "center") {
    if (chinScreen_active_numpad) {
        Serial.println("chinScreen: Numpad already active");
        return chinScreen_active_numpad;
    }
    
    chinScreen_lock_display();

    chinScreen_active_numpad = lv_buttonmatrix_create(lv_screen_active());
    lv_buttonmatrix_set_map(chinScreen_active_numpad, chinScreen_numpad_map);
    lv_obj_set_size(chinScreen_active_numpad, 200, 250);
    
    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(chinScreen_active_numpad, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(chinScreen_active_numpad, chinScreen_get_color(theme->primary_color), LV_PART_ITEMS);
    lv_obj_set_style_text_color(chinScreen_active_numpad, chinScreen_get_color(theme->text_color), LV_PART_ITEMS);
    lv_obj_set_style_text_font(chinScreen_active_numpad, theme->font, LV_PART_ITEMS);
    lv_obj_set_style_radius(chinScreen_active_numpad, theme->corner_radius, LV_PART_ITEMS);
    
    // Clear previous result
    strcpy(chinScreen_numpad_result, "");
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(chinScreen_active_numpad, align, 0, 0);
    
    // Add event handler
    lv_obj_add_event_cb(chinScreen_active_numpad, chinScreen_numpad_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    chinScreen_unlock_display();
    return chinScreen_active_numpad;
}

//=============================================================================
// DROPDOWN AND SELECTION CONTROLS (Using LVGL dropdown widget)
//=============================================================================

// Dropdown menu using LVGL's built-in dropdown widget
inline lv_obj_t* chinScreen_dropdown(const char* options,
                                    const char* default_text = "Select...",
                                    const char* vAlign = "middle",
                                    const char* hAlign = "center",
                                    int width = 150) {
    chinScreen_lock_display();

    lv_obj_t* dropdown = lv_dropdown_create(lv_screen_active());
    lv_obj_set_width(dropdown, width);
    
    if (options && strlen(options) > 0) {
        lv_dropdown_set_options(dropdown, options);
    } else {
        lv_dropdown_set_options(dropdown, "Option 1\nOption 2\nOption 3");
    }
    
    lv_dropdown_set_text(dropdown, default_text);

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(dropdown, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(dropdown, chinScreen_get_color(theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(dropdown, theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(dropdown, theme->corner_radius, LV_PART_MAIN);
    lv_obj_set_style_text_color(dropdown, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(dropdown, theme->font, LV_PART_MAIN);

    // Style dropdown list
    lv_obj_set_style_bg_color(dropdown, chinScreen_get_color(theme->background_color), LV_PART_SELECTED);
    lv_obj_set_style_bg_color(dropdown, chinScreen_get_color(theme->primary_color), LV_PART_SELECTED | LV_STATE_CHECKED);

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(dropdown, align, 0, 0);

    chinScreen_unlock_display();
    return dropdown;
}

// Radio button group using LVGL button matrix
inline lv_obj_t* chinScreen_radio_group(const char* options,
                                       int default_selection = 0,
                                       const char* vAlign = "middle",
                                       const char* hAlign = "center",
                                       bool vertical = true) {
    chinScreen_lock_display();

    lv_obj_t* btnm = lv_buttonmatrix_create(lv_screen_active());
    
    // Parse options string and create button map
    static char* radio_map[20]; // Support up to 19 options + null terminator
    static char radio_options_copy[256];
    strncpy(radio_options_copy, options, sizeof(radio_options_copy) - 1);
    
    int option_count = 0;
    char* token = strtok(radio_options_copy, "\n");
    while (token && option_count < 19) {
        radio_map[option_count] = token;
        option_count++;
        if (vertical) {
            radio_map[option_count] = "\n";
            option_count++;
        }
        token = strtok(NULL, "\n");
    }
    radio_map[option_count] = "";
    
    lv_buttonmatrix_set_map(btnm, (const char**)radio_map);
    lv_buttonmatrix_set_button_ctrl_all(btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
    lv_buttonmatrix_set_one_checked(btnm, true);
    
    if (default_selection >= 0) {
        lv_buttonmatrix_set_selected_button(btnm, default_selection);
    }

    // Apply theme styling  
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(btnm, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_bg_color(btnm, chinScreen_get_color(theme->primary_color), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(btnm, chinScreen_get_color(theme->secondary_color), LV_PART_ITEMS);
    lv_obj_set_style_text_color(btnm, chinScreen_get_color(theme->text_color), LV_PART_ITEMS);
    lv_obj_set_style_radius(btnm, theme->corner_radius, LV_PART_ITEMS);

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(btnm, align, 0, 0);

    chinScreen_unlock_display();
    return btnm;
}

//=============================================================================
// ADVANCED UI COMPONENTS (Using LVGL widgets)
//=============================================================================

// Tabview using LVGL's built-in tabview widget
inline lv_obj_t* chinScreen_tabview(const char* tab_names,
                                   const char* position = "top",
                                   int tab_height = 50) {
    chinScreen_lock_display();

    lv_obj_t* tabview = lv_tabview_create(lv_screen_active());
    
    // Set tab position
    if (strcmp(position, "bottom") == 0) {
        lv_tabview_set_tab_bar_position(tabview, LV_DIR_BOTTOM);
    } else if (strcmp(position, "left") == 0) {
        lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    } else if (strcmp(position, "right") == 0) {
        lv_tabview_set_tab_bar_position(tabview, LV_DIR_RIGHT);
    } else {
        lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
    }
    
    lv_tabview_set_tab_bar_size(tabview, tab_height);

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(tabview, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_text_color(tabview, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(tabview, theme->font, LV_PART_MAIN);

    // Parse and add tabs
    if (tab_names && strlen(tab_names) > 0) {
        char* tab_names_copy = strdup(tab_names);
        char* token = strtok(tab_names_copy, "\n");
        while (token) {
            lv_tabview_add_tab(tabview, token);
            token = strtok(NULL, "\n");
        }
        free(tab_names_copy);
    }

    chinScreen_unlock_display();
    return tabview;
}

// List using LVGL's built-in list widget  
inline lv_obj_t* chinScreen_list(const char* items,
                                const char* vAlign = "middle",
                                const char* hAlign = "center",
                                int width = 200,
                                int height = 300) {
    chinScreen_lock_display();

    lv_obj_t* list = lv_list_create(lv_screen_active());
    lv_obj_set_size(list, width, height);

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(list, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(list, chinScreen_get_color(theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(list, theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(list, theme->corner_radius, LV_PART_MAIN);

    // Add items
    if (items && strlen(items) > 0) {
        char* items_copy = strdup(items);
        char* token = strtok(items_copy, "\n");
        while (token) {
            lv_obj_t* btn = lv_list_add_button(list, NULL, token);
            lv_obj_set_style_bg_color(btn, chinScreen_get_color(theme->secondary_color), LV_PART_MAIN);
            lv_obj_set_style_text_color(btn, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
            token = strtok(NULL, "\n");
        }
        free(items_copy);
    }

    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(list, align, 0, 0);

    chinScreen_unlock_display();
    return list;
}

// Message box using LVGL's built-in msgbox widget
inline lv_obj_t* chinScreen_message_box(const char* title,
                                       const char* message,
                                       const char* buttons = "OK",
                                       chinScreen_btn_callback_t callback = nullptr) {
    chinScreen_lock_display();

    // Create message box buttons array
    static const char* msgbox_btns[5];
    int btn_count = 0;
    
    if (buttons && strlen(buttons) > 0) {
        char* buttons_copy = strdup(buttons);
        char* token = strtok(buttons_copy, "\n");
        while (token && btn_count < 4) {
            msgbox_btns[btn_count] = strdup(token);
            btn_count++;
            token = strtok(NULL, "\n");
        }
        free(buttons_copy);
    }
    msgbox_btns[btn_count] = NULL;

    lv_obj_t* msgbox = lv_msgbox_create(lv_screen_active());
    lv_msgbox_add_title(msgbox, title);
    lv_msgbox_add_text(msgbox, message);
    lv_msgbox_add_close_button(msgbox);
    
    // Add custom buttons if provided
    for (int i = 0; i < btn_count; i++) {
        lv_obj_t* btn = lv_msgbox_add_footer_button(msgbox, msgbox_btns[i]);
        if (callback) {
            lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
        }
    }

    // Apply theme styling
    const chinScreen_ui_theme_t* theme = chinScreen_current_theme;
    lv_obj_set_style_bg_color(msgbox, chinScreen_get_color(theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(msgbox, chinScreen_get_color(theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_text_color(msgbox, chinScreen_get_color(theme->text_color), LV_PART_MAIN);
    lv_obj_set_style_text_font(msgbox, theme->font, LV_PART_MAIN);

    chinScreen_unlock_display();
    return msgbox;
}

//=============================================================================
// UI UTILITY FUNCTIONS
//=============================================================================

// Get current values from UI components
inline int chinScreen_slider_get_value(lv_obj_t* slider) {
    return slider ? lv_slider_get_value(slider) : 0;
}

inline bool chinScreen_switch_get_state(lv_obj_t* sw) {
    return sw ? lv_obj_has_state(sw, LV_STATE_CHECKED) : false;
}

inline bool chinScreen_checkbox_get_state(lv_obj_t* cb) {
    return cb ? lv_obj_has_state(cb, LV_STATE_CHECKED) : false;
}

inline const char* chinScreen_textarea_get_text(lv_obj_t* ta) {
    return ta ? lv_textarea_get_text(ta) : "";
}

inline int chinScreen_dropdown_get_selected(lv_obj_t* dropdown) {
    return dropdown ? lv_dropdown_get_selected(dropdown) : -1;
}

inline const char* chinScreen_numpad_get_result() {
    return chinScreen_numpad_result;
}

inline int chinScreen_numpad_get_int() {
    return atoi(chinScreen_numpad_result);
}

inline float chinScreen_numpad_get_float() {
    return atof(chinScreen_numpad_result);
}

// Update UI component values  
inline void chinScreen_progress_set_value(lv_obj_t* bar, int value) {
    if (!bar) return;
    chinScreen_lock_display();
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    
    // Update label if it exists
    lv_obj_t* label = (lv_obj_t*)lv_obj_get_user_data(bar);
    if (label) {
        int range = lv_bar_get_max_value(bar) - lv_bar_get_min_value(bar);
        lv_label_set_text_fmt(label, "%d%%", (value * 100) / range);
    }
    chinScreen_unlock_display();
}

inline void chinScreen_slider_set_value(lv_obj_t* slider, int value) {
    if (!slider) return;
    chinScreen_lock_display();
    lv_slider_set_value(slider, value, LV_ANIM_ON);
    
    // Update label if it exists  
    lv_obj_t* label = (lv_obj_t*)lv_obj_get_user_data(slider);
    if (label) {
        lv_label_set_text_fmt(label, "Value: %d", value);
    }
    chinScreen_unlock_display();
}

inline void chinScreen_arc_set_value(lv_obj_t* arc, int value) {
    if (!arc) return;
    chinScreen_lock_display();
    lv_arc_set_value(arc, value);
    
    // Update center label if it exists
    lv_obj_t* label = (lv_obj_t*)lv_obj_get_user_data(arc);
    if (label) {
        int range = lv_arc_get_max_value(arc) - lv_arc_get_min_value(arc);
        lv_label_set_text_fmt(label, "%d%%", (value * 100) / range);
    }
    chinScreen_unlock_display();
}

// Close active input controls
inline void chinScreen_close_keyboard() {
    if (chinScreen_active_keyboard) {
        chinScreen_lock_display();
        lv_obj_delete(chinScreen_active_keyboard);
        chinScreen_active_keyboard = nullptr;
        chinScreen_unlock_display();
    }
}

inline void chinScreen_close_numpad() {
    if (chinScreen_active_numpad) {
        chinScreen_lock_display();
        lv_obj_delete(chinScreen_active_numpad);
        chinScreen_active_numpad = nullptr;
        chinScreen_unlock_display();
    }
}

//=============================================================================
// LEGACY COMPATIBILITY FUNCTIONS
//=============================================================================

// Legacy function aliases with deprecation warnings
inline lv_obj_t* chinScreen_toggle(bool initial_state = false,
                                  const char* bgColorName = "gray", 
                                  const char* knobColorName = "white",
                                  const char* vAlign = "middle", 
                                  const char* hAlign = "center") {
    chinScreen_warn_deprecated("chinScreen_toggle", "chinScreen_switch");
    return chinScreen_switch(initial_state, vAlign, hAlign);
}

inline bool chinScreen_toggle_get_state(lv_obj_t* toggle) {
    chinScreen_warn_deprecated("chinScreen_toggle_get_state", "chinScreen_switch_get_state");
    return chinScreen_switch_get_state(toggle);
}

inline lv_obj_t* chinScreen_keyboard_LEGACY(const char* placeholder = "Type here...",
                                           const char* vAlign = "bottom",
                                           const char* hAlign = "center") {
    chinScreen_warn_deprecated("Legacy keyboard function", "chinScreen_textarea + chinScreen_keyboard");
    
    // Create textarea and keyboard
    lv_obj_t* ta = chinScreen_textarea(placeholder, 300, 40, "top", "center", false);
    lv_obj_t* kb = chinScreen_keyboard(ta, vAlign, hAlign);
    
    return kb;
}

#endif // CHINSCREEN_INTERFACE_H