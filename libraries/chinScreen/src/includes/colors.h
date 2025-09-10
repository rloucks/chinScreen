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
//   File: colors.h
//   Purpose: colors!
//   Required: YES

/////////////////////////////////////////////////////////////
// Complete set of 140 HTML color names and values for LVGL 8.3.11
/////////////////////////////////////////////////////////////
static const char* colorNames[] = {
    "aliceblue", "antiquewhite", "aqua", "aquamarine", "azure",
    "beige", "bisque", "black", "blanchedalmond", "blue",
    "blueviolet", "brown", "burlywood", "cadetblue", "chartreuse",
    "chocolate", "coral", "cornflowerblue", "cornsilk", "crimson",
    "cyan", "darkblue", "darkcyan", "darkgoldenrod", "darkgray",
    "darkgrey", "darkgreen", "darkkhaki", "darkmagenta", "darkolivegreen",
    "darkorange", "darkorchid", "darkred", "darksalmon", "darkseagreen",
    "darkslateblue", "darkslategray", "darkslategrey", "darkturquoise", "darkviolet",
    "deeppink", "deepskyblue", "dimgray", "dimgrey", "dodgerblue",
    "firebrick", "floralwhite", "forestgreen", "fuchsia", "gainsboro",
    "ghostwhite", "gold", "goldenrod", "gray", "grey",
    "green", "greenyellow", "honeydew", "hotpink", "indianred",
    "indigo", "ivory", "khaki", "lavender", "lavenderblush",
    "lawngreen", "lemonchiffon", "lightblue", "lightcoral", "lightcyan",
    "lightgoldenrodyellow", "lightgray", "lightgrey", "lightgreen", "lightpink",
    "lightsalmon", "lightseagreen", "lightskyblue", "lightslategray", "lightslategrey",
    "lightsteelblue", "lightyellow", "lime", "limegreen", "linen",
    "magenta", "maroon", "mediumaquamarine", "mediumblue", "mediumorchid",
    "mediumpurple", "mediumseagreen", "mediumslateblue", "mediumspringgreen", "mediumturquoise",
    "mediumvioletred", "midnightblue", "mintcream", "mistyrose", "moccasin",
    "navajowhite", "navy", "oldlace", "olive", "olivedrab",
    "orange", "orangered", "orchid", "palegoldenrod", "palegreen",
    "paleturquoise", "palevioletred", "papayawhip", "peachpuff", "peru",
    "pink", "plum", "powderblue", "purple", "red",
    "rosybrown", "royalblue", "saddlebrown", "salmon", "sandybrown",
    "seagreen", "seashell", "sienna", "silver", "skyblue",
    "slateblue", "slategray", "slategrey", "snow", "springgreen",
    "steelblue", "tan", "teal", "thistle", "tomato",
    "turquoise", "violet", "wheat", "white", "whitesmoke",
    "yellow", "yellowgreen"
};

static const lv_color_t colorValues[] = {
    lv_color_make(240,248,255), // aliceblue
    lv_color_make(250,235,215), // antiquewhite
    lv_color_make(0,255,255),   // aqua
    lv_color_make(127,255,212), // aquamarine
    lv_color_make(240,255,255), // azure
    lv_color_make(245,245,220), // beige
    lv_color_make(255,228,196), // bisque
    lv_color_make(0,0,0),       // black
    lv_color_make(255,235,205), // blanchedalmond
    lv_color_make(0,0,255),     // blue
    lv_color_make(138,43,226),  // blueviolet
    lv_color_make(165,42,42),   // brown
    lv_color_make(222,184,135), // burlywood
    lv_color_make(95,158,160),  // cadetblue
    lv_color_make(127,255,0),   // chartreuse
    lv_color_make(210,105,30),  // chocolate
    lv_color_make(255,127,80),  // coral
    lv_color_make(100,149,237), // cornflowerblue
    lv_color_make(255,248,220), // cornsilk
    lv_color_make(220,20,60),   // crimson
    lv_color_make(0,255,255),   // cyan
    lv_color_make(0,0,139),     // darkblue
    lv_color_make(0,139,139),   // darkcyan
    lv_color_make(184,134,11),  // darkgoldenrod
    lv_color_make(169,169,169), // darkgray
    lv_color_make(169,169,169), // darkgrey
    lv_color_make(0,100,0),     // darkgreen
    lv_color_make(189,183,107), // darkkhaki
    lv_color_make(139,0,139),   // darkmagenta
    lv_color_make(85,107,47),   // darkolivegreen
    lv_color_make(255,140,0),   // darkorange
    lv_color_make(153,50,204),  // darkorchid
    lv_color_make(139,0,0),     // darkred
    lv_color_make(233,150,122), // darksalmon
    lv_color_make(143,188,143), // darkseagreen
    lv_color_make(72,61,139),   // darkslateblue
    lv_color_make(47,79,79),    // darkslategray
    lv_color_make(47,79,79),    // darkslategrey
    lv_color_make(0,206,209),   // darkturquoise
    lv_color_make(148,0,211),   // darkviolet
    lv_color_make(255,20,147),  // deeppink
    lv_color_make(0,191,255),   // deepskyblue
    lv_color_make(105,105,105), // dimgray
    lv_color_make(105,105,105), // dimgrey
    lv_color_make(30,144,255),  // dodgerblue
    lv_color_make(178,34,34),   // firebrick
    lv_color_make(255,250,240), // floralwhite
    lv_color_make(34,139,34),   // forestgreen
    lv_color_make(255,0,255),   // fuchsia
    lv_color_make(220,220,220), // gainsboro
    lv_color_make(248,248,255), // ghostwhite
    lv_color_make(255,215,0),   // gold
    lv_color_make(218,165,32),  // goldenrod
    lv_color_make(128,128,128), // gray
    lv_color_make(128,128,128), // grey
    lv_color_make(0,128,0),     // green
    lv_color_make(173,255,47),  // greenyellow
    lv_color_make(240,255,240), // honeydew
    lv_color_make(255,105,180), // hotpink
    lv_color_make(205,92,92),   // indianred
    lv_color_make(75,0,130),    // indigo
    lv_color_make(255,255,240), // ivory
    lv_color_make(240,230,140), // khaki
    lv_color_make(230,230,250), // lavender
    lv_color_make(255,240,245), // lavenderblush
    lv_color_make(124,252,0),   // lawngreen
    lv_color_make(255,250,205), // lemonchiffon
    lv_color_make(173,216,230), // lightblue
    lv_color_make(240,128,128), // lightcoral
    lv_color_make(224,255,255), // lightcyan
    lv_color_make(250,250,210), // lightgoldenrodyellow
    lv_color_make(211,211,211), // lightgray
    lv_color_make(211,211,211), // lightgrey
    lv_color_make(144,238,144), // lightgreen
    lv_color_make(255,182,193), // lightpink
    lv_color_make(255,160,122), // lightsalmon
    lv_color_make(32,178,170),  // lightseagreen
    lv_color_make(135,206,250), // lightskyblue
    lv_color_make(119,136,153), // lightslategray
    lv_color_make(119,136,153), // lightslategrey
    lv_color_make(176,196,222), // lightsteelblue
    lv_color_make(255,255,224), // lightyellow
    lv_color_make(0,255,0),     // lime
    lv_color_make(50,205,50),   // limegreen
    lv_color_make(250,240,230), // linen
    lv_color_make(255,0,255),   // magenta
    lv_color_make(128,0,0),     // maroon
    lv_color_make(102,205,170), // mediumaquamarine
    lv_color_make(0,0,205),     // mediumblue
    lv_color_make(186,85,211),  // mediumorchid
    lv_color_make(147,112,219), // mediumpurple
    lv_color_make(60,179,113),  // mediumseagreen
    lv_color_make(123,104,238), // mediumslateblue
    lv_color_make(0,250,154),   // mediumspringgreen
    lv_color_make(72,209,204),  // mediumturquoise
    lv_color_make(199,21,133),  // mediumvioletred
    lv_color_make(25,25,112),   // midnightblue
    lv_color_make(245,255,250), // mintcream
    lv_color_make(255,228,225), // mistyrose
    lv_color_make(255,228,181), // moccasin
    lv_color_make(255,222,173), // navajowhite
    lv_color_make(0,0,128),     // navy
    lv_color_make(253,245,230), // oldlace
    lv_color_make(128,128,0),   // olive
    lv_color_make(107,142,35),  // olivedrab
    lv_color_make(255,165,0),   // orange
    lv_color_make(255,69,0),    // orangered
    lv_color_make(218,112,214), // orchid
    lv_color_make(238,232,170), // palegoldenrod
    lv_color_make(152,251,152), // palegreen
    lv_color_make(175,238,238), // paleturquoise
    lv_color_make(219,112,147), // palevioletred
    lv_color_make(255,239,213), // papayawhip
    lv_color_make(255,218,185), // peachpuff
    lv_color_make(205,133,63),  // peru
    lv_color_make(255,192,203), // pink
    lv_color_make(221,160,221), // plum
    lv_color_make(176,224,230), // powderblue
    lv_color_make(128,0,128),   // purple
    lv_color_make(255,0,0),     // red
    lv_color_make(188,143,143), // rosybrown
    lv_color_make(65,105,225),  // royalblue
    lv_color_make(139,69,19),   // saddlebrown
    lv_color_make(250,128,114), // salmon
    lv_color_make(244,164,96),  // sandybrown
    lv_color_make(46,139,87),   // seagreen
    lv_color_make(255,245,238), // seashell
    lv_color_make(160,82,45),   // sienna
    lv_color_make(192,192,192), // silver
    lv_color_make(135,206,235), // skyblue
    lv_color_make(106,90,205),  // slateblue
    lv_color_make(112,128,144), // slategray
    lv_color_make(112,128,144), // slategrey
    lv_color_make(255,250,250), // snow
    lv_color_make(0,255,127),   // springgreen
    lv_color_make(70,130,180),  // steelblue
    lv_color_make(210,180,140), // tan
    lv_color_make(0,128,128),   // teal
    lv_color_make(216,191,216), // thistle
    lv_color_make(255,99,71),   // tomato
    lv_color_make(64,224,208),  // turquoise
    lv_color_make(238,130,238), // violet
    lv_color_make(245,222,179), // wheat
    lv_color_make(255,255,255), // white
    lv_color_make(245,245,245), // whitesmoke
    lv_color_make(255,255,0),   // yellow
    lv_color_make(154,205,50)   // yellowgreen
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

// Optional: Get the number of available colors
inline uint8_t getColorCount() {
    return sizeof(colorNames)/sizeof(colorNames[0]);
}

// Optional: Get color name by index
inline const char* getColorNameByIndex(uint8_t index) {
    if (index < sizeof(colorNames)/sizeof(colorNames[0])) {
        return colorNames[index];
    }
    return nullptr;
}

/////////////////////////////////////////////////////////////
// Function: Set gradient background (original 2-color)
/////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////
// Enhanced Gradient System - Multi-Color Support
/////////////////////////////////////////////////////////////

// Gradient stop structure for multi-color gradients
struct chinScreen_gradient_stop {
    lv_color_t color;
    uint8_t position;  // 0-100 percentage position
};

// Pre-defined rainbow color arrays
static const lv_color_t rainbow_colors[] = {
    lv_color_make(255, 0, 0),    // Red
    lv_color_make(255, 127, 0),  // Orange
    lv_color_make(255, 255, 0),  // Yellow
    lv_color_make(0, 255, 0),    // Green
    lv_color_make(0, 0, 255),    // Blue
    lv_color_make(75, 0, 130),   // Indigo
    lv_color_make(148, 0, 211)   // Violet
};

// Sunset gradient colors
static const lv_color_t sunset_colors[] = {
    lv_color_make(255, 94, 77),   // Coral
    lv_color_make(255, 154, 0),   // Orange
    lv_color_make(255, 206, 84),  // Yellow
    lv_color_make(255, 138, 101), // Peach
    lv_color_make(161, 102, 171)  // Purple
};

// Ocean gradient colors
static const lv_color_t ocean_colors[] = {
    lv_color_make(0, 119, 190),   // Deep blue
    lv_color_make(0, 180, 216),   // Sky blue
    lv_color_make(144, 224, 239), // Light blue
    lv_color_make(202, 240, 248)  // Very light blue
};

// Fire gradient colors
static const lv_color_t fire_colors[] = {
    lv_color_make(139, 0, 0),     // Dark red
    lv_color_make(255, 0, 0),     // Red
    lv_color_make(255, 165, 0),   // Orange
    lv_color_make(255, 255, 0),   // Yellow
    lv_color_make(255, 255, 255)  // White
};

/////////////////////////////////////////////////////////////
// Function: Create multi-color gradient background
/////////////////////////////////////////////////////////////
inline void chinScreen_background_multi_gradient(const chinScreen_gradient_stop* stops, 
                                                 uint8_t stop_count,
                                                 const char* direction = "vertical") {
    if (stop_count < 2) return;
    
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    
    // Clear any existing children first
    lv_obj_clean(scr);
    
    // CRITICAL: Ensure the screen itself has no padding/margins
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    
    lv_grad_dir_t grad_dir = LV_GRAD_DIR_VER;
    if (strcmp(direction, "horizontal") == 0) grad_dir = LV_GRAD_DIR_HOR;
    
    // Use the full display resolution constants
    lv_coord_t screen_width = LV_HOR_RES;
    lv_coord_t screen_height = LV_VER_RES;
    
    // Set screen background to the first color as fallback
    lv_obj_set_style_bg_color(scr, stops[0].color, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    // Create gradient segments with guaranteed full coverage
    for (uint8_t i = 0; i < stop_count - 1; i++) {
        lv_obj_t *gradient_obj = lv_obj_create(scr);
        
        // CRITICAL: Remove ALL styling that could create gaps
        lv_obj_remove_style_all(gradient_obj);
        lv_obj_set_style_border_width(gradient_obj, 0, LV_PART_MAIN);
        lv_obj_set_style_pad_all(gradient_obj, 0, LV_PART_MAIN);
        lv_obj_set_style_outline_width(gradient_obj, 0, LV_PART_MAIN);
        lv_obj_clear_flag(gradient_obj, LV_OBJ_FLAG_SCROLLABLE);
        
        // Calculate segment positions with overlap
        uint8_t start_pos = stops[i].position;
        uint8_t end_pos = stops[i + 1].position;
        
        lv_coord_t start_pixel, size_pixel;
        
        if (grad_dir == LV_GRAD_DIR_VER) {
            // Vertical gradient - calculate precise boundaries
            start_pixel = (start_pos * screen_height) / 100;
            lv_coord_t end_pixel = (end_pos * screen_height) / 100;
            size_pixel = end_pixel - start_pixel;
            
            // Add minimal overlap to prevent gaps but avoid scroll
            if (i > 0) start_pixel -= 1;  // Minimal overlap with previous
            if (i < stop_count - 2) {
                size_pixel += 2;  // Small overlap with next
            } else {
                // Last segment - ensure it ends exactly at screen bottom
                size_pixel = screen_height - start_pixel;
            }
            
            // Ensure we don't go negative or exceed screen bounds
            if (start_pixel < 0) start_pixel = 0;
            if (start_pixel + size_pixel > screen_height) {
                size_pixel = screen_height - start_pixel;
            }
            
            lv_obj_set_size(gradient_obj, screen_width, size_pixel);
            lv_obj_set_pos(gradient_obj, 0, start_pixel);
            
        } else {
            // Horizontal gradient - calculate precise boundaries
            start_pixel = (start_pos * screen_width) / 100;
            lv_coord_t end_pixel = (end_pos * screen_width) / 100;
            size_pixel = end_pixel - start_pixel;
            
            // Add minimal overlap to prevent gaps but avoid scroll
            if (i > 0) start_pixel -= 1;  // Minimal overlap with previous
            if (i < stop_count - 2) {
                size_pixel += 2;  // Small overlap with next
            } else {
                // Last segment - ensure it ends exactly at screen edge
                size_pixel = screen_width - start_pixel;
            }
            
            // Ensure we don't go negative or exceed screen bounds
            if (start_pixel < 0) start_pixel = 0;
            if (start_pixel + size_pixel > screen_width) {
                size_pixel = screen_width - start_pixel;
            }
            
            lv_obj_set_size(gradient_obj, size_pixel, screen_height);
            lv_obj_set_pos(gradient_obj, start_pixel, 0);
        }
        
        // Set gradient colors
        lv_obj_set_style_bg_color(gradient_obj, stops[i].color, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_color(gradient_obj, stops[i + 1].color, LV_PART_MAIN);
        lv_obj_set_style_bg_grad_dir(gradient_obj, grad_dir, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(gradient_obj, LV_OPA_COVER, LV_PART_MAIN);
        
        // Ensure the object is positioned correctly
        lv_obj_move_background(gradient_obj);
    }
    
    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Function: Create rainbow gradient background
/////////////////////////////////////////////////////////////
inline void chinScreen_background_rainbow(const char* direction = "vertical", 
                                          const char* style = "full") {
    chinScreen_gradient_stop stops[7];
    
    // Create rainbow stops with equal spacing
    for (int i = 0; i < 7; i++) {
        stops[i].color = rainbow_colors[i];
        stops[i].position = (i * 100) / 6;  // 0, 16, 33, 50, 66, 83, 100
    }
    
    // Adjust for different rainbow styles
    if (strcmp(style, "warm") == 0) {
        // Only red, orange, yellow
        chinScreen_gradient_stop warm_stops[3] = {
            {rainbow_colors[0], 0},   // Red
            {rainbow_colors[1], 50},  // Orange
            {rainbow_colors[2], 100}  // Yellow
        };
        chinScreen_background_multi_gradient(warm_stops, 3, direction);
    } else if (strcmp(style, "cool") == 0) {
        // Only green, blue, indigo, violet
        chinScreen_gradient_stop cool_stops[4] = {
            {rainbow_colors[3], 0},   // Green
            {rainbow_colors[4], 33},  // Blue
            {rainbow_colors[5], 66},  // Indigo
            {rainbow_colors[6], 100}  // Violet
        };
        chinScreen_background_multi_gradient(cool_stops, 4, direction);
    } else {
        // Full rainbow
        chinScreen_background_multi_gradient(stops, 7, direction);
    }
}

/////////////////////////////////////////////////////////////
// Function: Create preset gradient backgrounds
/////////////////////////////////////////////////////////////
inline void chinScreen_background_preset_gradient(const char* preset_name, 
                                                  const char* direction = "vertical") {
    if (strcmp(preset_name, "sunset") == 0) {
        chinScreen_gradient_stop stops[5];
        for (int i = 0; i < 5; i++) {
            stops[i].color = sunset_colors[i];
            stops[i].position = (i * 100) / 4;
        }
        chinScreen_background_multi_gradient(stops, 5, direction);
        
    } else if (strcmp(preset_name, "ocean") == 0) {
        chinScreen_gradient_stop stops[4];
        for (int i = 0; i < 4; i++) {
            stops[i].color = ocean_colors[i];
            stops[i].position = (i * 100) / 3;
        }
        chinScreen_background_multi_gradient(stops, 4, direction);
        
    } else if (strcmp(preset_name, "fire") == 0) {
        chinScreen_gradient_stop stops[5];
        for (int i = 0; i < 5; i++) {
            stops[i].color = fire_colors[i];
            stops[i].position = (i * 100) / 4;
        }
        chinScreen_background_multi_gradient(stops, 5, direction);
        
    } else if (strcmp(preset_name, "rainbow") == 0) {
        chinScreen_background_rainbow(direction, "full");
        
    } else {
        Serial.println("Unknown preset gradient name");
    }
}

/////////////////////////////////////////////////////////////
// Function: Create custom gradient from color names
/////////////////////////////////////////////////////////////
inline void chinScreen_background_custom_gradient(const char* color_names[], 
                                                  uint8_t color_count,
                                                  const char* direction = "vertical") {
    if (color_count < 2) return;
    
    chinScreen_gradient_stop* stops = new chinScreen_gradient_stop[color_count];
    
    for (uint8_t i = 0; i < color_count; i++) {
        stops[i].color = getColorByName(color_names[i]);
        stops[i].position = (i * 100) / (color_count - 1);
    }
    
    chinScreen_background_multi_gradient(stops, color_count, direction);
    delete[] stops;
}

/////////////////////////////////////////////////////////////
// Function: Create gradient with custom positions
/////////////////////////////////////////////////////////////
inline void chinScreen_background_positioned_gradient(const char* color_names[], 
                                                      uint8_t positions[],
                                                      uint8_t color_count,
                                                      const char* direction = "vertical") {
    if (color_count < 2) return;
    
    chinScreen_gradient_stop* stops = new chinScreen_gradient_stop[color_count];
    
    for (uint8_t i = 0; i < color_count; i++) {
        stops[i].color = getColorByName(color_names[i]);
        stops[i].position = positions[i];
    }
    
    chinScreen_background_multi_gradient(stops, color_count, direction);
    delete[] stops;
}

/////////////////////////////////////////////////////////////
// Debug function to check screen dimensions
/////////////////////////////////////////////////////////////
inline void chinScreen_debug_screen_info() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    
    Serial.printf("Screen dimensions: %d x %d\n", lv_obj_get_width(scr), lv_obj_get_height(scr));
    Serial.printf("LV_HOR_RES: %d, LV_VER_RES: %d\n", LV_HOR_RES, LV_VER_RES);
    Serial.printf("Screen padding: top=%d, bottom=%d, left=%d, right=%d\n", 
                  lv_obj_get_style_pad_top(scr, LV_PART_MAIN),
                  lv_obj_get_style_pad_bottom(scr, LV_PART_MAIN),
                  lv_obj_get_style_pad_left(scr, LV_PART_MAIN),
                  lv_obj_get_style_pad_right(scr, LV_PART_MAIN));
    
    bsp_display_unlock();
}

/////////////////////////////////////////////////////////////
// Initialize screen to ensure no padding/borders
/////////////////////////////////////////////////////////////
inline void chinScreen_init_screen_for_gradients() {
    bsp_display_lock(0);
    lv_obj_t *scr = lv_scr_act();
    
    // Remove any existing styling that might cause gaps
    lv_obj_set_style_pad_all(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(scr, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);
    
    bsp_display_unlock();
    
    Serial.println("Screen initialized for full-coverage gradients");
}