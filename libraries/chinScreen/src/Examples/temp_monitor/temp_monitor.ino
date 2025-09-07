//=============================================================================
// EXAMPLE 6: Temperature Monitor with Animations
//=============================================================================
#include "chinScreen.h"

lv_obj_t* temp_circle;
lv_obj_t* temp_bar;

void setup() {
    Serial.begin(115200);
    init_display();
    //chinScreen_init_sd_card(5);
    
    // Cool gradient background
    chinScreen_background_gradient("black", "purple", "vertical");
    
    // Title
    chinScreen_text("Temperature", 10, 10, "white", "large");
    
    // Temperature display circle
    temp_circle = chinScreen_circle("red", "white", 60, "top", "right");
    chinScreen_shape_text(temp_circle, "75°F", "white", "large");
    
    // Progress bar for temperature
    temp_bar = chinScreen_progress_bar(200, 20, 75, "gray", "orange", "middle", "center");
    
    // Animate the circle (pulsing effect)
    chinScreen_animate_advanced(temp_circle, ANIM_SCALE, 120, 140, 2000, true, "ease_in_out");
    
    // Control buttons
    chinScreen_button("green", "yellow", 100, 40, "Refresh", update_temp, "bottom", "center");
}

void loop() {
    delay(10);
}

void update_temp(lv_event_t* e) {
    // Simulate temperature reading
    int new_temp = random(60, 90);
    chinScreen_progress_set_value(temp_bar, new_temp);
    
    // Update circle text (in real code, you'd update the label)
    Serial.printf("Temperature updated to %d°F\n", new_temp);
}