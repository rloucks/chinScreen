/////////////////////////////////////////////////////////////
// Image display function - supports BMP, PNG (if decoder available)
/////////////////////////////////////////////////////////////
inline lv_obj_t* chinScreen_image(const char* filepath, int x = -1, int y = -1,
                                  const char* vAlign = "middle", 
                                  const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *img = lv_img_create(lv_scr_act());
    
    // Try to load image from SD card
    if (SD.exists(filepath)) {
        // For LVGL, we need to load the image data
        File imageFile = SD.open(filepath, FILE_READ);
        if (imageFile) {
            size_t fileSize = imageFile.size();
            uint8_t* imageData = (uint8_t*)malloc(fileSize);
            
            if (imageData) {
                imageFile.read(imageData, fileSize);
                imageFile.close();
                
                // Create LVGL image descriptor
                static lv_img_dsc_t img_dsc;
                img_dsc.header.always_zero = 0;
                img_dsc.header.w = 0; // Will be determined by decoder
                img_dsc.header.h = 0; // Will be determined by decoder
                img_dsc.data_size = fileSize;
                img_dsc.header.cf = LV_IMG_CF_RAW; // or LV_IMG_CF_TRUE_COLOR
                img_dsc.data = imageData;
                
                lv_img_set_src(img, &img_dsc);
            }
        } else {
            Serial.println("Failed to open image file");
            free(img);
            bsp_display_unlock();
            return nullptr;
        }
    } else {
        Serial.println("Image file not found on SD card");
        free(img);
        bsp_display_unlock();
        return nullptr;
    }

    // Position the image
    if (x >= 0 && y >= 0) {
        lv_obj_set_pos(img, x, y);
    } else {
        // Use alignment
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

/////////////////////////////////////////////////////////////
// Movie/Video player (using MJPEG format - most suitable for embedded)
// Note: This is a simplified implementation - full video requires more complex frame handling
/////////////////////////////////////////////////////////////
class chinScreen_Video {
private:
    File videoFile;
    lv_obj_t* videoObj;
    lv_timer_t* frameTimer;
    bool isPlaying;
    uint32_t frameDelay;
    
    static void frame_timer_cb(lv_timer_t* timer) {
        chinScreen_Video* video = (chinScreen_Video*)timer->user_data;
        video->nextFrame();
    }
    
public:
    chinScreen_Video() : videoObj(nullptr), frameTimer(nullptr), isPlaying(false), frameDelay(33) {}
    
    bool loadVideo(const char* filepath, uint32_t fps = 30) {
        if (!SD.exists(filepath)) {
            Serial.println("Video file not found");
            return false;
        }
        
        videoFile = SD.open(filepath, FILE_READ);
        if (!videoFile) {
            Serial.println("Failed to open video file");
            return false;
        }
        
        frameDelay = 1000 / fps; // Convert FPS to milliseconds
        return true;
    }
    
    void play() {
        if (!videoFile) return;
        
        bsp_display_lock(0);
        if (!videoObj) {
            videoObj = lv_img_create(lv_scr_act());
            lv_obj_center(videoObj);
        }
        
        if (!frameTimer) {
            frameTimer = lv_timer_create(frame_timer_cb, frameDelay, this);
        }
        
        isPlaying = true;
        lv_timer_resume(frameTimer);
        bsp_display_unlock();
    }
    
    void pause() {
        if (frameTimer) {
            lv_timer_pause(frameTimer);
        }
        isPlaying = false;
    }
    
    void stop() {
        if (frameTimer) {
            lv_timer_del(frameTimer);
            frameTimer = nullptr;
        }
        if (videoFile) {
            videoFile.close();
        }
        isPlaying = false;
    }
    
    void nextFrame() {
        // Simplified frame reading - you'll need to implement MJPEG parsing
        // This is a placeholder that reads raw frame data
        if (!videoFile.available()) {
            videoFile.seek(0); // Loop video
            return;
        }
        
        // Read frame size (first 4 bytes in our simple format)
        uint32_t frameSize;
        videoFile.read((uint8_t*)&frameSize, 4);
        
        if (frameSize > 0 && frameSize < 50000) { // Sanity check
            uint8_t* frameData = (uint8_t*)malloc(frameSize);
            if (frameData) {
                videoFile.read(frameData, frameSize);
                
                // Update image with new frame
                static lv_img_dsc_t frame_dsc;
                frame_dsc.data = frameData;
                frame_dsc.data_size = frameSize;
                
                bsp_display_lock(0);
                lv_img_set_src(videoObj, &frame_dsc);
                bsp_display_unlock();
                
                free(frameData);
            }
        }
    }
    
    bool isVideoPlaying() { return isPlaying; }
};

// Global video player instance
static chinScreen_Video globalVideoPlayer;

// Video control functions
inline bool chinScreen_video_load(const char* filepath, uint32_t fps = 30) {
    return globalVideoPlayer.loadVideo(filepath, fps);
}

inline void chinScreen_video_play() {
    globalVideoPlayer.play();
}

inline void chinScreen_video_pause() {
    globalVideoPlayer.pause();
}

inline void chinScreen_video_stop() {
    globalVideoPlayer.stop();
}

inline bool chinScreen_video_is_playing() {
    return globalVideoPlayer.isVideoPlaying();
}
