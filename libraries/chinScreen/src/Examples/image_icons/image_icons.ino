#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Flush callback debug test");
    
    // Use working buffer size
    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = 320 * 10,
        .rotate = LV_DISPLAY_ROTATION_0,
    };
    
    lv_display_t *disp = bsp_display_start_with_config(&cfg);
    if (disp != NULL) {
        Serial.println("Display initialized successfully");
        
        bsp_display_backlight_on();
        delay(500);
        
        // Force a full screen refresh with debugging
        Serial.println("Testing display refresh...");
        
        lv_obj_t *scr = lv_scr_act();
        if (scr) {
            Serial.println("Got active screen");
            
            // Set background to red
            lv_obj_set_style_bg_color(scr, lv_color_make(255, 0, 0), LV_PART_MAIN);
            lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
            Serial.println("Set background color");
            
            // Force immediate refresh
            lv_obj_invalidate(scr);
            Serial.println("Invalidated screen");
            
            // Process LVGL tasks to trigger flush
            for (int i = 0; i < 10; i++) {
                lv_timer_handler();
                delay(100);
                Serial.printf("LVGL timer handler call %d\n", i+1);
            }
            
            // Try forcing a refresh
            lv_refr_now(disp);
            Serial.println("Forced immediate refresh");
            
            // Create a visible object to force more drawing
            lv_obj_t *label = lv_label_create(scr);
            lv_label_set_text(label, "TEST");
            lv_obj_set_style_text_color(label, lv_color_make(255, 255, 255), LV_PART_MAIN);
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            Serial.println("Created test label");
            
            // More timer calls
            for (int i = 0; i < 10; i++) {
                lv_timer_handler();
                delay(100);
            }
            
        } else {
            Serial.println("ERROR: Could not get active screen");
        }
        
    } else {
        Serial.println("Display initialization failed");
    }
    
    Serial.println("Setup complete - check if flush callbacks were called in serial output");
}

void loop() {
    static unsigned long last_tick = 0;
    unsigned long current_tick = millis();
    lv_tick_inc(current_tick - last_tick);
    last_tick = current_tick;
    
    lv_timer_handler();
    delay(50);
}