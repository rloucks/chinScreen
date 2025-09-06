// Test code for embedded images with chinScreen library
// This shows how to use C array images (like your settings.c) with LVGL

#include "chinScreen.h"

// Include your embedded image
extern const lv_image_dsc_t settings;  // From your settings.c file

// Add this function to your chinScreen.h library
inline lv_obj_t* chinScreen_embedded_image(const lv_image_dsc_t* img_dsc, int x = -1, int y = -1,
                                          const char* vAlign = "middle", 
                                          const char* hAlign = "center") {
    if (!img_dsc) {
        Serial.println("Embedded image descriptor is null");
        return nullptr;
    }
    
    bsp_display_lock(0);
    
    lv_obj_t *img = lv_img_create(lv_scr_act());
    
    // Set the embedded image as source
    lv_img_set_src(img, img_dsc);
    
    // Position the image
    if (x >= 0 && y >= 0) {
        lv_obj_set_pos(img, x, y);
    } else {
        lv_align_t align = LV_ALIGN_CENTER;
        if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
        else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
        else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
        else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
        else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

        lv_obj_align(img, align, 0, 0);
    }

    bsp_display_unlock();
    return img;
}

// Add this for embedded image buttons too
inline lv_obj_t* chinScreen_embedded_image_button(const lv_image_dsc_t* img_dsc,
                                                 chinScreen_btn_callback_t callback,
                                                 const char* vAlign = "middle", 
                                                 const char* hAlign = "center",
                                                 int width = 80, int height = 80) {
    if (!img_dsc) {
        Serial.println("Embedded image descriptor is null - creating fallback");
        lv_obj_t* fallbackBtn = chinScreen_rectangle("blue", "white", width, height, vAlign, hAlign);
        if (callback) {
            lv_obj_add_event_cb(fallbackBtn, callback, LV_EVENT_CLICKED, NULL);
        }
        chinScreen_shape_text(fallbackBtn, "IMG", "white", "medium");
        return fallbackBtn;
    }
    
    bsp_display_lock(0);
    
    lv_obj_t* imgBtn = lv_imgbtn_create(lv_scr_act());
    
    // Set embedded image for button states
    lv_imgbtn_set_src(imgBtn, LV_IMGBTN_STATE_RELEASED, img_dsc, NULL, NULL);
    lv_imgbtn_set_src(imgBtn, LV_IMGBTN_STATE_PRESSED, img_dsc, NULL, NULL);
    
    lv_obj_set_size(imgBtn, width, height);
    
    if (callback) {
        lv_obj_add_event_cb(imgBtn, callback, LV_EVENT_CLICKED, NULL);
    }
    
    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(imgBtn, align, 0, 0);

    bsp_display_unlock();
    return imgBtn;
}

//=============================================================================
// TEST PROGRAM
//=============================================================================

void setup() {
    Serial.begin(115200);
    init_display();
    
    Serial.println("Testing embedded images...");
    
    // Dark gradient background
    chinScreen_background_gradient("navy", "purple", "vertical");
    
    // Title
    chinScreen_text("Embedded Image Test", 10, 10, "cyan", "xlarge");
    
    // Display the embedded settings icon
    lv_obj_t* settingsImg = chinScreen_embedded_image(&settings, 50, 100);
    if (settingsImg) {
        chinScreen_text("Settings Icon", 10, 50, "white", "medium");
        Serial.println("Settings icon displayed successfully!");
    } else {
        chinScreen_text("Image failed", 10, 50, "red", "medium");
        Serial.println("Failed to display settings icon");
    }
    
    // Create an image button using the embedded icon
    lv_obj_t* settingsBtn = chinScreen_embedded_image_button(&settings, settings_clicked, 
                                                           "middle", "center", 60, 60);
    
    // Some regular UI elements
    chinScreen_text("Click the settings icon!", 10, 200, "yellow", "medium");
    chinScreen_circle("orange", "white", 20, "bottom", "right");
    
    Serial.println("Test setup complete!");
}

void loop() {
    delay(10);
}

void settings_clicked(lv_event_t* e) {
    Serial.println("Settings button clicked!");
    
    chinScreen_clear();
    chinScreen_background_solid("blue");
    chinScreen_text("Settings Screen", 10, 10, "white", "xlarge");
    
    // Show the settings icon again on the new screen
    chinScreen_embedded_image(&settings, 100, 100);
    chinScreen_text("Settings Menu", 140, 120, "white", "large");
    
    // Back button
    chinScreen_button("red", "white", 100, 40, "Back", go_back, "bottom", "center");
}

void go_back(lv_event_t* e) {
    // Restart to go back to main screen (simple approach)
    ESP.restart();
}

//=============================================================================
// INSTRUCTIONS FOR CREATING MORE EMBEDDED IMAGES
//=============================================================================

/*
To create more embedded images like your settings.c:

1. Use an online converter like: 
   - https://lvgl.io/tools/imageconverter
   - Upload your PNG/BMP image
   - Choose "C array" output format
   - Choose "RGB565" color format (matches your display)
   - Download the .c file

2. Include the generated files in your project:
   - Add the .c file to your project
   - Add extern declaration in your main file:
     extern const lv_image_dsc_t your_image_name;

3. Use with chinScreen:
   chinScreen_embedded_image(&your_image_name, x, y);
   chinScreen_embedded_image_button(&your_image_name, callback);

Benefits of embedded images vs SD card:
✓ Faster loading (no file system overhead)
✓ No SD card required
✓ Images can't get corrupted or deleted
✓ Takes up program flash memory instead of SD space
✓ Perfect for small icons and UI elements

Drawbacks:
✗ Larger program size
✗ Must recompile to change images
✗ Limited by available flash memory
✗ Not suitable for large photos/videos
*/