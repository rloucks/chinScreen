/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

#include "chinScreen.h"

// Global objects for interaction


void setup() {
    Serial.begin(115200);
    Serial.println("chinScreen Library Examples");

    // Initialize display
    init_display();
    
    // Wait a moment for display to stabilize
    delay(500);


    
    example_backgrounds();
    delay(3000);

}

void loop() {
    // infinite void    
    delay(100);
}




void example_backgrounds() {
    Serial.println("=== Background Examples ===");
    
    chinScreen_clear();
    
    // Solid background
    chinScreen_background_solid("navy");
    chinScreen_text("Solid Background", 10, 10, "white", "large");
    delay(2000);
    
    // Gradient backgrounds
    chinScreen_background_gradient("red", "yellow", "vertical");
    chinScreen_text("Vertical Gradient", 10, 50, "black", "large");
    delay(2000);
    
    chinScreen_background_gradient("blue", "cyan", "horizontal");
    chinScreen_text("Horizontal Gradient", 10, 90, "white", "large");
    delay(2000);
}
