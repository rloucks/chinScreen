// Diagnostic test to figure out why icons show as colored bars
// Let's debug what's happening with the icon loading

#define CHINSCREEN_ENABLE_ICONS
#define CHINSCREEN_ICON_HOME
#define CHINSCREEN_ICON_SETTINGS
#define CHINSCREEN_ICON_BACK
#define CHINSCREEN_ICON_PLAY
#define CHINSCREEN_ICON_USER
#define CHINSCREEN_ICON_MENU

#define LVGL_PORT_ROTATION_DEGREE 90
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for serial to initialize
    Serial.println("Starting chinScreen Icon Diagnostic Test...");
    
    init_display();
    
    // Run diagnostic tests

    
    Serial.println("Diagnostic complete!");
}

void loop() {
    test();
    delay(5);
}

void test() {
     // Test 4: Try with color
    lv_obj_t* coloredIcon = chinScreen_icon("settings", 1.0f, "middle", "left", "red");
    
}