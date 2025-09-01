// Test code for embedded images with chinScreen library
#include "chinScreen.h"

// Include your embedded image
extern const lv_img_dsc_t settings;  // From your settings.c file

void setup() {
    Serial.begin(115200);
    init_display();
    
    Serial.println("Testing embedded images...");
    
    // Dark gradient background
    chinScreen_background_gradient("navy", "purple", "vertical");
    
    // Title
    chinScreen_text("Embedded Image Test", 10, 10, "cyan", "large");
    
    // Create image object using existing LVGL functions directly
    bsp_display_lock(0);
    
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &settings);  // Set the embedded image
    lv_obj_set_pos(img, 50, 100);    // Position it
    
    bsp_display_unlock();
    
    // Test if it worked
    chinScreen_text("Settings Icon Test", 10, 50, "white", "medium");
    chinScreen_circle("orange", "white", 20, "bottom", "right");
    
    Serial.println("Test setup complete!");
}

void loop() {
    delay(100);
}