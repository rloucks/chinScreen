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

void example_basic_shapes() {
    Serial.println("=== Basic Shapes Example ===");
    
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    // Create various shapes
    lv_obj_t* rect = chinScreen_rectangle("red", "white", 80, 60, "top", "left");
    chinScreen_shape_text(rect, "Rect", "white", "small");
    
    lv_obj_t* circle = chinScreen_circle("blue", "yellow", 40, "top", "right");
    chinScreen_shape_text(circle, "Circle", "white", "small");
    
    lv_obj_t* triangle = chinScreen_triangle("green", "red", 70, 70, "middle", "left");
    
    lv_obj_t* hexagon = chinScreen_hexagon("purple", "white", 35, "middle", "right");
    
    lv_obj_t* oval = chinScreen_oval("orange", "blue", 100, 60, "bottom", "center");
    chinScreen_shape_text(oval, "Oval", "black", "medium");
    
    // Add some text
    chinScreen_text("Basic Shapes Demo", 10, 10, "white", "large");
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