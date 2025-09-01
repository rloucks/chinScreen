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
// chinScreen Commands Module
// Screen management, backgrounds, brightness, and core display commands
// Compatible with LVGL 9.3.0
//=============================================================================

#pragma once

// Module guard - only load if commands haven't been loaded
#ifndef CHINSCREEN_COMMANDS_H
#define CHINSCREEN_COMMANDS_H

#include <SD.h>
#include <FS.h>
#include <SPI.h>

// SD Card Pins for JC3248WS35
#define CHINSCREEN_SD_CS   10
#define CHINSCREEN_SD_MOSI 11
#define CHINSCREEN_SD_CLK  12
#define CHINSCREEN_SD_MISO 13

//=============================================================================
// SCREEN MANAGEMENT FUNCTIONS
//=============================================================================

// Clear screen with thread safety (LVGL 9.3.0 compatible)
inline void chinScreen_clear() {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_screen_active();  // LVGL 9.3.0 function name
    lv_obj_clean(scr);
    
    // Reset background to black
    lv_obj_set_style_bg_color(scr, lv_color_make(0, 0, 0), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_NONE, LV_PART_MAIN);
    lv_obj_invalidate(scr);
    
    chinScreen_unlock_display();
}

// Refresh display - forces a complete redraw
inline void chinScreen_refresh() {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_screen_active();
    lv_obj_invalidate(scr);
    lv_refr_now(NULL);  // Force immediate refresh
    
    chinScreen_unlock_display();
}

// Get screen dimensions
inline void chinScreen_get_screen_size(int* width, int* height) {
    lv_obj_t *scr = lv_screen_active();
    *width = lv_obj_get_width(scr);
    *height = lv_obj_get_height(scr);
}

//=============================================================================
// BACKGROUND FUNCTIONS
//=============================================================================

// Set solid background color
inline void chinScreen_background_solid(const char* colorName) {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(scr, LV_GRAD_DIR_NONE, LV_PART_MAIN);
    
    chinScreen_unlock_display();
}

// Set gradient background with LVGL 9.3.0 improvements
inline void chinScreen_background_gradient(const char* color1Name, 
                                          const char* color2Name, 
                                          const char* direction = "vertical") {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_screen_active();
    lv_color_t color1 = chinScreen_get_color(color1Name);
    lv_color_t color2 = chinScreen_get_color(color2Name);
    
    // LVGL 9.3.0 gradient direction constants
    lv_grad_dir_t grad_dir = LV_GRAD_DIR_VER;
    if (strcmp(direction, "horizontal") == 0) grad_dir = LV_GRAD_DIR_HOR;
    else if (strcmp(direction, "diagonal") == 0) grad_dir = LV_GRAD_DIR_HOR;  // Fallback for diagonal
    
    lv_obj_set_style_bg_color(scr, color1, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(scr, color2, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(scr, grad_dir, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    chinScreen_unlock_display();
}

// Advanced gradient with multiple stops (LVGL 9.3.0 feature)
inline void chinScreen_background_gradient_advanced(const char* color1Name, 
                                                   const char* color2Name,
                                                   const char* color3Name = nullptr,
                                                   const char* direction = "vertical") {
    chinScreen_lock_display();
    
    lv_obj_t *scr = lv_screen_active();
    
    // For now, implement as two-color gradient
    // LVGL 9.3.0 has better multi-stop gradient support we could add later
    chinScreen_unlock_display();
    chinScreen_background_gradient(color1Name, color2Name, direction);
}

//=============================================================================
// DISPLAY CONTROL FUNCTIONS
//=============================================================================

// Backlight control with improved error handling
inline bool chinScreen_backlight(bool enable) {
    try {
        if (enable) {
            bsp_display_backlight_on();
            Serial.println("chinScreen: Backlight ON");
        } else {
            bsp_display_backlight_off();
            Serial.println("chinScreen: Backlight OFF");
        }
        return true;
    } catch (...) {
        Serial.println("chinScreen: Error controlling backlight");
        return false;
    }
}

// Brightness control with range validation
inline bool chinScreen_brightness(int brightness) {
    // Validate range
    if (brightness < 0) brightness = 0;
    if (brightness > 255) brightness = 255;
    
    try {
        bsp_display_brightness_set(brightness);
        Serial.printf("chinScreen: Brightness set to %d/255\n", brightness);
        return true;
    } catch (...) {
        Serial.println("chinScreen: Error setting brightness");
        return false;
    }
}

// Get current brightness (if supported by hardware)
inline int chinScreen_get_brightness() {
    // This would need hardware support to read current brightness
    // For now, return a default value
    return 128;  // Placeholder
}

//=============================================================================
// DISPLAY ROTATION FUNCTIONS (LVGL 9.3.0)
//=============================================================================

// Set display rotation at runtime
inline void chinScreen_set_rotation(int degrees) {
    chinScreen_lock_display();
    
    lv_display_t* display = lv_display_get_default();
    
    switch (degrees) {
        case 0:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_0);
            break;
        case 90:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_90);
            break;
        case 180:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_180);
            break;
        case 270:
            lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);
            break;
        default:
            Serial.printf("chinScreen: Invalid rotation %d. Use 0, 90, 180, or 270\n", degrees);
            chinScreen_unlock_display();
            return;
    }
    
    Serial.printf("chinScreen: Display rotation set to %d degrees\n", degrees);
    chinScreen_unlock_display();
}

// Get current rotation
inline int chinScreen_get_rotation() {
    lv_display_t* display = lv_display_get_default();
    lv_display_rotation_t rotation = lv_display_get_rotation(display);
    
    switch (rotation) {
        case LV_DISPLAY_ROTATION_0:   return 0;
        case LV_DISPLAY_ROTATION_90:  return 90;
        case LV_DISPLAY_ROTATION_180: return 180;
        case LV_DISPLAY_ROTATION_270: return 270;
        default: return 0;
    }
}

//=============================================================================
// TEXT FUNCTIONS
//=============================================================================

// Simple text drawing function
inline lv_obj_t* chinScreen_text(const char* text, 
                                 int x, int y, 
                                 const char* colorName = "white", 
                                 const char* fontSize = "medium") {
    chinScreen_lock_display();

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_get_font(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    chinScreen_unlock_display();
    return label;
}

// Centered text with alignment
inline lv_obj_t* chinScreen_text_aligned(const char* text,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center",
                                        const char* colorName = "white",
                                        const char* fontSize = "medium") {
    chinScreen_lock_display();

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_get_font(fontSize), LV_PART_MAIN);
    
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(label, align, 0, 0);

    chinScreen_unlock_display();
    return label;
}

// Multi-line text with word wrapping
inline lv_obj_t* chinScreen_text_multiline(const char* text,
                                          int x, int y,
                                          int max_width,
                                          const char* colorName = "white",
                                          const char* fontSize = "medium") {
    chinScreen_lock_display();

    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, text);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(label, max_width);
    lv_obj_set_style_text_color(label, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_get_font(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    chinScreen_unlock_display();
    return label;
}

//=============================================================================
// SD CARD AND FILESYSTEM FUNCTIONS
//=============================================================================

// LVGL filesystem driver for SD card (LVGL 9.3.0 compatible)
static lv_fs_drv_t sd_fs_drv;

// SD filesystem callbacks for LVGL 9.3.0
static void* sd_fs_open(lv_fs_drv_t* drv, const char* path, lv_fs_mode_t mode) {
    File* file = new File();
    
    if (mode & LV_FS_MODE_WR) {
        *file = SD.open(path, FILE_WRITE);
    } else {
        *file = SD.open(path, FILE_READ);
    }
    
    if (*file) {
        return (void*)file;
    }
    delete file;
    return NULL;
}

static lv_fs_res_t sd_fs_close(lv_fs_drv_t* drv, void* file_p) {
    File* file = (File*)file_p;
    if (file) {
        file->close();
        delete file;
    }
    return LV_FS_RES_OK;
}

static lv_fs_res_t sd_fs_read(lv_fs_drv_t* drv, void* file_p, void* buf, uint32_t btr, uint32_t* br) {
    File* file = (File*)file_p;
    if (!file) {
        *br = 0;
        return LV_FS_RES_UNKNOWN;
    }
    *br = file->read((uint8_t*)buf, btr);
    return (*br > 0) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t sd_fs_write(lv_fs_drv_t* drv, void* file_p, const void* buf, uint32_t btw, uint32_t* bw) {
    File* file = (File*)file_p;
    if (!file) {
        *bw = 0;
        return LV_FS_RES_UNKNOWN;
    }
    *bw = file->write((const uint8_t*)buf, btw);
    return (*bw > 0) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t sd_fs_seek(lv_fs_drv_t* drv, void* file_p, uint32_t pos, lv_fs_whence_t whence) {
    File* file = (File*)file_p;
    if (!file) return LV_FS_RES_UNKNOWN;
    
    if (whence == LV_FS_SEEK_SET) {
        return file->seek(pos) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
    } else if (whence == LV_FS_SEEK_CUR) {
        return file->seek(file->position() + pos) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
    } else if (whence == LV_FS_SEEK_END) {
        return file->seek(file->size() + pos) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
    }
    return LV_FS_RES_UNKNOWN;
}

static lv_fs_res_t sd_fs_tell(lv_fs_drv_t* drv, void* file_p, uint32_t* pos_p) {
    File* file = (File*)file_p;
    if (!file) return LV_FS_RES_UNKNOWN;
    *pos_p = file->position();
    return LV_FS_RES_OK;
}

// Initialize SD card with LVGL filesystem driver (LVGL 9.3.0 compatible)
inline bool chinScreen_init_sd_card() {
    Serial.println("chinScreen: Initializing SD card with LVGL 9.3.0 filesystem...");
    
    // Initialize SPI for SD card
    SPI.begin(CHINSCREEN_SD_CLK, CHINSCREEN_SD_MISO, CHINSCREEN_SD_MOSI, CHINSCREEN_SD_CS);
    
    if (!SD.begin(CHINSCREEN_SD_CS, SPI)) {
        Serial.println("chinScreen: SD Card Mount Failed!");
        return false;
    }
    
    Serial.println("chinScreen: SD Card hardware initialized successfully.");
    
    // Register LVGL 9.3.0 filesystem driver
    lv_fs_drv_init(&sd_fs_drv);
    sd_fs_drv.letter = 'S';  // Will be accessible as S:/path/file.png
    sd_fs_drv.cache_size = 0;  // No caching for simplicity
    
    // Set callback functions
    sd_fs_drv.open_cb = sd_fs_open;
    sd_fs_drv.close_cb = sd_fs_close;
    sd_fs_drv.read_cb = sd_fs_read;
    sd_fs_drv.write_cb = sd_fs_write;
    sd_fs_drv.seek_cb = sd_fs_seek;
    sd_fs_drv.tell_cb = sd_fs_tell;
    
    lv_fs_drv_register(&sd_fs_drv);
    
    Serial.println("chinScreen: LVGL 9.3.0 filesystem driver registered (S:/)");
    
    // Create standard folders if they don't exist
    const char* folders[] = {"/icons", "/photos", "/videos", "/fonts", "/data"};
    for (const char* folder : folders) {
        if (!SD.exists(folder)) {
            if (SD.mkdir(folder)) {
                Serial.printf("chinScreen: Created folder %s\n", folder);
            } else {
                Serial.printf("chinScreen: Failed to create folder %s\n", folder);
            }
        }
    }
    
    return true;
}

// Check if SD card is available
inline bool chinScreen_sd_available() {
    return SD.exists("/");
}

// Get SD card info
inline void chinScreen_sd_info() {
    if (!chinScreen_sd_available()) {
        Serial.println("chinScreen: SD card not available");
        return;
    }
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    uint64_t usedSize = SD.usedBytes() / (1024 * 1024);
    
    Serial.println("=== chinScreen SD Card Info ===");
    Serial.printf("Card Size: %llu MB\n", cardSize);
    Serial.printf("Used Space: %llu MB\n", usedSize);
    Serial.printf("Free Space: %llu MB\n", cardSize - usedSize);
    Serial.println("==============================");
}

//=============================================================================
// PERFORMANCE AND MEMORY FUNCTIONS
//=============================================================================

// Get LVGL memory usage
inline void chinScreen_memory_info() {
    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    
    Serial.println("=== chinScreen Memory Info ===");
    Serial.printf("Total Size: %d bytes\n", mon.total_size);
    Serial.printf("Free Count: %d\n", mon.free_cnt);
    Serial.printf("Free Size: %d bytes\n", mon.free_size);
    Serial.printf("Free Biggest: %d bytes\n", mon.free_biggest_size);
    Serial.printf("Used Count: %d\n", mon.used_cnt);
    Serial.printf("Max Used: %d bytes\n", mon.max_used);
    Serial.printf("Used Percent: %d%%\n", mon.used_pct);
    Serial.printf("Fragmentation: %d%%\n", mon.frag_pct);
    Serial.println("=============================");
}

// Force garbage collection
inline void chinScreen_gc() {
    Serial.println("chinScreen: Running garbage collection...");
    lv_gc_collect();
    chinScreen_memory_info();
}

//=============================================================================
// LEGACY COMPATIBILITY FUNCTIONS
//=============================================================================

// Legacy function aliases with deprecation warnings
inline void create_ui(int debugChin) {
    chinScreen_warn_deprecated("create_ui", "chinScreen_text or custom UI functions");
    
    Serial.println("chinScreen: Legacy create_ui called");

    if (debugChin == 1) {
        chinScreen_clear();
        chinScreen_text_aligned("chinScreen Works!", "middle", "center", "white", "large");
    }
    
    if (debugChin == 2) {
        chinScreen_clear();
        
        const char* colorNames[] = {"red", "green", "blue", "yellow", "orange", "purple", "cyan", "gray"};
        
        for (int i = 0; i < 8; i++) {
            int x = (i % 4) * 85 + 10;
            int y = (i / 4) * 65 + 10;
            chinScreen_text(colorNames[i], x, y, colorNames[i], "medium");
        }
    }
}

#endif // CHINSCREEN_COMMANDS_H