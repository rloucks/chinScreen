//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Improved library for the JC3248W525EN using LVGL
//
//

#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <ctype.h>

// Font size constants
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24

// Animation types
typedef enum {
    ANIM_MOVE_X,
    ANIM_MOVE_Y,
    ANIM_SCALE,
    ANIM_ROTATE,
    ANIM_FADE
} chinScreen_anim_type_t;

// Button callback type
typedef void (*chinScreen_btn_callback_t)(lv_event_t* e);

// Initialize the display (panel, rotation, backlight)
inline void init_display() {
    Serial.println("Initialize panel device");

    bsp_display_cfg_t cfg = {
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = EXAMPLE_LCD_QSPI_H_RES * EXAMPLE_LCD_QSPI_V_RES,
#if LVGL_PORT_ROTATION_DEGREE == 90
        .rotate = LV_DISP_ROT_90,
#elif LVGL_PORT_ROTATION_DEGREE == 270
        .rotate = LV_DISP_ROT_270,
#elif LVGL_PORT_ROTATION_DEGREE == 180
        .rotate = LV_DISP_ROT_180,
#elif LVGL_PORT_ROTATION_DEGREE == 0
        .rotate = LV_DISP_ROT_NONE,
#endif
    };

    bsp_display_start_with_config(&cfg);
    bsp_display_backlight_on();
    Serial.println("Display init done");
}

// Simple parallel arrays for color names and values
static const char* colorNames[] = {
    "black", "white", "red", "green", "blue",
    "yellow", "cyan", "magenta", "gray", "orange",
    "purple", "pink", "brown", "navy", "lime"
};

static const lv_color_t colorValues[] = {
    lv_color_make(0,0,0),       // black
    lv_color_make(255,255,255), // white
    lv_color_make(255,0,0),     // red
    lv_color_make(0,255,0),     // green
    lv_color_make(0,0,255),     // blue
    lv_color_make(255,255,0),   // yellow
    lv_color_make(0,255,255),   // cyan
    lv_color_make(255,0,255),   // magenta
    lv_color_make(128,128,128), // gray
    lv_color_make(255,165,0),   // orange
    lv_color_make(128,0,128),   // purple
    lv_color_make(255,192,203), // pink
    lv_color_make(139,69,19),   // brown
    lv_color_make(0,0,128),     // navy
    lv_color_make(50,205,50)    // lime
};

inline lv_color_t getColorByName(const char* name) {
    for (uint8_t i = 0; i < sizeof(colorNames)/sizeof(colorNames[0]); i++) {
        const char* p1 = name;
        const char* p2 = colorNames[i];
        bool match = true;
        while (*p1 && *p2) {
            if (tolower(*p1) != tolower(*p2)) {
                match = false;
                break;
            }
            p1++;
            p2++;
        }
        if (match && *p1 == '\0' && *p2 == '\0') {
            return colorValues[i];
        }
    }
    return lv_color_make(0,0,0); // default black if not found
}

inline void chinScreen_clear() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr);
    lv_obj_invalidate(scr);
    bsp_display_unlock();
}

// Set solid background color
inline void chinScreen_background_solid(const char* colorName) {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    bsp_display_unlock();
}

// Set gradient background
inline void chinScreen_background_gradient(const char* color1Name, const char* color2Name, 
                                          const char* direction = "vertical") {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    
    lv_color_t color1 = getColorByName(color1Name);
    lv_color_t color2 = getColorByName(color2Name);
    
    lv_grad_dir_t grad_dir = LV_GRAD_DIR_VER;
    if (strcmp(direction, "horizontal") == 0) grad_dir = LV_GRAD_DIR_HOR;
    
    lv_obj_set_style_bg_color(scr, color1, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(scr, color2, LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(scr, grad_dir, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    bsp_display_unlock();
}

// Turn the backlight on and off
inline void chinScreen_backlight(int bLight) {   
    if (bLight == 0) {
        bsp_display_backlight_off(); 
    } else {
        bsp_display_backlight_on();
    }
}

// Set brightness
inline void chinScreen_brightness(int howBright) {   
    bsp_display_brightness_set(howBright);
}



inline const lv_font_t* getFontBySize(const char* size) {
    if (strcmp(size, "small") == 0) return FONT_SMALL;
    if (strcmp(size, "medium") == 0) return FONT_MEDIUM;
    if (strcmp(size, "large") == 0) return FONT_LARGE;
    if (strcmp(size, "xlarge") == 0) return FONT_XLARGE;
    return FONT_MEDIUM; // default
}

// Improved text function with position, size, and font
inline void chinScreen_text(const char* text, int x, int y, 
                           const char* colorName = "white", 
                           const char* fontSize = "medium") {
    bsp_display_lock(0);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), LV_PART_MAIN);
    lv_obj_set_pos(label, x, y);

    bsp_display_unlock();
}

// Rectangle drawing function with optional gradient
inline lv_obj_t* chinScreen_rectangle(const char* bgColorName,
                                     const char* borderColorName,
                                     int width, int height,
                                     const char* vAlign = "middle",
                                     const char* hAlign = "center",
                                     const char* gradientColor = nullptr,
                                     const char* gradientDir = "vertical") {
    bsp_display_lock(0);

    lv_obj_t *rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, width, height);

    // Background color or gradient
    if (gradientColor != nullptr) {
        lv_color_t color1 = getColorByName(bgColorName);
        lv_color_t color2 = getColorByName(gradientColor);
        lv_grad_dir_t grad_dir = strcmp(gradientDir, "horizontal") == 0 ? LV_GRAD_DIR_HOR : LV_GRAD_DIR_VER;
        
        lv_obj_set_style_bg_color(rect, color1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(rect, color2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(rect, grad_dir, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(rect, getColorByName(bgColorName), LV_PART_MAIN);
    }
    
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

    // Border
    lv_obj_set_style_border_color(rect, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 3, LV_PART_MAIN);
    lv_obj_set_style_border_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

    // Positioning
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

    lv_obj_align(rect, align, 0, 0);

    bsp_display_unlock();
    return rect;
}

// Circle with optional gradient
inline lv_obj_t* chinScreen_circle(const char* fillColorName, const char* borderColorName, 
                                  uint16_t radius, const char* verticalPos = "middle", 
                                  const char* horizontalPos = "center",
                                  const char* gradientColor = nullptr,
                                  const char* gradientDir = "vertical") {
    bsp_display_lock(0);

    lv_obj_t *circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, radius * 2, radius * 2);

    // Background
    if (gradientColor != nullptr) {
        lv_color_t color1 = getColorByName(fillColorName);
        lv_color_t color2 = getColorByName(gradientColor);
        lv_grad_dir_t grad_dir = strcmp(gradientDir, "horizontal") == 0 ? LV_GRAD_DIR_HOR : LV_GRAD_DIR_VER;
        
        lv_obj_set_style_bg_color(circle, color1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(circle, color2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(circle, grad_dir, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(circle, getColorByName(fillColorName), LV_PART_MAIN);
    }
    
    lv_obj_set_style_bg_opa(circle, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(circle, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(circle, 3, LV_PART_MAIN);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(circle, align, 0, 0);

    bsp_display_unlock();
    return circle;
}

// Fixed triangle using canvas for proper shape rendering
inline lv_obj_t* chinScreen_triangle(const char* fillColorName, const char* borderColorName,
                                     uint16_t width, uint16_t height,
                                     const char* verticalPos = "middle", 
                                     const char* horizontalPos = "center") {
    bsp_display_lock(0);

    // Create canvas for custom drawing
    lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    lv_obj_set_size(canvas, width + 6, height + 6); // Extra space for border
    
    // Create buffer for canvas
    static lv_color_t canvas_buf[200 * 200]; // Adjust size as needed
    lv_canvas_set_buffer(canvas, canvas_buf, width + 6, height + 6, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_TRANSP);

    // Triangle points (centered in canvas)
    lv_point_t points[4];
    points[0].x = 3 + width/2;     points[0].y = 3;           // Top
    points[1].x = 3;               points[1].y = 3 + height; // Bottom left
    points[2].x = 3 + width;       points[2].y = 3 + height; // Bottom right
    points[3].x = 3 + width/2;     points[3].y = 3;           // Close triangle

    // Draw filled triangle
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);
    draw_dsc.bg_color = getColorByName(fillColorName);
    draw_dsc.bg_opa = LV_OPA_COVER;
    
    // Use polygon drawing for filled triangle
    static lv_point_t triangle_points[3];
    triangle_points[0] = points[0];
    triangle_points[1] = points[1]; 
    triangle_points[2] = points[2];
    
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = getColorByName(borderColorName);
    line_dsc.width = 3;
    
    // Draw triangle outline
    for (int i = 0; i < 3; i++) {
        lv_canvas_draw_line(canvas, triangle_points, 3, &line_dsc);
    }

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(canvas, align, 0, 0);

    bsp_display_unlock();
    return canvas;
}

// Hexagon shape
inline lv_obj_t* chinScreen_hexagon(const char* fillColorName, const char* borderColorName,
                                    uint16_t radius, const char* verticalPos = "middle", 
                                    const char* horizontalPos = "center") {
    bsp_display_lock(0);

    lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    uint16_t size = radius * 2 + 10;
    lv_obj_set_size(canvas, size, size);
    
    static lv_color_t hex_buf[150 * 150];
    lv_canvas_set_buffer(canvas, hex_buf, size, size, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_TRANSP);

    // Hexagon points
    lv_point_t points[7];
    int16_t cx = size/2, cy = size/2;
    for (int i = 0; i < 6; i++) {
        float angle = i * 60.0 * PI / 180.0;
        points[i].x = cx + radius * cos(angle);
        points[i].y = cy + radius * sin(angle);
    }
    points[6] = points[0]; // Close shape

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = getColorByName(borderColorName);
    line_dsc.width = 3;
    
    lv_canvas_draw_line(canvas, points, 7, &line_dsc);

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(canvas, align, 0, 0);

    bsp_display_unlock();
    return canvas;
}

// Oval/Ellipse shape
inline lv_obj_t* chinScreen_oval(const char* fillColorName, const char* borderColorName,
                                 uint16_t width, uint16_t height,
                                 const char* verticalPos = "middle", 
                                 const char* horizontalPos = "center") {
    bsp_display_lock(0);

    lv_obj_t *oval = lv_obj_create(lv_scr_act());
    lv_obj_set_size(oval, width, height);
    lv_obj_set_style_bg_color(oval, getColorByName(fillColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(oval, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(oval, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(oval, 3, LV_PART_MAIN);
    lv_obj_set_style_radius(oval, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // Position
    lv_align_t align = LV_ALIGN_CENTER;
    if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(verticalPos, "top") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(verticalPos, "middle") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(verticalPos, "bottom") == 0 && strcmp(horizontalPos, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;

    lv_obj_align(oval, align, 0, 0);

    bsp_display_unlock();
    return oval;
}

// Add text inside a shape
inline void chinScreen_shape_text(lv_obj_t* shape, const char* text, 
                                 const char* colorName = "white", 
                                 const char* fontSize = "medium") {
    if (!shape) return;
    
    bsp_display_lock(0);

    lv_obj_t *label = lv_label_create(shape);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, getColorByName(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, getFontBySize(fontSize), LV_PART_MAIN);
    lv_obj_center(label);

    bsp_display_unlock();
}

// Create button from shape
inline lv_obj_t* chinScreen_button(const char* bgColorName, const char* borderColorName,
                                   int width, int height, const char* text,
                                   chinScreen_btn_callback_t callback,
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, width, height);
    lv_obj_set_style_bg_color(btn, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_border_color(btn, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 3, LV_PART_MAIN);

    // Add text if provided
    if (text && strlen(text) > 0) {
        lv_obj_t *label = lv_label_create(btn);
        lv_label_set_text(label, text);
        lv_obj_center(label);
    }

    // Add callback if provided
    if (callback) {
        lv_obj_add_event_cb(btn, callback, LV_EVENT_CLICKED, NULL);
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

    lv_obj_align(btn, align, 0, 0);

    bsp_display_unlock();
    return btn;
}

// Animate a shape
inline void chinScreen_animate(lv_obj_t* obj, chinScreen_anim_type_t anim_type, 
                              int32_t start_value, int32_t end_value, 
                              uint32_t duration = 1000, bool repeat = false) {
    if (!obj) return;

    bsp_display_lock(0);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_values(&anim, start_value, end_value);

    switch (anim_type) {
        case ANIM_MOVE_X:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
            break;
        case ANIM_MOVE_Y:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
            break;
        case ANIM_SCALE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_width);
            break;
        case ANIM_ROTATE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_angle);
            break;
        case ANIM_FADE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
            break;
    }

    if (repeat) {
        lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_playback_time(&anim, duration);
    }

    lv_anim_start(&anim);

    bsp_display_unlock();
}

// Create slider
inline lv_obj_t* chinScreen_slider(int width, int height, int min_val, int max_val, int initial_val,
                                   const char* bgColorName = "gray", const char* knobColorName = "blue",
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *slider = lv_slider_create(lv_scr_act());
    lv_obj_set_size(slider, width, height);
    lv_slider_set_range(slider, min_val, max_val);
    lv_slider_set_value(slider, initial_val, LV_ANIM_OFF);

    // Styling
    lv_obj_set_style_bg_color(slider, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, getColorByName(knobColorName), LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, getColorByName(knobColorName), LV_PART_INDICATOR);

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

    lv_obj_align(slider, align, 0, 0);

    bsp_display_unlock();
    return slider;
}

// Create toggle switch
inline lv_obj_t* chinScreen_toggle(bool initial_state = false,
                                   const char* bgColorName = "gray", const char* knobColorName = "white",
                                   const char* vAlign = "middle", const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *sw = lv_switch_create(lv_scr_act());
    
    if (initial_state) {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }

    // Styling
    lv_obj_set_style_bg_color(sw, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(sw, getColorByName(knobColorName), LV_PART_KNOB);

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

    lv_obj_align(sw, align, 0, 0);

    bsp_display_unlock();
    return sw;
}

// Get slider value
inline int chinScreen_slider_get_value(lv_obj_t* slider) {
    if (!slider) return 0;
    return lv_slider_get_value(slider);
}

// Get toggle state
inline bool chinScreen_toggle_get_state(lv_obj_t* toggle) {
    if (!toggle) return false;
    return lv_obj_has_state(toggle, LV_STATE_CHECKED);
}

// Set object position (useful for manual positioning)
inline void chinScreen_set_position(lv_obj_t* obj, int x, int y) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_set_pos(obj, x, y);
    bsp_display_unlock();
}

// Delete an object
inline void chinScreen_delete_object(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_obj_del(obj);
    bsp_display_unlock();
}

// Debug function - improved to prevent screen flickering
inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
    
    if (debugChin == 1) {
        // Clear screen first to prevent overlap
        lv_obj_clean(lv_scr_act());
        
        lv_obj_t *label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "chinScreen Works!");
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    }
    
    if (debugChin == 2) {
        // Clear screen before creating new content
        lv_obj_clean(lv_scr_act());

        // Array of test colors
        lv_color_t colors[] = {
            lv_palette_main(LV_PALETTE_RED),
            lv_palette_main(LV_PALETTE_GREEN),
            lv_palette_main(LV_PALETTE_BLUE),
            lv_palette_main(LV_PALETTE_YELLOW),
            lv_palette_main(LV_PALETTE_ORANGE),
            lv_palette_main(LV_PALETTE_PURPLE),
            lv_palette_main(LV_PALETTE_CYAN),
            lv_palette_main(LV_PALETTE_GREY),
        };

        const char* names[] = {
            "Red", "Green", "Blue", "Yellow", "Orange", "Purple", "Cyan", "Grey"
        };

        // Create colored rectangles with labels
        for (int i = 0; i < 8; i++) {
            lv_obj_t *rect = lv_obj_create(lv_scr_act());
            lv_obj_set_size(rect, 80, 60);
            lv_obj_set_style_bg_color(rect, colors[i], LV_PART_MAIN);
            lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);
            lv_obj_align(rect, LV_ALIGN_TOP_LEFT, (i % 4) * 85, (i / 4) * 65);

            lv_obj_t *label = lv_label_create(rect);
            lv_label_set_text(label, names[i]);
            lv_obj_center(label);
        }
    }
    
    bsp_display_unlock();
    Serial.println("UI created");
}

// Example button callback function
inline void example_button_callback(lv_event_t* e) {
    Serial.println("Button clicked!");
    // Add your button action here
}

// Example animation callback for continuous effects
static void anim_ready_cb(lv_anim_t* a) {
    // Animation finished callback
    Serial.println("Animation completed");
}

// Advanced animation with easing
inline void chinScreen_animate_advanced(lv_obj_t* obj, chinScreen_anim_type_t anim_type,
                                       int32_t start_value, int32_t end_value,
                                       uint32_t duration = 1000, bool repeat = false,
                                       const char* easing = "ease_out") {
    if (!obj) return;

    bsp_display_lock(0);

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_values(&anim, start_value, end_value);

    // Set easing path
    if (strcmp(easing, "ease_in") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in);
    } else if (strcmp(easing, "ease_out") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    } else if (strcmp(easing, "ease_in_out") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);
    } else if (strcmp(easing, "bounce") == 0) {
        lv_anim_set_path_cb(&anim, lv_anim_path_bounce);
    } else {
        lv_anim_set_path_cb(&anim, lv_anim_path_linear);
    }

    switch (anim_type) {
        case ANIM_MOVE_X:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_x);
            break;
        case ANIM_MOVE_Y:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);
            break;
        case ANIM_SCALE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_width);
            break;
        case ANIM_ROTATE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_transform_angle);
            start_value *= 10; // LVGL uses 0.1 degree units
            end_value *= 10;
            break;
        case ANIM_FADE:
            lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_style_opa);
            break;
    }

    if (repeat) {
        lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_playback_time(&anim, duration);
    }

    lv_anim_set_ready_cb(&anim, anim_ready_cb);
    lv_anim_start(&anim);

    bsp_display_unlock();
}

// Get current animation state
inline bool chinScreen_is_animating(lv_obj_t* obj) {
    if (!obj) return false;
    return lv_anim_count_running() > 0;
}

// Stop all animations on an object
inline void chinScreen_stop_animations(lv_obj_t* obj) {
    if (!obj) return;
    bsp_display_lock(0);
    lv_anim_del(obj, NULL);
    bsp_display_unlock();
}

// Create progress bar
inline lv_obj_t* chinScreen_progress_bar(int width, int height, int value,
                                        const char* bgColorName = "gray", 
                                        const char* fgColorName = "green",
                                        const char* vAlign = "middle", 
                                        const char* hAlign = "center") {
    bsp_display_lock(0);

    lv_obj_t *bar = lv_bar_create(lv_scr_act());
    lv_obj_set_size(bar, width, height);
    lv_bar_set_value(bar, value, LV_ANIM_OFF);

    // Styling
    lv_obj_set_style_bg_color(bar, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, getColorByName(fgColorName), LV_PART_INDICATOR);

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

    lv_obj_align(bar, align, 0, 0);

    bsp_display_unlock();
    return bar;
}

// Update progress bar value
inline void chinScreen_progress_set_value(lv_obj_t* bar, int value) {
    if (!bar) return;
    bsp_display_lock(0);
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    bsp_display_unlock();
}