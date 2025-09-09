#define CHINSCREEN_ENABLE_MEDIA
#include "chinScreen.h"
#include "test_img.c"  // Your converted file

lv_obj_t* testGif = nullptr;
float current_zoom = 1.0f;

void setup() {
    Serial.begin(115200);
    
    Serial.printf("Free heap at startup: %d bytes\n", ESP.getFreeHeap());
    
    init_display();
    
    Serial.printf("Free heap after display init: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Test GIF size: %d bytes (%.1f KB)\n", test_image_gif_len, test_image_gif_len/1024.0);
    
    // Check GIF header
    if (test_image_gif_len > 6) {
        Serial.printf("GIF header: %c%c%c%c%c%c\n", 
                     test_image_gif[0], test_image_gif[1], test_image_gif[2], 
                     test_image_gif[3], test_image_gif[4], test_image_gif[5]);
    }

    // Background
    chinScreen_background_solid("white");
    chinScreen_text("GIF Decoding Works!", 10, 10, "black", "xlarge");

    Serial.printf("Free heap before GIF: %d bytes\n", ESP.getFreeHeap());
    
    // Try to create the GIF
    testGif = chinScreen_gif(test_image_gif, test_image_gif_len);
        //chinScreen_gif_zoom(testGif, 0.5f);
    if (testGif) {
        Serial.println("✅ Test GIF created successfully!");
        Serial.printf("Free heap after GIF: %d bytes\n", ESP.getFreeHeap());
    } else {
        Serial.println("❌ Test GIF creation failed!");
        Serial.printf("Free heap after failed attempt: %d bytes\n", ESP.getFreeHeap());
    }
}

void loop() {
    delay(10);
}