#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    chinScreen_init_display();  // Core display init
    chinScreen_init_shapes();   // Only load shapes module
    
    // Now you can use shape functions
    chinScreen_rectangle("red", "white", 100, 50);
}