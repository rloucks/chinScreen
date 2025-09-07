/////////////////////////////////////////////////////////////
// ALL SHAPE FUNCTIONS
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// Rectangle drawing function with optional gradient
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Circle with optional gradient
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Triangle using canvas for proper shape rendering
/////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////
// Hexagon shape
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Oval/Ellipse shape
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Function: Create button from shape
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Function: Animate a shape
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Function: Add text inside a shape
/////////////////////////////////////////////////////////////
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

