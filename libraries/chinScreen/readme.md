<img width="289" height="116" alt="image" src="https://github.com/user-attachments/assets/b94c7160-a1c1-4d21-ba04-0b2e1dd884c5" />


**Robust Ardunio ESP32 (S3) library for the JC3248W525EN using lvgl because it works**
> [!NOTE]
> Remember to use the following header to set the orientation:\
> #define LVGL_PORT_ROTATION_DEGREE 0\
> #include <chinScreen.h>
>
> See readme.md in library folder for all functions\
> https://github.com/rloucks/chinScreen/blob/main/libraries/chinScreen/readme.md


> [!IMPORTANT]
> <img width="536" height="386" alt="image" src="https://github.com/user-attachments/assets/e92ab03c-7026-410d-8a15-3082b83bc8dd" />


**New chinScreen Commands include support for LVGL 9.3.0 Compatibility**

## **Main Calls from chinScreen.h**

    chinScreen_init_shapes();     // Load shapes only
    chinScreen_init_interface();  // Load UI controls only
    chinScreen_init_all();        // Load everything

**Usage Example:**

    #define LVGL_PORT_ROTATION_DEGREE 0
    #include <chinScreen.h>
    
    void setup() {
        chinScreen_init_display();  // Core display init
        chinScreen_init_shapes();   // Only load shapes module
        
        // Now you can use shape functions
        chinScreen_rectangle("red", "white", 100, 50);
    }

## **chinScreen_commands.h**
> [!IMPORTANT]
>  Only loads when chinScreen_init_commands() is called

**LVGL 9.3.0 Inclusions**
- Updated function names (lv_screen_active() instead of lv_scr_act())
- New rotation constants (LV_DISPLAY_ROTATION_*)
- Improved memory management with lv_gc_collect()
- Enhanced filesystem driver with read/write support

**Enhanced Screen Management**

    chinScreen_clear();                    // Clear screen safely
    chinScreen_refresh();                  // Force display refresh
    chinScreen_get_screen_size(&w, &h);   // Get current dimensions

**Advanced Background System**

    chinScreen_background_solid("blue");
    chinScreen_background_gradient("red", "yellow", "horizontal");
    chinScreen_background_gradient_advanced("red", "yellow", "blue");

**Runtime Display Control**

    chinScreen_set_rotation(90);          // Change rotation at runtime
    chinScreen_brightness(128);           // Set brightness 0-255
    chinScreen_backlight(true);           // Control backlight

**Improved Text Functions**

    chinScreen_text("Hello", 10, 10, "red", "large");
    chinScreen_text_aligned("Centered", "middle", "center");
    chinScreen_text_multiline("Long text...", 10, 10, 200);

**Enhanced SD Card Support**

    chinScreen_init_sd_card();           // Initialize with LVGL 9.3.0 driver  
    chinScreen_sd_info();                // Get card statistics
    chinScreen_sd_available();           // Check if card is ready

**Performance Monitoring**

    chinScreen_memory_info();            // LVGL memory usage
    chinScreen_gc();                     // Force garbage collection

**Usage Example:**

    #include <chinScreen.h>
    
    void setup() {
        chinScreen_init_display();
        chinScreen_init_commands();        // Load commands module
        
        chinScreen_background_gradient("blue", "cyan", "vertical");
        chinScreen_text_aligned("Welcome!", "middle", "center", "white", "xlarge");
        
        chinScreen_init_sd_card();         // Optional SD card support
        chinScreen_memory_info();          // Check memory usage
    }

> [!NOTE]
> Backward Compatibility: Legacy functions still work with warnings

## chinScreen_shapes.h
> [!IMPORTANT]
>  Only loads when chinScreen_init_shapes() is called

**Style system for consistent appearance**

    chinScreen_shape_style_t style = chinScreen_create_style("blue",
    "white", 2, "cyan", "horizontal"); lv_obj_t* rect =
    chinScreen_rectangle_advanced(100, 50, &style);

**Simple backward-compatible functions**

    chinScreen_rectangle("red", "black", 100, 50);
    chinScreen_circle("green", "white", 30);
    chinScreen_rounded_rectangle("blue", "gray", 120, 60, 15);

**Geometric Shapes**

    chinScreen_triangle("yellow", "red", 80, 60);
    chinScreen_hexagon("purple", "white", 40);
    chinScreen_polygon("orange", "black", 50, 8);  // Octagon
    chinScreen_pentagon("cyan", "blue", 35);

**Complex Shapes**

    chinScreen_star("gold", "red", 40, 20, 5);           // 5-pointed star
    chinScreen_arrow("green", "black", 60, 40, "right"); // Directional arrow

**Canvas-Based Custom Shapes**

    lv_obj_t* canvas = chinScreen_create_canvas(200, 150);
    // Draw custom shapes on the canvas using LVGL 9.3.0 drawing functions

**Shape Utility Functions**

    // Add text to any shape
    chinScreen_shape_text(my_circle, "Click Me", "white", "large");

**Dynamic shape manipulation**

    chinScreen_move_shape(my_rect, 100, 50);
    chinScreen_resize_shape(my_rect, 150, 75);
    chinScreen_change_shape_color(my_rect, "purple");
    chinScreen_hide_shape(my_rect);
    chinScreen_show_shape(my_rect);

**Basic Shapes Example**

    #include <chinScreen.h>
    
    void setup() {
        chinScreen_init_display();
        chinScreen_init_shapes();
        
    // Create various shapes
        lv_obj_t* rect = chinScreen_rectangle("blue", "white", 100, 60);
        lv_obj_t* circle = chinScreen_circle("red", "black", 30);
        lv_obj_t* star = chinScreen_star("gold", "red", 40, 20, 5);
        
    // Add text to shapes
        chinScreen_shape_text(rect, "Rect", "white", "medium");
        chinScreen_shape_text(circle, "O", "white", "large");
    }
    Advanced Styling
    // Create custom style
    chinScreen_shape_style_t my_style = chinScreen_create_style(
        "blue",      // fill color
        "white",     // border color  
        3,           // border width
        "cyan",      // gradient color
        "horizontal" // gradient direction
    );
    
    // Apply to multiple shapes
    lv_obj_t* rect1 = chinScreen_rectangle_advanced(100, 50, &my_style, "top", "left");
    lv_obj_t* rect2 = chinScreen_rectangle_advanced(100, 50, &my_style, "bottom", "right");
    Complex Shapes
    // Multi-pointed star
    lv_obj_t* star8 = chinScreen_star("purple", "white", 50, 25, 8);
    
    // Custom polygon (decagon)  
    lv_obj_t* decagon = chinScreen_polygon("orange", "red", 40, 10);
    
    // Directional arrows
    lv_obj_t* up_arrow = chinScreen_arrow("green", "black", 40, 50, "up", "top", "center");
    lv_obj_t* down_arrow = chinScreen_arrow("red", "black", 40, 50, "down", "bottom", "center");

## chinScreen_interface.h
> [!IMPORTANT]
>  Only loads when chinScreen_init_interface() is called
>  
**Enhanced buttons with themes and styles**

    chinScreen_button("Click Me", callback, "middle", "center", 120, 50, "raised");
    chinScreen_image_button("/icons/play.png", callback);
    chinScreen_toggle_button("Enable Feature", true);

**Text input with built-in styling**

    lv_obj_t* ta = chinScreen_textarea("Enter name...", 200, 40);

**Virtual keyboard with automatic pairing**

    lv_obj_t* kb = chinScreen_keyboard(ta, "bottom", "center");

**Number pad with result tracking**

    lv_obj_t* numpad = chinScreen_numpad();
    int result = chinScreen_numpad_get_int();

**Selection Controls**

    // Dropdown menus
    chinScreen_dropdown("Option 1\nOption 2\nOption 3", "Choose...");

**Radio button groups**  

    chinScreen_radio_group("Red\nGreen\nBlue", 0, "middle", "center", true);

**Switches with labels**

    chinScreen_labeled_switch("Dark Mode", false);

**Progress Indicators**

    // Linear progress bars
    chinScreen_progress_bar(200, 20, 0, 100, 50, "middle", "center", true);
    
    // Circular progress arcs
    chinScreen_progress_arc(50, 0, 100, 75, "middle", "center", true);
    
    // Loading spinners  
    chinScreen_spinner(40, 1000);

**Tab views with multiple tabs**

    lv_obj_t* tabs = chinScreen_tabview("Home\nSettings\nAbout", "top", 50);

**Scrollable lists**

    chinScreen_list("Item 1\nItem 2\nItem 3", "middle", "center", 200, 300);

**Message boxes**

    chinScreen_message_box("Alert", "Save changes?", "Yes\nNo\nCancel", callback);

**Built-in Themes**

    // Apply consistent styling across all UI elements
    chinScreen_set_theme(&chinScreen_theme_dark);   // Dark theme
    chinScreen_set_theme(&chinScreen_theme_light);  // Light theme  
    chinScreen_set_theme(&chinScreen_theme_default); // Default theme
    Custom Themes
    cppchinScreen_ui_theme_t my_theme = {
        .primary_color = "purple",
        .secondary_color = "black", 
        .accent_color = "gold",
        .text_color = "white",
        .background_color = "black",
        .border_width = 3,
        .corner_radius = 15,
        .font = FONT_LARGE
    };
    chinScreen_set_theme(&my_theme);

**Usage Examples**

    //Complete UI Setup
    cpp#include <chinScreen.h>
    
    void button_clicked(lv_event_t* e) {
        Serial.println("Button pressed!");
    }
    
    void setup() {
        chinScreen_init_display();
        chinScreen_init_interface();
        
        // Set dark theme
        chinScreen_set_theme(&chinScreen_theme_dark);
        
        // Create UI elements  
        lv_obj_t* btn = chinScreen_button("Start", button_clicked, "top", "center");
        lv_obj_t* slider = chinScreen_slider(0, 100, 50, "horizontal", "middle", "center");
        lv_obj_t* progress = chinScreen_progress_bar(200, 15, 0, 100, 25, "bottom", "center", true);
    }
    
    void loop() {
        // Update progress based on slider
        int slider_val = chinScreen_slider_get_value(slider);
        chinScreen_progress_set_value(progress, slider_val);
    }
    Input Form Example
    cpp// Create input form
    lv_obj_t* name_input = chinScreen_textarea("Enter name...", 200, 30);
    lv_obj_t* email_input = chinScreen_textarea("Enter email...", 200, 30); 
    lv_obj_t* country = chinScreen_dropdown("USA\nCanada\nUK\nAustralia");
    lv_obj_t* subscribe = chinScreen_labeled_switch("Subscribe to newsletter", false);
    
    // Show keyboard when textarea is clicked
    lv_obj_add_event_cb(name_input, [](lv_event_t* e) {
        chinScreen_keyboard((lv_obj_t*)lv_event_get_target(e));
    }, LV_EVENT_FOCUSED, NULL);
    
## chinScreen_images.h
> [!IMPORTANT]
>  Only loads when chinScreen_init_images() is called

## chinScreen_video.h
> [!IMPORTANT]
>  Only loads when chinScreen_init_video() is called
