/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

#include "chinScreen.h"

// Global objects for interaction
lv_obj_t* myCircle = nullptr;


// Button callback functions
void myButtonCallback(lv_event_t* e) {
    Serial.println("My button was pressed!");
    
    // Change circle color when button is pressed
    if (myCircle) {
        chinScreen_animate_advanced(myCircle, ANIM_SCALE, 50, 80, 500, false, "bounce");
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("chinScreen Library Examples");

    // Initialize display
    init_display();
    
    // Wait a moment for display to stabilize
    delay(500);


    
    example_animations();
    delay(3000);

}

void loop() {
    // infinite void    
    delay(100);
}


void example_animations() {
    Serial.println("=== Animation Examples ===");
    
    chinScreen_clear();
    chinScreen_background_gradient("black", "purple", "vertical");
    
    // Create shapes to animate
    lv_obj_t* moveRect = chinScreen_rectangle("red", "white", 50, 50, "top", "left");
    lv_obj_t* scaleCircle = chinScreen_circle("blue", "white", 25, "top", "right");
    lv_obj_t* rotateTriangle = chinScreen_triangle("green", "white", 60, 60, "bottom", "left");
    lv_obj_t* fadeOval = chinScreen_oval("orange", "white", 80, 50, "bottom", "right");
    
    // Start animations
    chinScreen_animate_advanced(moveRect, ANIM_MOVE_X, 0, 200, 2000, true, "ease_in_out");
    chinScreen_animate_advanced(scaleCircle, ANIM_SCALE, 50, 100, 1500, true, "bounce");
    chinScreen_animate_advanced(rotateTriangle, ANIM_ROTATE, 0, 360, 3000, true, "linear");
    chinScreen_animate_advanced(fadeOval, ANIM_FADE, LV_OPA_COVER, LV_OPA_20, 1000, true, "ease_in_out");
    
    chinScreen_text("Animation Demo", 10, 10, "white", "large");
}

