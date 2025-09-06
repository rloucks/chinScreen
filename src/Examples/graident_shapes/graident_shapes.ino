/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */
#define LVGL_PORT_ROTATION_DEGREE 180
#include "chinScreen.h"

// Global objects for interaction
lv_obj_t* myCircle = nullptr;



void setup() {
    Serial.begin(115200);
    Serial.println("chinScreen Library Examples");

    // Initialize display
    init_display();
    
    // Wait a moment for display to stabilize
    delay(500);

  
    example_gradient_shapes(); 
    delay(3000);

}

void loop() {
    // infinite void    
    delay(100);
}


// Utility function to create shape with gradient background
void example_gradient_shapes() {
    Serial.println("=== Gradient Shapes Example ===");
    
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    // Rectangle with gradient
    lv_obj_t* gradRect = chinScreen_rectangle("red", "white", 100, 80, "top", "left", "yellow", "horizontal");
    chinScreen_shape_text(gradRect, "Gradient", "black", "medium");
    
    // Circle with gradient
    lv_obj_t* gradCircle = chinScreen_circle("blue", "white", 50, "top", "right", "cyan", "vertical");
    chinScreen_shape_text(gradCircle, "Grad", "white", "small");
    
    chinScreen_text("Gradient Shapes", 10, 200, "white", "large");
}