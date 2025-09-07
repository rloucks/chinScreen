// Animation types
typedef enum {
    ANIM_MOVE_X,
    ANIM_MOVE_Y,
    ANIM_SCALE,
    ANIM_ROTATE,
    ANIM_FADE
} chinScreen_anim_type_t;



/////////////////////////////////////////////////////////////!!!!!!!!!!!!!!!!
// Helper function to initialize SD card (call this in setup)
/////////////////////////////////////////////////////////////!!!!!!!!!!!!!!!!
inline bool chinScreen_init_sd_card(int cs_pin = 5) {
    if (!SD.begin(cs_pin)) {
        Serial.println("SD Card initialization failed!");
        return false;
    }
    
    // Create fonts directory if it doesn't exist
    if (!SD.exists("/fonts")) {
        SD.mkdir("/fonts");
    }
    
    Serial.println("SD Card initialized successfully");
    return true;
}

/////////////////////////////////////////////////////////////
// Function to preload images into memory for better performance
/////////////////////////////////////////////////////////////
static lv_img_dsc_t* preloaded_images[10] = {nullptr};
static int preloaded_count = 0;

inline int chinScreen_preload_image(const char* filepath) {
    if (preloaded_count >= 10) {
        Serial.println("Maximum preloaded images reached");
        return -1;
    }
    
    if (!SD.exists(filepath)) {
        Serial.println("Image file not found for preloading");
        return -1;
    }
    
    File imageFile = SD.open(filepath, FILE_READ);
    if (!imageFile) return -1;
    
    size_t fileSize = imageFile.size();
    uint8_t* imageData = (uint8_t*)malloc(fileSize);
    
    if (!imageData) {
        imageFile.close();
        return -1;
    }
    
    imageFile.read(imageData, fileSize);
    imageFile.close();
    
    lv_img_dsc_t* img_dsc = (lv_img_dsc_t*)malloc(sizeof(lv_img_dsc_t));
    img_dsc->data = imageData;
    img_dsc->data_size = fileSize;
    img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR;
    
    preloaded_images[preloaded_count] = img_dsc;
    return preloaded_count++;
}

inline lv_obj_t* chinScreen_show_preloaded_image(int imageIndex, 
                                                 const char* vAlign = "middle", 
                                                 const char* hAlign = "center") {
    if (imageIndex < 0 || imageIndex >= preloaded_count || !preloaded_images[imageIndex]) {
        return nullptr;
    }
    
    bsp_display_lock(0);
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, preloaded_images[imageIndex]);
    
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

    lv_obj_align(img, align, 0, 0);
    bsp_display_unlock();
    return img;
}