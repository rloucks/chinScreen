//         __     __         _______                              
//  .----.|  |--.|__|.-----.|     __|.----.----.-----.-----.-----.
//  |  __||     ||  ||     ||__     ||  __|   _|  -__|  -__|     |
//  |____||__|__||__||__|__||_______||____|__| |_____|_____|__|__|
//
//   Stupid library for the JC3248W525EN using lvgl because it works
//
//


#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <ctype.h>  // for tolower



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

inline void chinScreen_clear()
{
    lv_obj_t *scr = lv_scr_act();
    
    // Remove all child objects
    lv_obj_clean(scr);
    
    // Optional: force redraw
    lv_obj_invalidate(scr);
}

inline void create_ui(int debugChin) {
    Serial.println("Create UI");

    bsp_display_lock(0);  
	if (debugChin == 1) {
		lv_obj_t *label = lv_label_create(lv_scr_act());
		lv_label_set_text(label, "chinScreen Works!");
		lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
		}
	
	if (debugChin == 2) {
	   // Create a full-screen container
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr); // clear screen before test

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
        lv_obj_t *rect = lv_obj_create(scr);
        lv_obj_set_size(rect, 80, 60);
        lv_obj_set_style_bg_color(rect, colors[i], LV_PART_MAIN);
        lv_obj_align(rect, LV_ALIGN_TOP_LEFT, (i % 4) * 85, (i / 4) * 65);

        lv_obj_t *label = lv_label_create(rect);
        lv_label_set_text(label, names[i]);
        lv_obj_center(label);
    }

	}
    bsp_display_unlock();

    Serial.println("UI created");
}

// Turn the backlight on and off
inline void chinScreen_backlight(int bLight) {   
	if (bLight == 0) {
    bsp_display_backlight_off(); 
	} else {
	bsp_display_backlight_on();
	}
}

// Turn the backlight on and off
inline void chinScreen_brightness(int howBright) {   
	bsp_display_brightness_set(howBright);
}


inline void chinScreen_text(char *m) {
   bsp_display_lock(0);  

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, m);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    bsp_display_unlock();	
}

// Simple parallel arrays for color names and values using lv_color_make
static const char* colorNames[] = {
    "black", "white", "red", "green", "blue",
    "yellow", "cyan", "magenta", "gray", "orange",
    "purple", "pink"
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
    lv_color_make(255,192,203)  // pink
};

lv_color_t getColorByName(const char* name) {
    for (uint8_t i = 0; i < sizeof(colorNames)/sizeof(colorNames[0]); i++) {
        // Case-insensitive compare
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

// Rectangle drawing function
void chinScreen_rectangle(const char* bgColorName,
                   const char* borderColorName,
                   int width, int height,
                   const char* vAlign,   // "top", "middle", "bottom"
                   const char* hAlign)   // "left", "center", "right"
{
    lv_obj_t *rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, width, height);

    // Background color
    lv_obj_set_style_bg_color(rect, getColorByName(bgColorName), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, LV_PART_MAIN);

    // Border color
    lv_obj_set_style_border_color(rect, getColorByName(borderColorName), LV_PART_MAIN);
    lv_obj_set_style_border_width(rect, 3, LV_PART_MAIN);

    // Positioning
    lv_align_t align;
    if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_TOP_LEFT;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_TOP_MID;
    else if (strcmp(vAlign, "top") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_TOP_RIGHT;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_LEFT_MID;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_CENTER;
    else if (strcmp(vAlign, "middle") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_RIGHT_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "left") == 0) align = LV_ALIGN_BOTTOM_LEFT;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "center") == 0) align = LV_ALIGN_BOTTOM_MID;
    else if (strcmp(vAlign, "bottom") == 0 && strcmp(hAlign, "right") == 0) align = LV_ALIGN_BOTTOM_RIGHT;
    else align = LV_ALIGN_CENTER; // fallback

    lv_obj_align(rect, align, 0, 0);
}

void chinScreen_circle(const char* fillColorName, const char* borderColorName, uint16_t radius,
                const char* verticalPos = "middle", const char* horizontalPos = "center")
{
    lv_color_t fillColor   = getColorByName(fillColorName);
    lv_color_t borderColor = getColorByName(borderColorName);

    lv_obj_t *circle = lv_obj_create(lv_scr_act());
    lv_obj_set_size(circle, radius * 2, radius * 2);

    // Style
    lv_obj_set_style_bg_color(circle, fillColor, 0);
    lv_obj_set_style_border_color(circle, borderColor, 0);
    lv_obj_set_style_border_width(circle, 3, 0);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);

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

    lv_obj_align(circle, LV_ALIGN_CENTER, 0, 0); // default center
    lv_obj_align(circle, align, 0, 0);
}

// -----------------------
// Triangle
// -----------------------
void chinScreen_triangle(const char* fillColorName, const char* borderColorName,
                  uint16_t width, uint16_t height,
                  const char* verticalPos = "middle", const char* horizontalPos = "center")
{
    lv_color_t fillColor   = getColorByName(fillColorName);
    lv_color_t borderColor = getColorByName(borderColorName);

    static lv_point_t points[3];

    points[0].x = 0;      points[0].y = height;
    points[1].x = width/2; points[1].y = 0;
    points[2].x = width;  points[2].y = height;

    lv_obj_t *triangle = lv_line_create(lv_scr_act());
    lv_line_set_points(triangle, points, 3); // set points
    lv_obj_set_style_line_color(triangle, borderColor, 0);
    lv_obj_set_style_line_width(triangle, 3, 0);

    // For filled triangle, create a polygon (lvgl 8 uses lv_canvas or lv_obj with mask)
    lv_obj_set_style_bg_color(triangle, fillColor, 0);
    lv_obj_set_style_bg_opa(triangle, LV_OPA_COVER, 0);

    // Positioning
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

    lv_obj_align(triangle, LV_ALIGN_CENTER, 0, 0); // default center
    lv_obj_align(triangle, align, 0, 0);
}