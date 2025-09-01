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
// chinScreen Shapes Module
// Rectangles, circles, triangles, polygons, and custom shapes
// Compatible with LVGL 9.3.0
//=============================================================================

#pragma once

// Module guard - only load if shapes haven't been loaded
#ifndef CHINSCREEN_SHAPES_H
#define CHINSCREEN_SHAPES_H

#include <math.h>

//=============================================================================
// SHAPE STYLE STRUCTURES (LVGL 9.3.0)
//=============================================================================

// Shape configuration structure for consistent styling
typedef struct {
    const char* fill_color;
    const char* border_color;
    const char* gradient_color;
    const char* gradient_direction;
    int border_width;
    int border_radius;
    bool use_gradient;
    lv_opa_t opacity;
} chinScreen_shape_style_t;

// Default shape style
static const chinScreen_shape_style_t chinScreen_default_shape_style = {
    .fill_color = "white",
    .border_color = "black",
    .gradient_color = nullptr,
    .gradient_direction = "vertical",
    .border_width = 2,
    .border_radius = 0,
    .use_gradient = false,
    .opacity = LV_OPA_COVER
};

//=============================================================================
// SHAPE STYLING HELPER FUNCTIONS
//=============================================================================

// Apply style configuration to any LVGL object
inline void chinScreen_apply_shape_style(lv_obj_t* obj, const chinScreen_shape_style_t* style) {
    if (!obj || !style) return;
    
    chinScreen_lock_display();
    
    // Background color or gradient
    if (style->use_gradient && style->gradient_color) {
        lv_color_t color1 = chinScreen_get_color(style->fill_color);
        lv_color_t color2 = chinScreen_get_color(style->gradient_color);
        
        lv_grad_dir_t grad_dir = LV_GRAD_DIR_VER;
        if (strcmp(style->gradient_direction, "horizontal") == 0) grad_dir = LV_GRAD_DIR_HOR;
        
        lv_obj_set_style_bg_color(obj, color1, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(obj, color2, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(obj, grad_dir, LV_PART_MAIN);
    } else {
        lv_obj_set_style_bg_color(obj, chinScreen_get_color(style->fill_color), LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_MAIN);
    }
    
    // Border and opacity
    lv_obj_set_style_bg_opa(obj, style->opacity, LV_PART_MAIN);
    lv_obj_set_style_border_color(obj, chinScreen_get_color(style->border_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(obj, style->border_width, LV_PART_MAIN);
    lv_obj_set_style_border_opa(obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(obj, style->border_radius, LV_PART_MAIN);
    
    chinScreen_unlock_display();
}

// Create a custom style configuration
inline chinScreen_shape_style_t chinScreen_create_style(const char* fill_color,
                                                       const char* border_color = "black",
                                                       int border_width = 2,
                                                       const char* gradient_color = nullptr,
                                                       const char* gradient_direction = "vertical") {
    chinScreen_shape_style_t style = chinScreen_default_shape_style;
    style.fill_color = fill_color;
    style.border_color = border_color;
    style.border_width = border_width;
    style.gradient_color = gradient_color;
    style.gradient_direction = gradient_direction;
    style.use_gradient = (gradient_color != nullptr);
    return style;
}

//=============================================================================
// BASIC SHAPES - RECTANGLES
//=============================================================================

// Advanced rectangle with full styling options
inline lv_obj_t* chinScreen_rectangle_advanced(int width, int height,
                                              const chinScreen_shape_style_t* style,
                                              const char* vAlign = "middle",
                                              const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t *rect = lv_obj_create(lv_screen_active());
    lv_obj_set_size(rect, width, height);
    
    // Apply styling
    if (style) {
        chinScreen_apply_shape_style(rect, style);
    } else {
        chinScreen_apply_shape_style(rect, &chinScreen_default_shape_style);
    }

    // Position the rectangle
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(rect, align, 0, 0);

    chinScreen_unlock_display();
    return rect;
}

// Simple rectangle (backward compatibility)
inline lv_obj_t* chinScreen_rectangle(const char* fillColor,
                                     const char* borderColor,
                                     int width, int height,
                                     const char* vAlign = "middle",
                                     const char* hAlign = "center",
                                     const char* gradientColor = nullptr,
                                     const char* gradientDir = "vertical") {
    chinScreen_shape_style_t style = chinScreen_create_style(fillColor, borderColor, 3, gradientColor, gradientDir);
    return chinScreen_rectangle_advanced(width, height, &style, vAlign, hAlign);
}

// Rounded rectangle
inline lv_obj_t* chinScreen_rounded_rectangle(const char* fillColor,
                                             const char* borderColor,
                                             int width, int height,
                                             int radius,
                                             const char* vAlign = "middle",
                                             const char* hAlign = "center") {
    chinScreen_shape_style_t style = chinScreen_create_style(fillColor, borderColor, 3);
    style.border_radius = radius;
    return chinScreen_rectangle_advanced(width, height, &style, vAlign, hAlign);
}

//=============================================================================
// BASIC SHAPES - CIRCLES AND ELLIPSES
//=============================================================================

// Advanced circle with full styling options
inline lv_obj_t* chinScreen_circle_advanced(int radius,
                                           const chinScreen_shape_style_t* style,
                                           const char* vAlign = "middle",
                                           const char* hAlign = "center") {
    chinScreen_lock_display();

    lv_obj_t *circle = lv_obj_create(lv_screen_active());
    lv_obj_set_size(circle, radius * 2, radius * 2);
    
    // Apply styling
    if (style) {
        chinScreen_apply_shape_style(circle, style);
    } else {
        chinScreen_apply_shape_style(circle, &chinScreen_default_shape_style);
    }
    
    // Make it circular
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    // Position the circle
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(circle, align, 0, 0);

    chinScreen_unlock_display();
    return circle;
}

// Simple circle (backward compatibility)
inline lv_obj_t* chinScreen_circle(const char* fillColor, 
                                  const char* borderColor, 
                                  int radius, 
                                  const char* vAlign = "middle", 
                                  const char* hAlign = "center",
                                  const char* gradientColor = nullptr,
                                  const char* gradientDir = "vertical") {
    chinScreen_shape_style_t style = chinScreen_create_style(fillColor, borderColor, 3, gradientColor, gradientDir);
    return chinScreen_circle_advanced(radius, &style, vAlign, hAlign);
}

// Ellipse/Oval shape
inline lv_obj_t* chinScreen_ellipse(const char* fillColor, 
                                   const char* borderColor,
                                   int width, int height,
                                   const char* vAlign = "middle", 
                                   const char* hAlign = "center",
                                   const char* gradientColor = nullptr) {
    chinScreen_shape_style_t style = chinScreen_create_style(fillColor, borderColor, 3, gradientColor);
    style.border_radius = LV_RADIUS_CIRCLE;
    return chinScreen_rectangle_advanced(width, height, &style, vAlign, hAlign);
}

// Backward compatibility alias
inline lv_obj_t* chinScreen_oval(const char* fillColor, 
                                const char* borderColor,
                                int width, int height,
                                const char* vAlign = "middle", 
                                const char* hAlign = "center") {
    return chinScreen_ellipse(fillColor, borderColor, width, height, vAlign, hAlign);
}

//=============================================================================
// CANVAS-BASED SHAPES (For complex shapes)
//=============================================================================

// Canvas buffer management for complex shapes
#define CHINSCREEN_CANVAS_BUFFER_SIZE (400 * 400)
static lv_color_t chinScreen_canvas_buffer[CHINSCREEN_CANVAS_BUFFER_SIZE];

// Create a canvas for drawing custom shapes
inline lv_obj_t* chinScreen_create_canvas(int width, int height,
                                         const char* vAlign = "middle",
                                         const char* hAlign = "center") {
    if (width * height > CHINSCREEN_CANVAS_BUFFER_SIZE) {
        Serial.printf("chinScreen: Canvas size %dx%d too large. Max: %d pixels\n", 
                      width, height, CHINSCREEN_CANVAS_BUFFER_SIZE);
        return nullptr;
    }
    
    chinScreen_lock_display();
    
    lv_obj_t *canvas = lv_canvas_create(lv_screen_active());
    lv_obj_set_size(canvas, width, height);
    
    // Set canvas buffer
    lv_canvas_set_buffer(canvas, chinScreen_canvas_buffer, width, height, LV_COLOR_FORMAT_RGB565);
    lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_TRANSP);
    
    // Position the canvas
    lv_align_t align = chinScreen_get_alignment(vAlign, hAlign);
    lv_obj_align(canvas, align, 0, 0);
    
    chinScreen_unlock_display();
    return canvas;
}

//=============================================================================
// POLYGON SHAPES
//=============================================================================

// Triangle using improved canvas drawing (LVGL 9.3.0)
inline lv_obj_t* chinScreen_triangle(const char* fillColor, 
                                    const char* borderColor,
                                    int width, int height,
                                    const char* vAlign = "middle", 
                                    const char* hAlign = "center") {
    lv_obj_t *canvas = chinScreen_create_canvas(width + 6, height + 6, vAlign, hAlign);
    if (!canvas) return nullptr;
    
    chinScreen_lock_display();
    
    // Triangle vertices
    lv_point_precise_t triangle_points[3];
    triangle_points[0].x = 3 + width / 2.0f;  triangle_points[0].y = 3;
    triangle_points[1].x = 3;                 triangle_points[1].y = 3 + height;
    triangle_points[2].x = 3 + width;         triangle_points[2].y = 3 + height;
    
    // Draw triangle using LVGL 9.3.0 drawing system
    lv_draw_triangle_dsc_t triangle_dsc;
    lv_draw_triangle_dsc_init(&triangle_dsc);
    triangle_dsc.bg_color = chinScreen_get_color(fillColor);
    triangle_dsc.bg_opa = LV_OPA_COVER;
    
    lv_canvas_draw_triangle(canvas, triangle_points, &triangle_dsc);
    
    // Draw border if needed
    if (strcmp(borderColor, "none") != 0) {
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = chinScreen_get_color(borderColor);
        line_dsc.width = 3;
        
        // Draw triangle outline
        for (int i = 0; i < 3; i++) {
            lv_point_precise_t p1 = triangle_points[i];
            lv_point_precise_t p2 = triangle_points[(i + 1) % 3];
            lv_canvas_draw_line(canvas, &p1, &p2, &line_dsc);
        }
    }
    
    chinScreen_unlock_display();
    return canvas;
}

// Regular polygon (pentagon, hexagon, octagon, etc.)
inline lv_obj_t* chinScreen_polygon(const char* fillColor,
                                   const char* borderColor,
                                   int radius, int sides,
                                   const char* vAlign = "middle",
                                   const char* hAlign = "center") {
    if (sides < 3 || sides > 20) {
        Serial.printf("chinScreen: Invalid polygon sides (%d). Use 3-20.\n", sides);
        return nullptr;
    }
    
    int canvas_size = radius * 2 + 10;
    lv_obj_t *canvas = chinScreen_create_canvas(canvas_size, canvas_size, vAlign, hAlign);
    if (!canvas) return nullptr;
    
    chinScreen_lock_display();
    
    // Calculate polygon vertices
    lv_point_precise_t* points = new lv_point_precise_t[sides + 1];
    float center_x = canvas_size / 2.0f;
    float center_y = canvas_size / 2.0f;
    
    for (int i = 0; i < sides; i++) {
        float angle = (2.0f * M_PI * i) / sides - M_PI / 2.0f; // Start from top
        points[i].x = center_x + radius * cosf(angle);
        points[i].y = center_y + radius * sinf(angle);
    }
    points[sides] = points[0]; // Close the polygon
    
    // Draw filled polygon
    if (strcmp(fillColor, "none") != 0) {
        lv_draw_triangle_dsc_t fill_dsc;
        lv_draw_triangle_dsc_init(&fill_dsc);
        fill_dsc.bg_color = chinScreen_get_color(fillColor);
        fill_dsc.bg_opa = LV_OPA_COVER;
        
        // Draw triangles from center to create filled polygon
        lv_point_precise_t center = {center_x, center_y};
        for (int i = 0; i < sides; i++) {
            lv_point_precise_t triangle[3] = {center, points[i], points[i + 1]};
            lv_canvas_draw_triangle(canvas, triangle, &fill_dsc);
        }
    }
    
    // Draw border
    if (strcmp(borderColor, "none") != 0) {
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = chinScreen_get_color(borderColor);
        line_dsc.width = 3;
        
        for (int i = 0; i < sides; i++) {
            lv_canvas_draw_line(canvas, &points[i], &points[i + 1], &line_dsc);
        }
    }
    
    delete[] points;
    chinScreen_unlock_display();
    return canvas;
}

// Convenience functions for common polygons
inline lv_obj_t* chinScreen_hexagon(const char* fillColor, const char* borderColor,
                                   int radius, const char* vAlign = "middle", 
                                   const char* hAlign = "center") {
    return chinScreen_polygon(fillColor, borderColor, radius, 6, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_pentagon(const char* fillColor, const char* borderColor,
                                    int radius, const char* vAlign = "middle", 
                                    const char* hAlign = "center") {
    return chinScreen_polygon(fillColor, borderColor, radius, 5, vAlign, hAlign);
}

inline lv_obj_t* chinScreen_octagon(const char* fillColor, const char* borderColor,
                                   int radius, const char* vAlign = "middle", 
                                   const char* hAlign = "center") {
    return chinScreen_polygon(fillColor, borderColor, radius, 8, vAlign, hAlign);
}

//=============================================================================
// COMPLEX SHAPES
//=============================================================================

// Star shape
inline lv_obj_t* chinScreen_star(const char* fillColor, const char* borderColor,
                                int outer_radius, int inner_radius, int points = 5,
                                const char* vAlign = "middle", const char* hAlign = "center") {
    if (points < 3 || points > 12) {
        Serial.printf("chinScreen: Invalid star points (%d). Use 3-12.\n", points);
        return nullptr;
    }
    
    int canvas_size = outer_radius * 2 + 10;
    lv_obj_t *canvas = chinScreen_create_canvas(canvas_size, canvas_size, vAlign, hAlign);
    if (!canvas) return nullptr;
    
    chinScreen_lock_display();
    
    // Calculate star vertices
    int total_points = points * 2;
    lv_point_precise_t* vertices = new lv_point_precise_t[total_points + 1];
    float center_x = canvas_size / 2.0f;
    float center_y = canvas_size / 2.0f;
    
    for (int i = 0; i < total_points; i++) {
        float angle = (2.0f * M_PI * i) / total_points - M_PI / 2.0f;
        float radius = (i % 2 == 0) ? outer_radius : inner_radius;
        vertices[i].x = center_x + radius * cosf(angle);
        vertices[i].y = center_y + radius * sinf(angle);
    }
    vertices[total_points] = vertices[0]; // Close the star
    
    // Draw filled star
    if (strcmp(fillColor, "none") != 0) {
        lv_draw_triangle_dsc_t fill_dsc;
        lv_draw_triangle_dsc_init(&fill_dsc);
        fill_dsc.bg_color = chinScreen_get_color(fillColor);
        fill_dsc.bg_opa = LV_OPA_COVER;
        
        lv_point_precise_t center = {center_x, center_y};
        for (int i = 0; i < total_points; i++) {
            lv_point_precise_t triangle[3] = {center, vertices[i], vertices[i + 1]};
            lv_canvas_draw_triangle(canvas, triangle, &fill_dsc);
        }
    }
    
    // Draw border
    if (strcmp(borderColor, "none") != 0) {
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = chinScreen_get_color(borderColor);
        line_dsc.width = 3;
        
        for (int i = 0; i < total_points; i++) {
            lv_canvas_draw_line(canvas, &vertices[i], &vertices[i + 1], &line_dsc);
        }
    }
    
    delete[] vertices;
    chinScreen_unlock_display();
    return canvas;
}

// Arrow shape
inline lv_obj_t* chinScreen_arrow(const char* fillColor, const char* borderColor,
                                 int width, int height, const char* direction = "right",
                                 const char* vAlign = "middle", const char* hAlign = "center") {
    lv_obj_t *canvas = chinScreen_create_canvas(width + 6, height + 6, vAlign, hAlign);
    if (!canvas) return nullptr;
    
    chinScreen_lock_display();
    
    // Arrow points based on direction
    lv_point_precise_t arrow_points[8]; // Maximum for complex arrow
    int point_count = 7;
    
    if (strcmp(direction, "right") == 0) {
        arrow_points[0] = {3, 3 + height/3};
        arrow_points[1] = {3 + width*2/3, 3 + height/3};
        arrow_points[2] = {3 + width*2/3, 3};
        arrow_points[3] = {3 + width, 3 + height/2};
        arrow_points[4] = {3 + width*2/3, 3 + height};
        arrow_points[5] = {3 + width*2/3, 3 + height*2/3};
        arrow_points[6] = {3, 3 + height*2/3};
    } else if (strcmp(direction, "left") == 0) {
        arrow_points[0] = {3 + width, 3 + height/3};
        arrow_points[1] = {3 + width/3, 3 + height/3};
        arrow_points[2] = {3 + width/3, 3};
        arrow_points[3] = {3, 3 + height/2};
        arrow_points[4] = {3 + width/3, 3 + height};
        arrow_points[5] = {3 + width/3, 3 + height*2/3};
        arrow_points[6] = {3 + width, 3 + height*2/3};
    } else if (strcmp(direction, "up") == 0) {
        arrow_points[0] = {3 + width/3, 3 + height};
        arrow_points[1] = {3 + width/3, 3 + height/3};
        arrow_points[2] = {3, 3 + height/3};
        arrow_points[3] = {3 + width/2, 3};
        arrow_points[4] = {3 + width, 3 + height/3};
        arrow_points[5] = {3 + width*2/3, 3 + height/3};
        arrow_points[6] = {3 + width*2/3, 3 + height};
    } else { // down
        arrow_points[0] = {3 + width/3, 3};
        arrow_points[1] = {3 + width/3, 3 + height*2/3};
        arrow_points[2] = {3, 3 + height*2/3};
        arrow_points[3] = {3 + width/2, 3 + height};
        arrow_points[4] = {3 + width, 3 + height*2/3};
        arrow_points[5] = {3 + width*2/3, 3 + height*2/3};
        arrow_points[6] = {3 + width*2/3, 3};
    }
    arrow_points[7] = arrow_points[0]; // Close shape
    
    // Draw filled arrow
    if (strcmp(fillColor, "none") != 0) {
        lv_draw_triangle_dsc_t fill_dsc;
        lv_draw_triangle_dsc_init(&fill_dsc);
        fill_dsc.bg_color = chinScreen_get_color(fillColor);
        fill_dsc.bg_opa = LV_OPA_COVER;
        
        // Simple triangulation for arrow fill
        lv_point_precise_t center = {3 + width/2, 3 + height/2};
        for (int i = 0; i < point_count; i++) {
            lv_point_precise_t triangle[3] = {center, arrow_points[i], arrow_points[i + 1]};
            lv_canvas_draw_triangle(canvas, triangle, &fill_dsc);
        }
    }
    
    // Draw border
    if (strcmp(borderColor, "none") != 0) {
        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.color = chinScreen_get_color(borderColor);
        line_dsc.width = 3;
        
        for (int i = 0; i < point_count; i++) {
            lv_canvas_draw_line(canvas, &arrow_points[i], &arrow_points[i + 1], &line_dsc);
        }
    }
    
    chinScreen_unlock_display();
    return canvas;
}

//=============================================================================
// SHAPE UTILITY FUNCTIONS
//=============================================================================

// Add text to any shape
inline void chinScreen_shape_text(lv_obj_t* shape, const char* text, 
                                 const char* colorName = "white", 
                                 const char* fontSize = "medium") {
    if (!shape || !text) return;
    
    chinScreen_lock_display();

    lv_obj_t *label = lv_label_create(shape);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, chinScreen_get_color(colorName), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, chinScreen_get_font(fontSize), LV_PART_MAIN);
    lv_obj_center(label);

    chinScreen_unlock_display();
}

// Get shape dimensions
inline void chinScreen_get_shape_size(lv_obj_t* shape, int* width, int* height) {
    if (!shape) return;
    *width = lv_obj_get_width(shape);
    *height = lv_obj_get_height(shape);
}

// Move shape to new position
inline void chinScreen_move_shape(lv_obj_t* shape, int x, int y) {
    if (!shape) return;
    chinScreen_lock_display();
    lv_obj_set_pos(shape, x, y);
    chinScreen_unlock_display();
}

// Resize shape
inline void chinScreen_resize_shape(lv_obj_t* shape, int width, int height) {
    if (!shape) return;
    chinScreen_lock_display();
    lv_obj_set_size(shape, width, height);
    chinScreen_unlock_display();
}

// Change shape color dynamically
inline void chinScreen_change_shape_color(lv_obj_t* shape, const char* new_color) {
    if (!shape || !new_color) return;
    chinScreen_lock_display();
    lv_obj_set_style_bg_color(shape, chinScreen_get_color(new_color), LV_PART_MAIN);
    chinScreen_unlock_display();
}

// Hide/Show shape
inline void chinScreen_hide_shape(lv_obj_t* shape) {
    if (!shape) return;
    chinScreen_lock_display();
    lv_obj_add_flag(shape, LV_OBJ_FLAG_HIDDEN);
    chinScreen_unlock_display();
}

inline void chinScreen_show_shape(lv_obj_t* shape) {
    if (!shape) return;
    chinScreen_lock_display();
    lv_obj_remove_flag(shape, LV_OBJ_FLAG_HIDDEN);
    chinScreen_unlock_display();
}

//=============================================================================
// LEGACY COMPATIBILITY
//=============================================================================

// Legacy function warnings and aliases
inline void chinScreen_set_position(lv_obj_t* obj, int x, int y) {
    chinScreen_warn_deprecated("chinScreen_set_position", "chinScreen_move_shape");
    chinScreen_move_shape(obj, x, y);
}

inline void chinScreen_delete_object(lv_obj_t* obj) {
    chinScreen_warn_deprecated("chinScreen_delete_object", "lv_obj_delete or chinScreen_hide_shape");
    if (!obj) return;
    chinScreen_lock_display();
    lv_obj_delete(obj);
    chinScreen_unlock_display();
}

#endif // CHINSCREEN_SHAPES_H