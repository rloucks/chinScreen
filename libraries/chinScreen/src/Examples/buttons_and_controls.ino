/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

#include "chinScreen.h"

// Global objects for interaction
lv_obj_t* myButton = nullptr;
lv_obj_t* mySlider = nullptr;
lv_obj_t* myToggle = nullptr;
lv_obj_t* myCircle = nullptr;
lv_obj_t* myProgressBar = nullptr;

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


    
    example_buttons_and_controls();
    delay(3000);

}

void loop() {
    // Check slider value and update progress bar
    if (mySlider && myProgressBar) {
        int sliderValue = chinScreen_slider_get_value(mySlider);
        chinScreen_progress_set_value(myProgressBar, sliderValue);
    }
    
    // Check toggle state
    if (myToggle) {
        bool toggleState = chinScreen_toggle_get_state(myToggle);
        if (toggleState) {
            chinScreen_backlight(1);
        } else {
            chinScreen_brightness(50); // Dim when toggle is off
        }
    }
    
    delay(100);
}


void example_buttons_and_controls() {
    Serial.println("=== Buttons and Controls Example ===");
    
    chinScreen_clear();
    chinScreen_background_solid("gray");
    
    // Create button
    myButton = chinScreen_button("green", "white", 120, 50, "Press Me!", 
                                myButtonCallback, "top", "center");
    
    // Create slider
    mySlider = chinScreen_slider(200, 20, 0, 100, 50, "white", "blue", "middle", "center");
    chinScreen_text("Slider:", 10, 240, "white", "medium");
    
    // Create toggle
    myToggle = chinScreen_toggle(true, "red", "white", "bottom", "left");
    chinScreen_text("Toggle:", 10, 400, "white", "medium");
    
    // Create progress bar
    myProgressBar = chinScreen_progress_bar(180, 15, 50, "white", "lime", "bottom", "center");
    chinScreen_text("Progress:", 10, 350, "white", "medium");
    
    // Create circle for animation
    myCircle = chinScreen_circle("yellow", "red", 30, "middle", "left");
}

