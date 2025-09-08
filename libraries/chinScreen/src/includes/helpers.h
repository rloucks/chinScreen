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

// Replace the existing timer functions in your helpers.h with these corrected versions

/////////////////////////////////////////////////////////////
// Timer Management for chinScreen - LVGL 8.3.11 Compatible
/////////////////////////////////////////////////////////////

// Timer handle type (hides LVGL implementation)
typedef void* chinScreen_timer_t;

// Timer callback type - simplified interface
typedef void (*chinScreen_timer_callback_t)(void* user_data);

// Timer control structure (internal) - UPDATED VERSION
struct chinScreen_timer_info {
    lv_timer_t* lvgl_timer;
    chinScreen_timer_callback_t user_callback;
    void* user_data;
    uint32_t period_ms;  // Store period since LVGL 8.3.11 doesn't have get_period
    bool is_paused;      // Track pause state since lv_timer_is_paused doesn't exist
    bool is_active;
};

// Global timer storage (adjust MAX_TIMERS as needed)
#define MAX_CHINSCREEN_TIMERS 10
static chinScreen_timer_info timer_pool[MAX_CHINSCREEN_TIMERS];
static bool timer_pool_initialized = false;

// Internal LVGL timer callback that calls user callback
static void chinScreen_internal_timer_cb(lv_timer_t* timer) {
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer->user_data;
    if (timer_info && timer_info->is_active && timer_info->user_callback && !timer_info->is_paused) {
        timer_info->user_callback(timer_info->user_data);
    }
}

// Initialize timer pool
static void chinScreen_timer_pool_init() {
    if (timer_pool_initialized) return;
    
    for (int i = 0; i < MAX_CHINSCREEN_TIMERS; i++) {
        timer_pool[i].lvgl_timer = nullptr;
        timer_pool[i].user_callback = nullptr;
        timer_pool[i].user_data = nullptr;
        timer_pool[i].period_ms = 0;
        timer_pool[i].is_paused = false;
        timer_pool[i].is_active = false;
    }
    timer_pool_initialized = true;
}

/////////////////////////////////////////////////////////////
// Function: Create a timer
/////////////////////////////////////////////////////////////
inline chinScreen_timer_t chinScreen_timer_create(chinScreen_timer_callback_t callback, 
                                                  uint32_t period_ms, 
                                                  void* user_data = nullptr) {
    chinScreen_timer_pool_init();
    
    // Find available slot
    for (int i = 0; i < MAX_CHINSCREEN_TIMERS; i++) {
        if (!timer_pool[i].is_active) {
            chinScreen_timer_info* timer_info = &timer_pool[i];
            
            bsp_display_lock(0);
            timer_info->lvgl_timer = lv_timer_create(chinScreen_internal_timer_cb, period_ms, timer_info);
            bsp_display_unlock();
            
            if (timer_info->lvgl_timer) {
                timer_info->user_callback = callback;
                timer_info->user_data = user_data;
                timer_info->period_ms = period_ms;
                timer_info->is_paused = false;
                timer_info->is_active = true;
                
                Serial.printf("Timer created with %dms period\n", period_ms);
                return (chinScreen_timer_t)timer_info;
            }
            break;
        }
    }
    
    Serial.println("Error: No available timer slots");
    return nullptr;
}

/////////////////////////////////////////////////////////////
// Function: Delete a timer
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_delete(chinScreen_timer_t timer) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        bsp_display_lock(0);
        lv_timer_del(timer_info->lvgl_timer);
        bsp_display_unlock();
        
        timer_info->lvgl_timer = nullptr;
        timer_info->user_callback = nullptr;
        timer_info->user_data = nullptr;
        timer_info->period_ms = 0;
        timer_info->is_paused = false;
        timer_info->is_active = false;
        
        Serial.println("Timer deleted");
    }
}

/////////////////////////////////////////////////////////////
// Function: Change timer speed/period
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_set_period(chinScreen_timer_t timer, uint32_t new_period_ms) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        bsp_display_lock(0);
        lv_timer_set_period(timer_info->lvgl_timer, new_period_ms);
        bsp_display_unlock();
        
        timer_info->period_ms = new_period_ms;
        Serial.printf("Timer period changed to %dms\n", new_period_ms);
    }
}

/////////////////////////////////////////////////////////////
// Function: Get current timer period - FIXED
/////////////////////////////////////////////////////////////
inline uint32_t chinScreen_timer_get_period(chinScreen_timer_t timer) {
    if (!timer) return 0;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active) {
        return timer_info->period_ms;  // Return stored period
    }
    
    return 0;
}

/////////////////////////////////////////////////////////////
// Function: Pause timer
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_pause(chinScreen_timer_t timer) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        bsp_display_lock(0);
        lv_timer_pause(timer_info->lvgl_timer);
        bsp_display_unlock();
        
        timer_info->is_paused = true;
        Serial.println("Timer paused");
    }
}

/////////////////////////////////////////////////////////////
// Function: Resume timer
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_resume(chinScreen_timer_t timer) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        bsp_display_lock(0);
        lv_timer_resume(timer_info->lvgl_timer);
        bsp_display_unlock();
        
        timer_info->is_paused = false;
        Serial.println("Timer resumed");
    }
}

/////////////////////////////////////////////////////////////
// Function: Check if timer is running - FIXED
/////////////////////////////////////////////////////////////
inline bool chinScreen_timer_is_running(chinScreen_timer_t timer) {
    if (!timer) return false;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        return !timer_info->is_paused;  // Use stored pause state
    }
    
    return false;
}

/////////////////////////////////////////////////////////////
// Function: Speed up timer (decrease period) - FIXED
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_speed_up(chinScreen_timer_t timer, uint32_t decrease_ms = 200) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        uint32_t current_period = timer_info->period_ms;  // Use stored period
        if (current_period > decrease_ms) {
            uint32_t new_period = current_period - decrease_ms;
            chinScreen_timer_set_period(timer, new_period);
            Serial.printf("Timer sped up to %dms\n", new_period);
        } else {
            Serial.println("Timer already at minimum speed");
        }
    }
}

/////////////////////////////////////////////////////////////
// Function: Slow down timer (increase period) - FIXED
/////////////////////////////////////////////////////////////
inline void chinScreen_timer_slow_down(chinScreen_timer_t timer, uint32_t increase_ms = 200) {
    if (!timer) return;
    
    chinScreen_timer_info* timer_info = (chinScreen_timer_info*)timer;
    
    if (timer_info->is_active && timer_info->lvgl_timer) {
        uint32_t current_period = timer_info->period_ms;  // Use stored period
        uint32_t new_period = current_period + increase_ms;
        chinScreen_timer_set_period(timer, new_period);
        Serial.printf("Timer slowed down to %dms\n", new_period);
    }
}

/////////////////////////////////////////////////////////////
// Convenience Functions for Common Timer Operations
/////////////////////////////////////////////////////////////

// Create a repeating timer that calls a function every X milliseconds
inline chinScreen_timer_t chinScreen_repeat_every(uint32_t milliseconds, 
                                                  chinScreen_timer_callback_t callback,
                                                  void* user_data = nullptr) {
    return chinScreen_timer_create(callback, milliseconds, user_data);
}

// Create a one-shot timer that calls a function after X milliseconds
inline chinScreen_timer_t chinScreen_call_after(uint32_t milliseconds, 
                                                chinScreen_timer_callback_t callback,
                                                void* user_data = nullptr) {
    // Note: This would need additional logic to auto-delete after first call
    // For now, it's the same as repeat_every
    return chinScreen_timer_create(callback, milliseconds, user_data);
}

// Stop all timers (useful for cleanup)
inline void chinScreen_timer_stop_all() {
    chinScreen_timer_pool_init();
    
    for (int i = 0; i < MAX_CHINSCREEN_TIMERS; i++) {
        if (timer_pool[i].is_active) {
            chinScreen_timer_delete((chinScreen_timer_t)&timer_pool[i]);
        }
    }
    
    Serial.println("All timers stopped");
}

/////////////////////////////////////////////////////////////
// Function: chinScreen task handler - hides LVGL from users
/////////////////////////////////////////////////////////////
inline void chinScreen_task_handler() {
    lv_task_handler();
}