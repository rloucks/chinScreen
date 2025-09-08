#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

// Global timer handles (chinScreen types, not LVGL)
chinScreen_timer_t color_timer = nullptr;
chinScreen_timer_t gradient_timer = nullptr;

// Color cycling variables
int current_color_index = 0;
int gradient_phase = 0;

// Available colors for cycling
const char* cycle_colors[] = {
    "red", "orange", "yellow", "lime", "green", "cyan", 
    "blue", "purple", "magenta", "pink", "white"
};
const int num_colors = sizeof(cycle_colors) / sizeof(cycle_colors[0]);

// Gradient color pairs
const char* gradient_pairs[][2] = {
    {"red", "orange"},
    {"orange", "yellow"}, 
    {"yellow", "lime"},
    {"lime", "cyan"},
    {"cyan", "blue"},
    {"blue", "purple"},
    {"purple", "magenta"},
    {"magenta", "red"}
};
const int num_gradient_pairs = sizeof(gradient_pairs) / sizeof(gradient_pairs[0]);

//=============================================================================
// Example 1: Transitioning Background Colors
//=============================================================================
void color_transition_callback(void* user_data) {
    // Cycle through background colors
    chinScreen_background_solid(cycle_colors[current_color_index]);
    
    current_color_index++;
    if (current_color_index >= num_colors) {
        current_color_index = 0;
    }
    
    Serial.print("Background color changed to: ");
    Serial.println(cycle_colors[current_color_index]);
}

void stop_color_transitions(lv_event_t* e) {
    if (color_timer) {
        chinScreen_timer_delete(color_timer);
        color_timer = nullptr;
        Serial.println("Background transition stopped");
    }
}

void example_1_transitioning_backgrounds() {
    Serial.println("=== Example 1: Transitioning Background Colors ===");
    
    // Clear screen and stop any existing timers
    chinScreen_clear();
    chinScreen_timer_stop_all();
    
    // Add title text
    chinScreen_text("Transitioning Backgrounds", 10, 10, "white", "large");
    chinScreen_text("Watch the background colors cycle!", 10, 40, "white", "medium");
    
    // Create a button to stop the animation
    chinScreen_button("darkred", "white", 120, 40, "Stop", stop_color_transitions, "bottom", "center");
    
    // Start the color transition timer (change every 800ms) - pure chinScreen!
    color_timer = chinScreen_timer_create(color_transition_callback, 800);
    
    Serial.println("Background color transitions started! Press stop button to end.");
}

//=============================================================================
// Example 2: Animated Gradient Backgrounds  
//=============================================================================
void gradient_transition_callback(void* user_data) {
    // Cycle through gradient pairs
    const char* color1 = gradient_pairs[gradient_phase][0];
    const char* color2 = gradient_pairs[gradient_phase][1];
    
    // Alternate between horizontal and vertical gradients
    const char* direction = (gradient_phase % 2 == 0) ? "horizontal" : "vertical";
    
    chinScreen_background_gradient(color1, color2, direction);
    
    gradient_phase++;
    if (gradient_phase >= num_gradient_pairs) {
        gradient_phase = 0;
    }
    
    Serial.printf("Gradient: %s -> %s (%s)\n", color1, color2, direction);
}

void stop_gradient_animation(lv_event_t* e) {
    if (gradient_timer) {
        chinScreen_timer_delete(gradient_timer);
        gradient_timer = nullptr;
        Serial.println("Gradient animation stopped");
    }
}

void speed_up_gradients(lv_event_t* e) {
    if (gradient_timer) {
        chinScreen_timer_speed_up(gradient_timer, 200);
    }
}

void slow_down_gradients(lv_event_t* e) {
    if (gradient_timer) {
        chinScreen_timer_slow_down(gradient_timer, 200);
    }
}

void example_2_animated_gradients() {
    Serial.println("=== Example 2: Animated Gradient Backgrounds ===");
    
    // Clear screen and stop any existing timers
    chinScreen_clear();
    chinScreen_timer_stop_all();
    
    // Add title text
    chinScreen_text("Animated Gradients", 10, 10, "white", "large");
    chinScreen_text("Beautiful flowing colors!", 10, 40, "white", "medium");
    
    // Create control buttons - all using pure chinScreen functions
    chinScreen_button("darkred", "white", 80, 35, "Stop", stop_gradient_animation, "bottom", "left");
    chinScreen_button("orange", "white", 80, 35, "Speed+", speed_up_gradients, "bottom", "center");
    chinScreen_button("blue", "white", 80, 35, "Speed-", slow_down_gradients, "bottom", "right");
    
    // Start the gradient animation timer - pure chinScreen!
    gradient_timer = chinScreen_timer_create(gradient_transition_callback, 1200);
    
    Serial.println("Gradient animations started! Use buttons to control speed.");
}

//=============================================================================
// Example 3: Enhanced Numberpad
//=============================================================================
void example_3_numberpad() {
    Serial.println("=== Example 3: Enhanced Numberpad ===");
    
    // Stop any running timers
    chinScreen_timer_stop_all();
    
    chinScreen_clear();
    chinScreen_background_gradient("purple", "darkpurple", "horizontal");
    
    // Add decorative title
    chinScreen_text("Smart Numberpad", 20, 20, "yellow", "large");
    chinScreen_text("Enter any number (decimals OK!)", 20, 50, "white", "medium");
    
    // Create the numberpad - it handles its own callbacks internally
    chinScreen_numpad("Enter a number...", "center", "center");
    
    Serial.println("Numberpad created! Enter a number and press Enter.");
}

//=============================================================================
// Example 4: Advanced Textpad Input
//=============================================================================
void example_4_textpad() {
    Serial.println("=== Example 4: Advanced Textpad Input ===");
    
    // Stop any running timers
    chinScreen_timer_stop_all();
    
    chinScreen_clear();
    chinScreen_background_gradient("teal", "darkcyan", "horizontal");
    
    // Add instructions
    chinScreen_text("Advanced Text Input", 20, 20, "white", "large");
    chinScreen_text("Type anything! Full keyboard support.", 20, 50, "yellow", "medium");
    chinScreen_text("Press Enter or close keyboard when done.", 20, 80, "white", "small");
    
    // Create the keyboard/textpad - pure chinScreen!
    chinScreen_keyboard("Write your message here...", "bottom", "center");
    
    Serial.println("Virtual keyboard created! Start typing.");
}

//=============================================================================
// Main Menu System
//=============================================================================
void show_main_menu() {
    Serial.println("=== chinScreen Clean Examples Menu ===");
    
    // Stop any running timers
    chinScreen_timer_stop_all();
    
    chinScreen_clear();
    chinScreen_background_gradient("navy", "blue", "vertical");
    
    // Title
    chinScreen_text("chinScreen Examples", 60, 20, "white", "large");
    chinScreen_text("Choose an example:", 80, 50, "yellow", "medium");
    chinScreen_text("(No LVGL code needed!)", 90, 80, "cyan", "small");
    
    // Create menu buttons in a 2x2 grid - all pure chinScreen
    chinScreen_button("red", "white", 140, 50, "1. Transitions", 
        [](lv_event_t* e) { example_1_transitioning_backgrounds(); }, "middle", "left");
    
    chinScreen_button("orange", "white", 140, 50, "2. Gradients", 
        [](lv_event_t* e) { example_2_animated_gradients(); }, "middle", "right");
    
    chinScreen_button("purple", "white", 140, 50, "3. Numberpad", 
        [](lv_event_t* e) { example_3_numberpad(); }, "bottom", "left");
    
    chinScreen_button("green", "white", 140, 50, "4. Textpad", 
        [](lv_event_t* e) { example_4_textpad(); }, "bottom", "right");
    
    // Add some decorative elements
    chinScreen_circle("yellow", "white", 15, "top", "left");
    chinScreen_circle("cyan", "white", 15, "top", "right");
    
    Serial.println("Main menu displayed. All functions are pure chinScreen!");
}

//=============================================================================
// Setup and Loop
//=============================================================================
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("chinScreen Clean Examples Starting...");
    Serial.println("No LVGL code in main script - everything handled by chinScreen!");
    
    // Initialize the display
    init_display();
    delay(500);
    
    // Show the main menu
    show_main_menu();
    
    Serial.println("Setup complete! All timer control is now handled by chinScreen.");
}

void loop() {
    // Only chinScreen task handler needed!
    chinScreen_task_handler(); // This would be a new function you'd add
    delay(5);
    
    // Handle any serial commands for debugging
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "1") {
            example_1_transitioning_backgrounds();
        } else if (command == "2") {
            example_2_animated_gradients();
        } else if (command == "3") {
            example_3_numberpad();
        } else if (command == "4") {
            example_4_textpad();
        } else if (command == "menu") {
            show_main_menu();
        } else if (command == "stop") {
            chinScreen_timer_stop_all();
            Serial.println("All timers stopped");
        } else if (command == "help") {
            Serial.println("Commands: 1, 2, 3, 4, menu, stop, help");
        }
    }
}