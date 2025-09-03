#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== DEBUG TEST - Simplified Display Test ===");

    // Step 1: Initialize display with larger buffer
    Serial.println("Initializing display...");
    
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = 320 * 80,  // Increased buffer size
        .rotate = LV_DISPLAY_ROTATION_0,
    };

    lv_display_t *disp = bsp_display_start_with_config(&cfg);
    if (disp == NULL) {
        Serial.println("FAILED: Display initialization failed!");
        return;
    }
    
    Serial.println("SUCCESS: Display initialized");
    
    // Step 2: Turn on backlight and wait
    bsp_display_backlight_on();
    delay(500);  // Give display time to stabilize
    
    // Step 3: Test basic drawing without chinScreen functions
    Serial.println("Testing basic LVGL drawing...");
    
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    
    // Set solid background color first
    lv_obj_set_style_bg_color(scr, lv_color_make(255, 0, 0), LV_PART_MAIN);  // Red background
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    // Force refresh
    lv_obj_invalidate(scr);
    lv_refr_now(disp);
    
    Serial.println("Red background should be visible");
    delay(2000);
    
    // Step 4: Try simple text
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "HELLO");
    lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    // Force refresh again
    lv_obj_invalidate(scr);
    lv_refr_now(disp);
    
    Serial.println("White text should be visible");
    delay(2000);
    
    // Step 5: Test chinScreen functions
    Serial.println("Testing chinScreen functions...");
    chinScreen_background_solid("blue");
    delay(1000);
    
    chinScreen_text("TEST", 50, 50, "white", "large");
    delay(1000);
    
    chinScreen_rectangle("green", "yellow", 100, 100);
    
    Serial.println("Setup complete - check display");
}

void loop() {
    static unsigned long last_tick = 0;
    unsigned long current_tick = millis();
    lv_tick_inc(current_tick - last_tick);
    last_tick = current_tick;
    
    lv_timer_handler();
    delay(10);
}