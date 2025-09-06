/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

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


    
    example_basic_shapes();
    delay(3000);

}

void loop() {
    // infinite void    
    delay(100);
}


void example_basic_shapes() {
    Serial.println("=== Basic Shapes Example ===");
    
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    // Create various shapes
    lv_obj_t* rect = chinScreen_rectangle("red", "white", 80, 60, "top", "left");
    chinScreen_shape_text(rect, "Rect", "white", "small");
    
    lv_obj_t* circle = chinScreen_circle("blue", "yellow", 40, "top", "right");
    chinScreen_shape_text(circle, "Circle", "white", "small");
    
   // lv_obj_t* triangle = chinScreen_triangle("green", "red", 70, 70, "middle", "left");
    
   // lv_obj_t* hexagon = chinScreen_hexagon("purple", "white", 35, "middle", "right");
    
    lv_obj_t* oval = chinScreen_oval("orange", "blue", 100, 60, "bottom", "center");
    chinScreen_shape_text(oval, "Oval", "black", "medium");
    
    // Add some text
    chinScreen_text("Basic Shapes Demo", 10, 10, "white", "large");
}

