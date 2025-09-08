#include "chinScreen.h"
#include "sonic.c"
// Bring in your generated Sonic image from sonic.c
//extern const unsigned char sonic_gif[];
//extern const unsigned int sonic_gif_len;


void setup() {
    Serial.begin(115200);
    init_display();

    Serial.println("Testing Sonic embedded image...");

    // Background
    chinScreen_background_gradient("black", "blue", "vertical");

    // Title
    chinScreen_text("Sonic Test", 10, 10, "cyan", "xlarge");

 // Create a GIF object
lv_obj_t* sonicGif = lv_gif_create(lv_scr_act());

// Use the raw array as the source
lv_gif_set_src(sonicGif, sonic_gif);

// Center it (or set x/y)
lv_obj_center(sonicGif);


}

void loop() {
    delay(10);  // let LVGL handle events
}
