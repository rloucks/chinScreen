#define CHINSCREEN_ENABLE_ANIMATED_ICONS
#define CHINSCREEN_ICON_LOADING    // Enable loading icons
//#define CHINSCREEN_ICON_GEAR 
//#define CHINSCREEN_ICON_TOUCH
#include "chinScreen.h"

void setup() {
    Serial.begin(115200);
    init_display();
    
    Serial.println("=== Icon Debug Test ===");
    
    // List what icons are available
    //chinScreen_list_available_icons();
    
    //Serial.printf("Icon count: %d\n", chinScreen_icon_count);
    chinScreen_background_solid("white");
    //lv_obj_t* loading = chinScreen_icon("loading-2", 1.0f, "middle", "left");
    lv_obj_t* gear = chinScreen_icon("gear-1", 2.0f);
    //lv_obj_t* touch = chinScreen_icon("touch", 1.0f, "middle", "right");


}

void loop() {
  //the void
  delay(100);
}