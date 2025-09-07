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
    
    // Purple gradient background
    chinScreen_background_gradient("purple", "pink", "vertical");
    
    // Welcome message
    chinScreen_text("Welcome!", 10, 20, "white", "xlarge");
    chinScreen_text("Please enter your name", 10, 60, "white", "medium");
    
    // Name entry button
    chinScreen_button("orange", "white", 250, 50, "Enter Your Name", show_keyboard, "middle", "center");
    
    // Decorative circle
    chinScreen_circle("yellow", "white", 30, "top", "right");
}

void loop() {
    delay(10);
}

void show_keyboard(lv_event_t* e) {
    chinScreen_clear();
    chinScreen_background_gradient("purple", "pink", "vertical");
    chinScreen_text("Type your name:", 10, 10, "white", "large");
    chinScreen_keyboard("Your name here...", "bottom", "center");
}