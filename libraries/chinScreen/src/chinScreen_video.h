//       _     _       __                        
//   ___| |__ (_)_ __ / _\ ___ _ __ ___  ___ _ __   
//  / __| '_ \| | '_ \\ \ / __| '__/ _ \/ _ \ '_ \ 
// | (__| | | | | | | |\ \ (__| | |  __/  __/ | | |
//  \___|_| |_|_|_| |_\__/\___|_|  \___|\___|_| |_| 
//
//   Modular library for the JC3248W525EN using LVGL 9.3.0
//   Improved architecture with optional module loading
// 
//   Author: Richard Loucks
//   Version: 0.1.1
//   Updated: Sept 2025 - Modular Architecture
//   For updates, see git commits
//   https://github.com/rloucks/chinScreen
//=============================================================================
// chinScreen Video Module
// Simple video playback using LVGL 9.3.0 built-in video support
// Basic launch and close functionality only
//=============================================================================

#pragma once

// Module guard - only load if video hasn't been loaded
#ifndef CHINSCREEN_VIDEO_H
#define CHINSCREEN_VIDEO_H

#include <SD.h>
#include <FS.h>

//=============================================================================
// VIDEO PLAYBACK FUNCTIONS
//=============================================================================

// Global video object tracking
static lv_obj_t* chinScreen_active_video = nullptr;
static lv_obj_t* chinScreen_video_container = nullptr;

// Video close callback
static void chinScreen_video_close_cb(lv_event_t* e) {
    if (chinScreen_video_container) {
        chinScreen_lock_display();
        lv_obj_delete(chinScreen_video_container);
        chinScreen_video_container = nullptr;
        chinScreen_active_video = nullptr;
        chinScreen_unlock_display();
        Serial.println("chinScreen: Video closed");
    }
}

// Launch video playback using LVGL 9.3.0 video widget
inline lv_obj_t* chinScreen_video_play(const char* video_path,
                                      int width = 320,
                                      int height = 240,
                                      const char* vAlign = "middle",
                                      const char* hAlign = "center") {
    
    // Check if video file exists
    if (!SD.exists(video_path)) {
        Serial.printf("chinScreen: Video file not found: %s\n", video_path);
        return nullptr;
    }
    
    // Close any existing video
    if (chinScreen_video_container) {
        chinScreen_video_close();
    }
    
    chinScreen_lock_display();
    
    // Create container for video and close button
    chinScreen_video_container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(chinScreen_video_container, width + 20, height + 60);
    lv_obj_set_style_bg_color(chinScreen_video_container, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_border_color(chinScreen_video_container, lv_color_make(64, 64, 64), LV_PART_MAIN);
    lv_obj_set_style_border_width(chinScreen_video_container, 2, LV_PART_MAIN);
    lv_obj_set_style_radius(chinScreen_video_container, 8, LV_PART_MAIN);
    
    // Position container
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(chinScreen_video_container, align, 0, 0);
    
    // Create video object using LVGL 9.3.0 video widget
    chinScreen_active_video = lv_video_create(chinScreen_video_container);
    lv_obj_set_size(chinScreen_active_video, width, height);
    lv_obj_align(chinScreen_active_video, LV_ALIGN_TOP_MID, 0, 10);
    
    // Convert path to LVGL filesystem format
    char lvgl_path[128];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", video_path);
    
    // Set video source and start playback
    lv_video_set_src(chinScreen_active_video, lvgl_path);
    lv_video_set_auto_play(chinScreen_active_video, true);
    
    // Create close button
    lv_obj_t* close_btn = lv_button_create(chinScreen_video_container);
    lv_obj_set_size(close_btn, 60, 30);
    lv_obj_align(close_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    lv_obj_add_event_cb(close_btn, chinScreen_video_close_cb, LV_EVENT_CLICKED, NULL);
    
    // Close button label
    lv_obj_t* close_label = lv_label_create(close_btn);
    lv_label_set_text(close_label, "Close");
    lv_obj_set_style_text_color(close_label, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_center(close_label);
    
    // Style close button
    lv_obj_set_style_bg_color(close_btn, lv_color_make(200, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_radius(close_btn, 4, LV_PART_MAIN);
    
    chinScreen_unlock_display();
    
    Serial.printf("chinScreen: Video launched: %s\n", video_path);
    return chinScreen_active_video;
}

// Close active video
inline void chinScreen_video_close() {
    if (chinScreen_video_container) {
        chinScreen_video_close_cb(nullptr);
    }
}

// Check if video is currently playing
inline bool chinScreen_video_is_playing() {
    if (chinScreen_active_video) {
        return lv_video_get_state(chinScreen_active_video) == LV_VIDEO_STATE_PLAYING;
    }
    return false;
}

// Simple video control functions using LVGL 9.3.0 video API
inline void chinScreen_video_pause() {
    if (chinScreen_active_video) {
        lv_video_pause(chinScreen_active_video);
        Serial.println("chinScreen: Video paused");
    }
}

inline void chinScreen_video_resume() {
    if (chinScreen_active_video) {
        lv_video_play(chinScreen_active_video);
        Serial.println("chinScreen: Video resumed");
    }
}

inline void chinScreen_video_stop() {
    if (chinScreen_active_video) {
        lv_video_stop(chinScreen_active_video);
        Serial.println("chinScreen: Video stopped");
    }
}

//=============================================================================
// VIDEO UTILITY FUNCTIONS
//=============================================================================

// Get video file info
inline void chinScreen_video_info(const char* video_path) {
    Serial.println("=== chinScreen Video Info ===");
    Serial.printf("File: %s\n", video_path);
    Serial.printf("Exists: %s\n", SD.exists(video_path) ? "YES" : "NO");
    
    if (SD.exists(video_path)) {
        File file = SD.open(video_path);
        Serial.printf("Size: %lu bytes\n", file.size());
        file.close();
    }
    Serial.println("============================");
}

// Check if video module is ready
inline bool chinScreen_video_ready() {
    return SD.exists("/") && chinScreen_sd_available();
}

//=============================================================================
// LEGACY COMPATIBILITY
//=============================================================================

// Legacy function aliases with deprecation warnings
inline bool chinScreen_video_load(const char* filepath, uint32_t fps = 30) {
    chinScreen_warn_deprecated("chinScreen_video_load", "chinScreen_video_play");
    lv_obj_t* result = chinScreen_video_play(filepath);
    return (result != nullptr);
}

inline void chinScreen_video_play_legacy() {
    chinScreen_warn_deprecated("chinScreen_video_play (legacy)", "chinScreen_video_resume");
    chinScreen_video_resume();
}

inline bool chinScreen_video_is_playing_legacy() {
    chinScreen_warn_deprecated("chinScreen_video_is_playing (legacy)", "chinScreen_video_is_playing");
    return chinScreen_video_is_playing();
}

#endif // CHINSCREEN_VIDEO_H