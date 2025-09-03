#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== FINAL TEST - After LCD Init Fix ===");

    Serial.println("Initializing with corrected LCD commands...");
    
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = 320 * 60,  // Slightly larger buffer
        .rotate = LV_DISPLAY_ROTATION_0,
    };

    lv_display_t *disp = bsp_display_start_with_config(&cfg);
    if (disp == NULL) {
        Serial.println("BSP initialization FAILED");
        return;
    }
    
    Serial.println("BSP initialization SUCCESS");
    
    // Turn on backlight
    bsp_display_backlight_on();
    
    // Longer delay for LCD to fully initialize with new commands
    delay(1000);
    Serial.println("LCD stabilization complete");
    
    // Create test UI
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    
    // Green background (different from previous tests)
    lv_obj_set_style_bg_color(scr, lv_color_make(0, 255, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    // Large black text for contrast
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "SUCCESS!\nLVGL 9.3.0\nWORKING");
    lv_obj_set_style_text_color(label, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    // Add a colored rectangle for extra visual confirmation
    lv_obj_t *rect = lv_obj_create(scr);
    lv_obj_set_size(rect, 200, 80);
    lv_obj_set_style_bg_color(rect, lv_color_make(255, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(rect, lv_color_make(0, 0, 255), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 5, LV_PART_MAIN);
    lv_obj_align(rect, LV_ALIGN_BOTTOM_MID, 0, -20);
    
    // White text in the rectangle
    lv_obj_t *rect_label = lv_label_create(rect);
    lv_label_set_text(rect_label, "LVGL 9.3.0");
    lv_obj_set_style_text_color(rect_label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_center(rect_label);
    
    Serial.println("UI created - performing final refresh...");
    
    // Single refresh
    lv_obj_invalidate(scr);
    lv_refr_now(disp);
    
    delay(500);
    
    Serial.println("=== TEST COMPLETE ===");
    Serial.println("Expected: Green background, black 'SUCCESS!' text, red rectangle with blue border");
    Serial.println("If you see this, LVGL 9.3.0 migration is COMPLETE! 🎉");
}

void loop() {
    static unsigned long last_tick = 0;
    unsigned long current_tick = millis();
    lv_tick_inc(current_tick - last_tick);
    last_tick = current_tick;
    
    lv_timer_handler();
    delay(10);
}