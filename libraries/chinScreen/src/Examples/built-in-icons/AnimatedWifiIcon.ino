#define CHINSCREEN_ENABLE_ICONS
#define CHINSCREEN_ICON_WIFI

#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    init_display();
    
    chinScreen_background_solid("black");
}

void loop() {
    // WiFi Low
    chinScreen_icon("wifi_low", 0.2f, nullptr, nullptr, "red", 17, 2);
    delay(1000);
    
    // Clear screen
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    // WiFi Medium  
    chinScreen_icon("wifi_medium", 0.2f, nullptr, nullptr, "yellow", 17, 2);
    delay(1000);
    
    // Clear screen
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    // WiFi High
    chinScreen_icon("wifi_high", 0.2f, nullptr, nullptr, "green", 17, 2);
    delay(1000);
    
    // Clear screen for next cycle
    chinScreen_clear();
    chinScreen_background_solid("black");
    
    lv_timer_handler();
    delay(5);
}