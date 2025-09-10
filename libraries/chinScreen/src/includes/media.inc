//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Improved library for the JC3248W525EN using LVGL based on 
//   too many attempts on the internet. This should be easy to useful
// 
//   Author: Richard Loucks
//   Inital Release: Sept 2025
//   For updates, see git commits
//   https://github.com/rloucks/chinScreen
//
//   Version: 0.0.2b
//
//   Hardware based for JC3248W525EN touch screen ESP32-S3 Panel
//
//   File: media.h
//   Purpose: image and movie functions
//   Required: NO


// Add PNG support function
inline lv_obj_t* chinScreen_png(const unsigned char* png_data, uint32_t png_size,
                               const char* vAlign = "middle", 
                               const char* hAlign = "center") {
    bsp_display_lock(0);

    // Create image descriptor for PNG
    static lv_img_dsc_t png_dsc;
    memset(&png_dsc, 0, sizeof(lv_img_dsc_t));
    png_dsc.header.cf = LV_IMG_CF_RAW;
    png_dsc.data = png_data;
    png_dsc.data_size = png_size;

    lv_obj_t *img = lv_img_create(lv_scr_act());
    if (img) {
        lv_img_set_src(img, &png_dsc);
        
        // Handle alignment (same as before)
        lv_align_t align = LV_ALIGN_CENTER;
        if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
        // ... etc (same alignment code as your GIF function)
        
        lv_obj_align(img, align, 0, 0);
    }

    bsp_display_unlock();
    return img;
}


lv_obj_t* chinScreen_gif(const unsigned char* gif_data, uint32_t gif_size) {
    bsp_display_lock(0);

    // Initialize structure to zero first
    static lv_img_dsc_t gif_dsc;
    memset(&gif_dsc, 0, sizeof(lv_img_dsc_t));
    
    // Set the fields we need
    gif_dsc.data = gif_data;
    gif_dsc.data_size = gif_size;

    lv_obj_t *img = lv_gif_create(lv_scr_act());
    if (img) {
        lv_gif_set_src(img, &gif_dsc);
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    }

    bsp_display_unlock();
    return img;
}

void chinScreen_gif_zoom(lv_obj_t* gif_obj, float zoom_factor) {
    if (!gif_obj) return;
    
    bsp_display_lock(0);
    
    // Convert zoom factor to LVGL's scale format (256 = 100%)
    int16_t zoom_lvgl = (int16_t)(zoom_factor * 256);
    
    lv_obj_set_style_transform_zoom(gif_obj, zoom_lvgl, LV_PART_MAIN);
    
    bsp_display_unlock();
}

// Usage examples:
// chinScreen_gif_zoom(myGif, 0.5f);   // 50% size (shrink)
// chinScreen_gif_zoom(myGif, 1.0f);   // 100% size (normal)
// chinScreen_gif_zoom(myGif, 2.0f);   // 200% size (grow)
// chinScreen_gif_zoom(myGif, 1.5f);   // 150% size

// Smoothly animate zoom changes
void chinScreen_gif_animate_zoom(lv_obj_t* gif_obj, float from_zoom, float to_zoom, 
                                 uint32_t duration_ms = 500) {
    if (!gif_obj) return;
    
    bsp_display_lock(0);
    
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, gif_obj);
    lv_anim_set_time(&anim, duration_ms);
    lv_anim_set_values(&anim, (int32_t)(from_zoom * 256), (int32_t)(to_zoom * 256));
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_zoom);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);
    lv_anim_start(&anim);
    
    bsp_display_unlock();
}

// Usage:
// chinScreen_gif_animate_zoom(myGif, 1.0f, 2.0f, 1000); // Grow over 1 second
// chinScreen_gif_animate_zoom(myGif, 2.0f, 0.5f, 500);  // Shrink over 0.5 seconds


/////////////////////////////////////////////////////////////
// Image/GIF display function


inline lv_obj_t* chinScreen_image(const void* src,
                                  bool isGif = false,
                                  int x = -1, int y = -1,
                                  const char* vAlign = "middle",
                                  const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *img;

    if (isGif) {
        // GIF from embedded uint8_t[]
        img = lv_gif_create(lv_scr_act());
        lv_gif_set_src(img, src);
    } else {
        // Static image (lv_img_dsc_t*)
        img = lv_img_create(lv_scr_act());
        lv_img_set_src(img, src);
    }

    // Position the object
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


inline lv_obj_t* chinScreen_image_local(const lv_img_dsc_t *img_dsc,
                                        int x = -1, int y = -1,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, img_dsc);

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
