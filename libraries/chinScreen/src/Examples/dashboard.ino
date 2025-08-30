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


    
    example_dashboard();
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


// Advanced example: Interactive dashboard
void example_dashboard() {
    Serial.println("=== Dashboard Example ===");
    
    chinScreen_clear();
    chinScreen_background_gradient("black", "navy", "vertical");
    
    // Title
    chinScreen_text("System Dashboard", 10, 10, "cyan", "large");
    
    // Status indicators
    lv_obj_t* status1 = chinScreen_circle("green", "white", 15, "top", "right");
    chinScreen_shape_text(status1, "OK", "black", "small");
    
    lv_obj_t* status2 = chinScreen_circle("yellow", "white", 15, "top", "right");
    chinScreen_set_position(status2, 280, 50);
    chinScreen_shape_text(status2, "!", "black", "small");
    
    // Temperature gauge (using progress bar)
    lv_obj_t* tempGauge = chinScreen_progress_bar(200, 20, 75, "blue", "red", "middle", "left");
    chinScreen_text("Temperature: 75°C", 10, 200, "white", "medium");
    
    // Control buttons
    lv_obj_t* startBtn = chinScreen_button("green", "white", 80, 40, "START", nullptr, "bottom", "left");
    lv_obj_t* stopBtn = chinScreen_button("red", "white", 80, 40, "STOP", nullptr, "bottom", "center");
    lv_obj_t* resetBtn = chinScreen_button("orange", "white", 80, 40, "RESET", nullptr, "bottom", "right");
}

