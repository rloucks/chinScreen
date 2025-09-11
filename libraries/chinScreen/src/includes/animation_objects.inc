// simple_objects.inc
// Basic object creation that works after your animations are running
// No timers, no locks, no complex systems

/////////////////////////////////////////////////////////////
// Simple Object Creation Functions
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Create text that stays visible over animations
/////////////////////////////////////////////////////////////
lv_obj_t* chinScreen_add_text(const char* text, lv_coord_t x, lv_coord_t y) {
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_pos(label, x, y);
    
    // White text with black background for visibility
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_set_style_bg_color(label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(label, LV_OPA_80, LV_PART_MAIN);
    lv_obj_set_style_pad_all(label, 5, LV_PART_MAIN);
    lv_obj_set_style_radius(label, 5, LV_PART_MAIN);
    
    return label;
}

/////////////////////////////////////////////////////////////
// Create button that stays visible over animations
/////////////////////////////////////////////////////////////
lv_obj_t* chinScreen_add_button(const char* text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, w, h);
    lv_obj_set_pos(btn, x, y);
    
    // Style for visibility
    lv_obj_set_style_bg_color(btn, lv_color_make(0, 100, 200), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(btn, LV_OPA_90, LV_PART_MAIN);
    
    // Add label to button
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_center(label);
    
    return btn;
}

/////////////////////////////////////////////////////////////
// Create panel that stays visible over animations  
/////////////////////////////////////////////////////////////
lv_obj_t* chinScreen_add_panel(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) {
    lv_obj_t* panel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(panel, w, h);
    lv_obj_set_pos(panel, x, y);
    
    // Style for visibility
    lv_obj_set_style_bg_color(panel, lv_color_make(20, 20, 20), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(panel, LV_OPA_80, LV_PART_MAIN);
    lv_obj_set_style_border_color(panel, lv_color_make(100, 100, 100), LV_PART_MAIN);
    lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(panel, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(panel, 10, LV_PART_MAIN);
    
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    
    return panel;
}

/////////////////////////////////////////////////////////////
// Force an object to the front (call manually when needed)
/////////////////////////////////////////////////////////////
void chinScreen_bring_to_front(lv_obj_t* obj) {
    if (obj) {
        lv_obj_move_foreground(obj);
    }
}

/////////////////////////////////////////////////////////////
// Example usage function
/////////////////////////////////////////////////////////////
void chinScreen_demo_simple_objects() {
    // Wait for your initialization to complete
    delay(2000);
    
    // Create simple objects
    lv_obj_t* title = chinScreen_add_text("Demo", 10, 10);
    lv_obj_t* btn = chinScreen_add_button("Test", 50, 100, 80, 40);
    lv_obj_t* panel = chinScreen_add_panel(150, 80, 120, 80);
    
    // Add text to panel
    lv_obj_t* panel_text = lv_label_create(panel);
    lv_label_set_text(panel_text, "Panel Text");
    lv_obj_set_style_text_color(panel_text, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_align(panel_text, LV_ALIGN_CENTER, 0, 0);
    
    Serial.println("Simple objects created");
}