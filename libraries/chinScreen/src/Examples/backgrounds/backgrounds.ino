/**
 * @file chinScreen_examples.ino
 * @brief Examples showing how to use the improved chinScreen library
 */

#define LVGL_PORT_ROTATION_DEGREE 90
#include "chinScreen.h"

// Global objects for interaction


void setup() {
    init_display();
        
    // Show keyboard
    chinScreen_keyboard("Enter your name...", "bottom", "center");
}


