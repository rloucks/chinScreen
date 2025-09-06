/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

#define LVGL_PORT_ROTATION_DEGREE 180
#include "chinScreen.h"

// Global objects for interaction


void setup() {
    Serial.begin(115200);
    init_display();
    chinScreen_init_sd_card(5);
    
    // Blue gradient background
    chinScreen_background_gradient("navy", "blue", "vertical");
    
    // Title
    chinScreen_text("Simple Calculator", 10, 10, "white", "large");
    
    // Calculator button - opens numpad when pressed
    chinScreen_button("green", "white", 200, 60, "Open Calculator", show_calculator, "middle", "center");
}

void loop() {
    delay(10);
}

void show_calculator(lv_event_t* e) {
    chinScreen_clear();
    chinScreen_background_gradient("navy", "blue", "vertical");
    chinScreen_text("Enter a number:", 10, 50, "white", "medium");
    chinScreen_numpad("0", "middle", "center");
}