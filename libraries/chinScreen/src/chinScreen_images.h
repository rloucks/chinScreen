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
// chinScreen Images Module
// PNG, JPG, BMP, GIF support using LVGL 9.3.0 image decoders
// Image buttons, galleries, and advanced image processing
//=============================================================================

#pragma once

// Module guard - only load if images haven't been loaded
#ifndef CHINSCREEN_IMAGES_H
#define CHINSCREEN_IMAGES_H

#include <SD.h>
#include <FS.h>

//=============================================================================
// IMAGE FORMAT SUPPORT AND DECODERS
//=============================================================================

// Image format detection
typedef enum {
    CHINSCREEN_IMG_UNKNOWN = 0,
    CHINSCREEN_IMG_PNG,
    CHINSCREEN_IMG_JPG,
    CHINSCREEN_IMG_BMP,
    CHINSCREEN_IMG_GIF,
    CHINSCREEN_IMG_RAW
} chinScreen_img_format_t;

// Image configuration structure
typedef struct {
    bool auto_size;
    bool maintain_aspect;
    lv_img_size_mode_t size_mode;
    lv_opa_t opacity;
    int rotation_angle;
    bool clickable;
    chinScreen_btn_callback_t click_callback;
} chinScreen_img_config_t;

// Default image configuration
static const chinScreen_img_config_t chinScreen_default_img_config = {
    .auto_size = true,
    .maintain_aspect = true,
    .size_mode = LV_IMG_SIZE_MODE_VIRTUAL,
    .opacity = LV_OPA_COVER,
    .rotation_angle = 0,
    .clickable = false,
    .click_callback = nullptr
};

// Detect image format from file header
inline chinScreen_img_format_t chinScreen_detect_image_format(const char* filepath) {
    if (!SD.exists(filepath)) {
        Serial.printf("chinScreen: Image file not found: %s\n", filepath);
        return CHINSCREEN_IMG_UNKNOWN;
    }
    
    File file = SD.open(filepath, FILE_READ);
    if (!file) return CHINSCREEN_IMG_UNKNOWN;
    
    uint8_t header[8];
    size_t bytes_read = file.read(header, 8);
    file.close();
    
    if (bytes_read < 4) return CHINSCREEN_IMG_UNKNOWN;
    
    // PNG signature: 89 50 4E 47 0D 0A 1A 0A
    if (header[0] == 0x89 && header[1] == 0x50 && header[2] == 0x4E && header[3] == 0x47) {
        return CHINSCREEN_IMG_PNG;
    }
    
    // JPEG signature: FF D8 FF
    if (header[0] == 0xFF && header[1] == 0xD8 && header[2] == 0xFF) {
        return CHINSCREEN_IMG_JPG;
    }
    
    // BMP signature: 42 4D (BM)
    if (header[0] == 0x42 && header[1] == 0x4D) {
        return CHINSCREEN_IMG_BMP;
    }
    
    // GIF signature: 47 49 46 38 (GIF8)
    if (header[0] == 0x47 && header[1] == 0x49 && header[2] == 0x46 && header[3] == 0x38) {
        return CHINSCREEN_IMG_GIF;
    }
    
    return CHINSCREEN_IMG_UNKNOWN;
}

// Get image format name as string
inline const char* chinScreen_get_format_name(chinScreen_img_format_t format) {
    switch (format) {
        case CHINSCREEN_IMG_PNG: return "PNG";
        case CHINSCREEN_IMG_JPG: return "JPEG";
        case CHINSCREEN_IMG_BMP: return "BMP";
        case CHINSCREEN_IMG_GIF: return "GIF";
        case CHINSCREEN_IMG_RAW: return "RAW";
        default: return "UNKNOWN";
    }
}

//=============================================================================
// BASIC IMAGE DISPLAY FUNCTIONS
//=============================================================================

// Enhanced image display with full configuration
inline lv_obj_t* chinScreen_image_advanced(const char* filepath,
                                          int x = -1, int y = -1,
                                          int width = -1, int height = -1,
                                          const chinScreen_img_config_t* config = nullptr,
                                          const char* vAlign = "middle",
                                          const char* hAlign = "center") {
    
    // Verify file exists and detect format
    chinScreen_img_format_t format = chinScreen_detect_image_format(filepath);
    if (format == CHINSCREEN_IMG_UNKNOWN) {
        Serial.printf("chinScreen: Unsupported or missing image: %s\n", filepath);
        return nullptr;
    }
    
    Serial.printf("chinScreen: Loading %s image: %s\n", 
                  chinScreen_get_format_name(format), filepath);
    
    chinScreen_lock_display();
    
    lv_obj_t *img = lv_image_create(lv_screen_active());
    
    // Convert path to LVGL filesystem format
    char lvgl_path[128];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", filepath);
    
    // Set image source - LVGL 9.3.0 will handle decoding automatically
    lv_image_set_src(img, lvgl_path);
    
    // Apply configuration
    const chinScreen_img_config_t* img_config = config ? config : &chinScreen_default_img_config;
    
    // Set size and scaling
    if (width > 0 && height > 0) {
        lv_obj_set_size(img, width, height);
        if (img_config->maintain_aspect) {
            lv_image_set_size_mode(img, LV_IMG_SIZE_MODE_REAL);
        } else {
            lv_image_set_size_mode(img, img_config->size_mode);
        }
    } else if (img_config->auto_size) {
        lv_image_set_size_mode(img, LV_IMG_SIZE_MODE_VIRTUAL);
    }
    
    // Apply opacity
    lv_obj_set_style_img_opa(img, img_config->opacity, LV_PART_MAIN);
    
    // Apply rotation (LVGL 9.3.0 supports rotation)
    if (img_config->rotation_angle != 0) {
        lv_image_set_rotation(img, img_config->rotation_angle * 10); // LVGL uses 0.1 degree units
    }
    
    // Make clickable if requested
    if (img_config->clickable) {
        lv_obj_add_flag(img, LV_OBJ_FLAG_CLICKABLE);
        if (img_config->click_callback) {
            lv_obj_add_event_cb(img, img_config->click_callback, LV_EVENT_CLICKED, NULL);
        }
    }
    
    // Position the image
    if (x >= 0 && y >= 0) {
        lv_obj_set_pos(img, x, y);
    } else {
        lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
        lv_obj_align(img, align, 0, 0);
    }
    
    chinScreen_unlock_display();
    return img;
}

// Simple image display (backward compatibility)
inline lv_obj_t* chinScreen_image(const char* filepath,
                                 int x = -1, int y = -1,
                                 const char* vAlign = "middle",
                                 const char* hAlign = "center") {
    return chinScreen_image_advanced(filepath, x, y, -1, -1, nullptr, vAlign, hAlign);
}

// Scaled image with specific dimensions
inline lv_obj_t* chinScreen_image_scaled(const char* filepath,
                                        int width, int height,
                                        bool maintain_aspect = true,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center") {
    chinScreen_img_config_t config = chinScreen_default_img_config;
    config.maintain_aspect = maintain_aspect;
    config.auto_size = false;
    
    return chinScreen_image_advanced(filepath, -1, -1, width, height, &config, vAlign, hAlign);
}

// Rotated image
inline lv_obj_t* chinScreen_image_rotated(const char* filepath,
                                         int rotation_degrees,
                                         const char* vAlign = "middle",
                                         const char* hAlign = "center") {
    chinScreen_img_config_t config = chinScreen_default_img_config;
    config.rotation_angle = rotation_degrees;
    
    return chinScreen_image_advanced(filepath, -1, -1, -1, -1, &config, vAlign, hAlign);
}

// Transparent/faded image
inline lv_obj_t* chinScreen_image_transparent(const char* filepath,
                                             int opacity_percent,
                                             const char* vAlign = "middle",
                                             const char* hAlign = "center") {
    chinScreen_img_config_t config = chinScreen_default_img_config;
    config.opacity = (opacity_percent * 255) / 100;
    
    return chinScreen_image_advanced(filepath, -1, -1, -1, -1, &config, vAlign, hAlign);
}

//=============================================================================
// INTERACTIVE IMAGE BUTTONS
//=============================================================================

// Enhanced image button with multiple states
inline lv_obj_t* chinScreen_image_button_advanced(const char* normal_image,
                                                 const char* pressed_image = nullptr,
                                                 const char* disabled_image = nullptr,
                                                 chinScreen_btn_callback_t callback = nullptr,
                                                 const char* vAlign = "middle",
                                                 const char* hAlign = "center",
                                                 int width = -1,
                                                 int height = -1) {
    
    // Verify normal image exists
    if (chinScreen_detect_image_format(normal_image) == CHINSCREEN_IMG_UNKNOWN) {
        Serial.printf("chinScreen: Image button normal state not found: %s\n", normal_image);
        // Create fallback button
        return chinScreen_button("IMG", callback, vAlign, hAlign, width > 0 ? width : 80, height > 0 ? height : 80);
    }
    
    chinScreen_lock_display();
    
    lv_obj_t* imgbtn = lv_imagebutton_create(lv_screen_active());
    
    // Convert paths to LVGL filesystem format and set sources
    char normal_path[128], pressed_path[128], disabled_path[128];
    snprintf(normal_path, sizeof(normal_path), "S:%s", normal_image);
    
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, normal_path, NULL);
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, normal_path, NULL);
    
    // Set pressed state image if provided
    if (pressed_image && chinScreen_detect_image_format(pressed_image) != CHINSCREEN_IMG_UNKNOWN) {
        snprintf(pressed_path, sizeof(pressed_path), "S:%s", pressed_image);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, pressed_path, NULL);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, pressed_path, NULL);
    } else {
        // Use normal image for pressed state if no pressed image provided
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, normal_path, NULL);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, normal_path, NULL);
    }
    
    // Set disabled state image if provided
    if (disabled_image && chinScreen_detect_image_format(disabled_image) != CHINSCREEN_IMG_UNKNOWN) {
        snprintf(disabled_path, sizeof(disabled_path), "S:%s", disabled_image);
        lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_DISABLED, NULL, disabled_path, NULL);
    }
    
    // Set size if specified
    if (width > 0 && height > 0) {
        lv_obj_set_size(imgbtn, width, height);
    }
    
    // Apply theme-consistent styling
    lv_obj_set_style_border_color(imgbtn, chinScreen_get_color(chinScreen_current_theme->accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(imgbtn, chinScreen_current_theme->border_width, LV_PART_MAIN);
    lv_obj_set_style_radius(imgbtn, chinScreen_current_theme->corner_radius, LV_PART_MAIN);
    
    // Add click shadow effect
    lv_obj_set_style_shadow_width(imgbtn, 4, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(imgbtn, LV_OPA_30, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(imgbtn, 2, LV_PART_MAIN);
    
    // Add callback
    if (callback) {
        lv_obj_add_event_cb(imgbtn, callback, LV_EVENT_CLICKED, NULL);
    }
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(imgbtn, align, 0, 0);
    
    chinScreen_unlock_display();
    return imgbtn;
}

// Simple image button (backward compatibility)
inline lv_obj_t* chinScreen_image_button(const char* image_path,
                                        chinScreen_btn_callback_t callback,
                                        const char* vAlign = "middle",
                                        const char* hAlign = "center",
                                        int width = 80,
                                        int height = 80) {
    return chinScreen_image_button_advanced(image_path, nullptr, nullptr, callback, vAlign, hAlign, width, height);
}

// Toggle image button (two-state)
inline lv_obj_t* chinScreen_toggle_image_button(const char* off_image,
                                               const char* on_image,
                                               bool initial_state = false,
                                               chinScreen_btn_callback_t callback = nullptr,
                                               const char* vAlign = "middle",
                                               const char* hAlign = "center") {
    chinScreen_lock_display();
    
    lv_obj_t* imgbtn = lv_imagebutton_create(lv_screen_active());
    lv_obj_add_flag(imgbtn, LV_OBJ_FLAG_CHECKABLE);
    
    // Set up toggle states
    char off_path[128], on_path[128];
    snprintf(off_path, sizeof(off_path), "S:%s", off_image);
    snprintf(on_path, sizeof(on_path), "S:%s", on_image);
    
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_RELEASED, NULL, off_path, NULL);
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_PRESSED, NULL, off_path, NULL);
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, on_path, NULL);
    lv_imagebutton_set_source(imgbtn, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, on_path, NULL);
    
    if (initial_state) {
        lv_obj_add_state(imgbtn, LV_STATE_CHECKED);
    }
    
    if (callback) {
        lv_obj_add_event_cb(imgbtn, callback, LV_EVENT_VALUE_CHANGED, NULL);
    }
    
    // Position
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(imgbtn, align, 0, 0);
    
    chinScreen_unlock_display();
    return imgbtn;
}

//=============================================================================
// IMAGE GALLERY AND SLIDESHOW
//=============================================================================

// Image gallery structure
typedef struct {
    char** image_paths;
    int image_count;
    int current_index;
    lv_obj_t* container;
    lv_obj_t* current_image;
    lv_obj_t* prev_btn;
    lv_obj_t* next_btn;
    lv_obj_t* counter_label;
    bool auto_advance;
    uint32_t slide_duration;
    lv_timer_t* slide_timer;
} chinScreen_gallery_t;

static chinScreen_gallery_t* chinScreen_active_gallery = nullptr;

// Gallery navigation callbacks
static void chinScreen_gallery_prev_cb(lv_event_t* e) {
    if (!chinScreen_active_gallery) return;
    
    chinScreen_active_gallery->current_index--;
    if (chinScreen_active_gallery->current_index < 0) {
        chinScreen_active_gallery->current_index = chinScreen_active_gallery->image_count - 1;
    }
    
    // Update image
    char lvgl_path[128];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", 
             chinScreen_active_gallery->image_paths[chinScreen_active_gallery->current_index]);
    lv_image_set_src(chinScreen_active_gallery->current_image, lvgl_path);
    
    // Update counter
    lv_label_set_text_fmt(chinScreen_active_gallery->counter_label, "%d/%d", 
                          chinScreen_active_gallery->current_index + 1,
                          chinScreen_active_gallery->image_count);
}

static void chinScreen_gallery_next_cb(lv_event_t* e) {
    if (!chinScreen_active_gallery) return;
    
    chinScreen_active_gallery->current_index++;
    if (chinScreen_active_gallery->current_index >= chinScreen_active_gallery->image_count) {
        chinScreen_active_gallery->current_index = 0;
    }
    
    // Update image
    char lvgl_path[128];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", 
             chinScreen_active_gallery->image_paths[chinScreen_active_gallery->current_index]);
    lv_image_set_src(chinScreen_active_gallery->current_image, lvgl_path);
    
    // Update counter
    lv_label_set_text_fmt(chinScreen_active_gallery->counter_label, "%d/%d", 
                          chinScreen_active_gallery->current_index + 1,
                          chinScreen_active_gallery->image_count);
}

// Auto-advance timer callback
static void chinScreen_gallery_timer_cb(lv_timer_t* timer) {
    chinScreen_gallery_next_cb(nullptr);
}

// Create image gallery/slideshow
inline lv_obj_t* chinScreen_image_gallery(const char* folder_path,
                                         const char* file_pattern = "*",
                                         int width = 300, int height = 200,
                                         bool show_controls = true,
                                         bool auto_slideshow = false,
                                         uint32_t slide_seconds = 3) {
    
    // Clean up previous gallery
    if (chinScreen_active_gallery) {
        if (chinScreen_active_gallery->slide_timer) {
            lv_timer_delete(chinScreen_active_gallery->slide_timer);
        }
        if (chinScreen_active_gallery->image_paths) {
            for (int i = 0; i < chinScreen_active_gallery->image_count; i++) {
                free(chinScreen_active_gallery->image_paths[i]);
            }
            free(chinScreen_active_gallery->image_paths);
        }
        free(chinScreen_active_gallery);
    }
    
    // Create new gallery structure
    chinScreen_active_gallery = (chinScreen_gallery_t*)malloc(sizeof(chinScreen_gallery_t));
    memset(chinScreen_active_gallery, 0, sizeof(chinScreen_gallery_t));
    
    // Scan folder for images
    File dir = SD.open(folder_path);
    if (!dir || !dir.isDirectory()) {
        Serial.printf("chinScreen: Gallery folder not found: %s\n", folder_path);
        free(chinScreen_active_gallery);
        chinScreen_active_gallery = nullptr;
        return nullptr;
    }
    
    // Count valid images first
    int max_images = 50; // Reasonable limit
    chinScreen_active_gallery->image_paths = (char**)malloc(max_images * sizeof(char*));
    chinScreen_active_gallery->image_count = 0;
    
    File file = dir.openNextFile();
    while (file && chinScreen_active_gallery->image_count < max_images) {
        if (!file.isDirectory()) {
            String filename = String(file.name());
            filename.toLowerCase();
            
            // Check if it's an image file
            if (filename.endsWith(".png") || filename.endsWith(".jpg") || 
                filename.endsWith(".jpeg") || filename.endsWith(".bmp") || 
                filename.endsWith(".gif")) {
                
                String full_path = String(folder_path) + "/" + String(file.name());
                chinScreen_active_gallery->image_paths[chinScreen_active_gallery->image_count] = 
                    strdup(full_path.c_str());
                chinScreen_active_gallery->image_count++;
            }
        }
        file.close();
        file = dir.openNextFile();
    }
    dir.close();
    
    if (chinScreen_active_gallery->image_count == 0) {
        Serial.printf("chinScreen: No images found in %s\n", folder_path);
        free(chinScreen_active_gallery->image_paths);
        free(chinScreen_active_gallery);
        chinScreen_active_gallery = nullptr;
        return nullptr;
    }
    
    Serial.printf("chinScreen: Gallery loaded with %d images\n", chinScreen_active_gallery->image_count);
    
    chinScreen_lock_display();
    
    // Create container
    chinScreen_active_gallery->container = lv_obj_create(lv_screen_active());
    lv_obj_set_size(chinScreen_active_gallery->container, width, height + (show_controls ? 50 : 0));
    lv_obj_set_style_bg_color(chinScreen_active_gallery->container, 
                              chinScreen_get_color(chinScreen_current_theme->background_color), LV_PART_MAIN);
    lv_obj_set_style_border_color(chinScreen_active_gallery->container,
                                 chinScreen_get_color(chinScreen_current_theme->accent_color), LV_PART_MAIN);
    lv_obj_center(chinScreen_active_gallery->container);
    
    // Create main image display
    chinScreen_active_gallery->current_image = lv_image_create(chinScreen_active_gallery->container);
    lv_obj_set_size(chinScreen_active_gallery->current_image, width - 10, height - 10);
    lv_obj_align(chinScreen_active_gallery->current_image, LV_ALIGN_TOP_MID, 0, 5);
    
    // Load first image
    char lvgl_path[128];
    snprintf(lvgl_path, sizeof(lvgl_path), "S:%s", chinScreen_active_gallery->image_paths[0]);
    lv_image_set_src(chinScreen_active_gallery->current_image, lvgl_path);
    lv_image_set_size_mode(chinScreen_active_gallery->current_image, LV_IMG_SIZE_MODE_REAL);
    
    // Create controls if requested
    if (show_controls) {
        // Previous button
        chinScreen_active_gallery->prev_btn = lv_button_create(chinScreen_active_gallery->container);
        lv_obj_set_size(chinScreen_active_gallery->prev_btn, 50, 30);
        lv_obj_align(chinScreen_active_gallery->prev_btn, LV_ALIGN_BOTTOM_LEFT, 5, -5);
        lv_obj_add_event_cb(chinScreen_active_gallery->prev_btn, chinScreen_gallery_prev_cb, LV_EVENT_CLICKED, NULL);
        
        lv_obj_t* prev_label = lv_label_create(chinScreen_active_gallery->prev_btn);
        lv_label_set_text(prev_label, "<");
        lv_obj_center(prev_label);
        
        // Next button  
        chinScreen_active_gallery->next_btn = lv_button_create(chinScreen_active_gallery->container);
        lv_obj_set_size(chinScreen_active_gallery->next_btn, 50, 30);
        lv_obj_align(chinScreen_active_gallery->next_btn, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
        lv_obj_add_event_cb(chinScreen_active_gallery->next_btn, chinScreen_gallery_next_cb, LV_EVENT_CLICKED, NULL);
        
        lv_obj_t* next_label = lv_label_create(chinScreen_active_gallery->next_btn);
        lv_label_set_text(next_label, ">");
        lv_obj_center(next_label);
        
        // Counter label
        chinScreen_active_gallery->counter_label = lv_label_create(chinScreen_active_gallery->container);
        lv_label_set_text_fmt(chinScreen_active_gallery->counter_label, "1/%d", chinScreen_active_gallery->image_count);
        lv_obj_align(chinScreen_active_gallery->counter_label, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_obj_set_style_text_color(chinScreen_active_gallery->counter_label,
                                   chinScreen_get_color(chinScreen_current_theme->text_color), LV_PART_MAIN);
    }
    
    // Set up auto-advance if requested
    if (auto_slideshow && slide_seconds > 0) {
        chinScreen_active_gallery->auto_advance = true;
        chinScreen_active_gallery->slide_duration = slide_seconds * 1000;
        chinScreen_active_gallery->slide_timer = lv_timer_create(chinScreen_gallery_timer_cb, 
                                                                chinScreen_active_gallery->slide_duration, NULL);
    }
    
    chinScreen_unlock_display();
    return chinScreen_active_gallery->container;
}

//=============================================================================
// IMAGE PROCESSING AND EFFECTS
//=============================================================================

// Apply visual effects to images
inline void chinScreen_image_set_opacity(lv_obj_t* img, int opacity_percent) {
    if (!img) return;
    chinScreen_lock_display();
    lv_obj_set_style_img_opa(img, (opacity_percent * 255) / 100, LV_PART_MAIN);
    chinScreen_unlock_display();
}

inline void chinScreen_image_set_rotation(lv_obj_t* img, int degrees) {
    if (!img) return;
    chinScreen_lock_display();
    lv_image_set_rotation(img, degrees * 10); // LVGL uses 0.1 degree units
    chinScreen_unlock_display();
}

inline void chinScreen_image_set_zoom(lv_obj_t* img, int zoom_percent) {
    if (!img) return;
    chinScreen_lock_display();
    lv_image_set_scale(img, (zoom_percent * 256) / 100); // LVGL uses 1/256 scale units
    chinScreen_unlock_display();
}

// Add border/frame to image
inline void chinScreen_image_add_frame(lv_obj_t* img, const char* color, int width) {
    if (!img) return;
    chinScreen_lock_display();
    lv_obj_set_style_border_color(img, chinScreen_get_color(color), LV_PART_MAIN);
    lv_obj_set_style_border_width(img, width, LV_PART_MAIN);
    lv_obj_set_style_border_opa(img, LV_OPA_COVER, LV_PART_MAIN);
    chinScreen_unlock_display();
}

// Add shadow effect
inline void chinScreen_image_add_shadow(lv_obj_t* img, const char* color = "black", int width = 8) {
    if (!img) return;
    chinScreen_lock_display();
    lv_obj_set_style_shadow_color(img, chinScreen_get_color(color), LV_PART_MAIN);
    lv_obj_set_style_shadow_width(img, width, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(img, LV_OPA_50, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_x(img, 2, LV_PART_MAIN);
    lv_obj_set_style_shadow_offset_y(img, 2, LV_PART_MAIN);
    chinScreen_unlock_display();
}

//=============================================================================
// IMAGE UTILITY FUNCTIONS
//=============================================================================

// Get image information
inline void chinScreen_image_get_info(const char* filepath) {
    chinScreen_img_format_t format = chinScreen_detect_image_format(filepath);
    
    Serial.println("=== chinScreen Image Info ===");
    Serial.printf("File: %s\n", filepath);
    Serial.printf("Format: %s\n", chinScreen_get_format_name(format));
    Serial.printf("Exists: %s\n", SD.exists(filepath) ? "YES" : "NO");
    
    if (SD.exists(filepath)) {
        File file = SD.open(filepath);
        Serial.printf("Size: %lu bytes\n", file.size());
        file.close();
    }
    Serial.println("============================");
}

// Gallery control functions
inline void chinScreen_gallery_next() {
    chinScreen_gallery_next_cb(nullptr);
}

inline void chinScreen_gallery_previous() {
    chinScreen_gallery_prev_cb(nullptr);
}