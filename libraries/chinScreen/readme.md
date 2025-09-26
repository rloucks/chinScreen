# chinScreen Library v0.0.2b

<img width="289" height="116" alt="image" src="https://github.com/user-attachments/assets/ac291586-abfd-420b-b469-9a0d837e4e67" />


**Enhanced library for the JC3248W525EN using LVGL with comprehensive features**

> [!NOTE]
> Remember to use the following header to set the orientation:
> ```cpp
> #define LVGL_PORT_ROTATION_DEGREE 0
> #include <chinScreen.h>
> ```

## Feature Modules

Enable features by adding defines before including the library:

```cpp
#define CHINSCREEN_ENABLE_SHAPES        // Rectangle, circle, triangle, hexagon, oval
#define CHINSCREEN_ENABLE_MEDIA         // Image, GIF, video support
#define CHINSCREEN_ENABLE_ICONS         // Static PNG icons with color support
#define CHINSCREEN_ENABLE_ANIMATED_ICONS // Animated GIF icons
#define CHINSCREEN_ENABLE_ANIMATIONS    // Advanced animation system
#define CHINSCREEN_ENABLE_EXAMPLES      // Built-in example functions
#define CHINSCREEN_ENABLE_DEBUG         // Debug utilities
#define CHINSCREEN_ENABLE_GFONTS        // Google Fonts support (WIP)
#define CHINSCREEN_ENABLE_SPRITES       // Sprite system (WIP)
```

## Table of Contents

- [Installation](#installation)
- [Initialization](#initialization)
- [Display Controls](#display-controls)
- [Backgrounds](#backgrounds)
  - [Solid Backgrounds](#solid-backgrounds)
  - [Gradient Backgrounds](#gradient-backgrounds)
  - [Multi-Color Gradients](#multi-color-gradients)
  - [Preset Gradients](#preset-gradients)
- [Text Functions](#text-functions)
- [Shapes](#shapes)
- [Icons](#icons)
- [Animated Icons](#animated-icons)
- [Media](#media)
- [Input Controls](#input-controls)
- [Animations](#animations)
- [Timer System](#timer-system)
- [Utilities](#utilities)
- [Color System](#color-system)

---

## Initialization

```cpp
init_display()
```
Initializes the display panel, sets rotation, and turns on the backlight.

---

## Display Controls

### Backlight Control
```cpp
chinScreen_backlight(1); // On
chinScreen_backlight(0); // Off
```

### Set Brightness
```cpp
chinScreen_brightness(128); // Range 0-255
```

### Clear Screen
```cpp
chinScreen_clear();
```

---

## Backgrounds

### Solid Backgrounds
```cpp
chinScreen_background_solid("red");
```

### Gradient Backgrounds
```cpp
chinScreen_background_gradient("blue", "cyan", "horizontal");
chinScreen_background_gradient("red", "yellow", "vertical");
```

### Multi-Color Gradients
```cpp
// Rainbow gradient
chinScreen_background_rainbow("vertical", "full");    // Full spectrum
chinScreen_background_rainbow("horizontal", "warm");  // Red, orange, yellow
chinScreen_background_rainbow("vertical", "cool");    // Green, blue, indigo, violet

// Custom gradient from color array
const char* colors[] = {"red", "orange", "yellow", "green"};
chinScreen_background_custom_gradient(colors, 4, "vertical");

// Gradient with custom positions (0-100%)
const char* colors[] = {"red", "yellow", "blue"};
uint8_t positions[] = {0, 30, 100};
chinScreen_background_positioned_gradient(colors, positions, 3, "horizontal");
```

### Preset Gradients
```cpp
chinScreen_background_preset_gradient("sunset", "vertical");
chinScreen_background_preset_gradient("ocean", "horizontal");
chinScreen_background_preset_gradient("fire", "vertical");
```

---

## Text Functions
Note: if you use "center" on an object, the x/y will move from that starting point as an example
```cpp
chinScreen_text("Hello World", 50, 50, "yellow", "large");
```

**Font Sizes:** `"small"`, `"medium"`, `"large"`, `"xlarge"`

---

## Shapes

All shapes support alignment and optional gradients:

**Vertical:** `"top"`, `"middle"`, `"bottom"`
**Horizontal:** `"left"`, `"center"`, `"right"`

### Rectangle
```cpp
lv_obj_t* rect = chinScreen_rectangle("blue", "white", 100, 50, "top", "center");
// With gradient
lv_obj_t* rect = chinScreen_rectangle("blue", "white", 100, 50, "top", "center", "cyan", "horizontal");
```

### Circle
```cpp
lv_obj_t* circle = chinScreen_circle("red", "white", 40, "middle", "left");
// With gradient
lv_obj_t* circle = chinScreen_circle("red", "white", 40, "middle", "left", "pink", "vertical");
```

### Triangle
```cpp
lv_obj_t* triangle = chinScreen_triangle("green", "black", 60, 50, "bottom", "right");
```

### Hexagon
```cpp
lv_obj_t* hex = chinScreen_hexagon("orange", "purple", 50, "middle", "center");
```

### Oval/Ellipse
```cpp
lv_obj_t* oval = chinScreen_oval("pink", "navy", 80, 40, "top", "left");
```

### Shape Text
Add text inside any shape:
```cpp
chinScreen_shape_text(circle, "OK", "white", "medium");
```

---

## Icons

### Static PNG Icons

Enable specific icon sets:
```cpp
#define CHINSCREEN_ICON_HOME
#define CHINSCREEN_ICON_SETTINGS
#define CHINSCREEN_ICON_USER
#define CHINSCREEN_ICON_MENU
#define CHINSCREEN_ICON_BACK
#define CHINSCREEN_ICON_FORWARD
#define CHINSCREEN_ICON_PLAY
#define CHINSCREEN_ICON_PAUSE
#define CHINSCREEN_ICON_STOP
#define CHINSCREEN_ICON_VOLUME
#define CHINSCREEN_ICON_POWER
#define CHINSCREEN_ICON_BATTERY
#define CHINSCREEN_ICON_SIGNAL
#define CHINSCREEN_ICON_WIFI
#define CHINSCREEN_ICON_DOWNLOAD
#define CHINSCREEN_ICON_UPLOAD
#define CHINSCREEN_ICON_REFRESH
#define CHINSCREEN_ICON_DELETE
#define CHINSCREEN_ICON_SAVE
#define CHINSCREEN_ICON_EDIT
#define CHINSCREEN_ICON_SEARCH
```

### Basic Icon Usage
```cpp
lv_obj_t* icon = chinScreen_icon("home", 1.0f, "middle", "center");
lv_obj_t* colored_icon = chinScreen_icon("settings", 1.5f, "top", "left", "red");
lv_obj_t* positioned_icon = chinScreen_icon("user", 1.0f, "middle", "center", "blue", 100, 200);
```

### Convenience Functions
```cpp
// Color shortcuts
lv_obj_t* red_home = chinScreen_icon_red("home", 1.0f, "middle", "center");
lv_obj_t* blue_settings = chinScreen_icon_blue("settings", 1.2f);
lv_obj_t* green_play = chinScreen_icon_green("play");

// Positioning shortcuts
lv_obj_t* positioned = chinScreen_icon_at("pause", 50, 100, 1.0f, "yellow");
lv_obj_t* grid_icon = chinScreen_icon_grid("stop", 0, 0, 3, 2, 80, 80, 120, 80, 1.0f, "purple");
```

### Change Icon Color
```cpp
chinScreen_icon_change_color(icon_obj, "green");
chinScreen_icon_set_color_advanced(icon_obj, "red", 128); // With opacity
```

### Icon Management
```cpp
chinScreen_list_available_icons();  // Print available icons to Serial
```

---

## Animated Icons

Enable animated icon sets:
```cpp
#define CHINSCREEN_ICON_AVATAR
#define CHINSCREEN_ICON_BOOK
#define CHINSCREEN_ICON_CLOSED
#define CHINSCREEN_ICON_GEAR
#define CHINSCREEN_ICON_LOADING
#define CHINSCREEN_ICON_OPEN
#define CHINSCREEN_ICON_TOUCH
#define CHINSCREEN_ICON_WIFI
```

### Usage
```cpp
lv_obj_t* loading = chinScreen_icon("loading-2", 1.0f, "middle", "center");
lv_obj_t* gear = chinScreen_icon("gear-1", 2.0f, "top", "left");
lv_obj_t* wifi_anim = chinScreen_icon("wifi-1", 1.5f);
```

---

## Media

### Images and GIFs
```cpp
// Static image
lv_obj_t* img = chinScreen_image_local(&my_image_dsc, 100, 100);

// GIF with zoom
lv_obj_t* gif = chinScreen_gif(gif_data, gif_size);
chinScreen_gif_zoom(gif, 1.5f);  // 150% size
chinScreen_gif_animate_zoom(gif, 1.0f, 2.0f, 1000);  // Animate zoom change

// Image from embedded data
lv_obj_t* img = chinScreen_image(&image_data, false, 50, 50);  // Static image
lv_obj_t* gif = chinScreen_image(&gif_data, true, -1, -1, "middle", "center");  // GIF
```

### Video Player (MJPEG)
```cpp
chinScreen_video_load("/video.mjpeg", 30);  // 30 FPS
chinScreen_video_play();
chinScreen_video_pause();
chinScreen_video_stop();
bool playing = chinScreen_video_is_playing();
```

---

## Input Controls

### Buttons
```cpp
lv_obj_t* btn = chinScreen_button("green", "white", 100, 50, "Click Me", button_callback, "middle", "center");

void button_callback(lv_event_t* e) {
    Serial.println("Button clicked!");
}
```

### Image Buttons
```cpp
lv_obj_t* img_btn = chinScreen_image_button("/path/to/image.png", callback, "middle", "center", 80, 60);
```

### Sliders
```cpp
lv_obj_t* slider = chinScreen_slider(200, 20, 0, 100, 50, "gray", "blue", "middle", "center");
int value = chinScreen_slider_get_value(slider);
```

### Toggle Switches
```cpp
lv_obj_t* toggle = chinScreen_toggle(true, "gray", "white", "middle", "center");
bool state = chinScreen_toggle_get_state(toggle);
```

### Virtual Keyboard
```cpp
lv_obj_t* keyboard = chinScreen_keyboard("Type here...", "bottom", "center");
const char* text = chinScreen_keyboard_get_text();
chinScreen_keyboard_close();
```

### Number Pad
```cpp
lv_obj_t* numpad = chinScreen_numpad("Enter number...", "center", "center");
const char* result = chinScreen_numpad_get_result();
int int_result = chinScreen_numpad_get_int();
float float_result = chinScreen_numpad_get_float();
```

---

## Animations

### Basic Animations
```cpp
chinScreen_animate(rect, ANIM_MOVE_X, 0, 100, 1000, true);
chinScreen_animate(circle, ANIM_ROTATE, 0, 360, 2000, false);
```

### Advanced Animations
```cpp
chinScreen_animate_advanced(circle, ANIM_ROTATE, 0, 360, 2000, true, "ease_in_out");
chinScreen_animate_advanced(rect, ANIM_FADE, 255, 100, 1000, false, "bounce");
```

**Animation Types:**
- `ANIM_MOVE_X` - Move horizontally
- `ANIM_MOVE_Y` - Move vertically  
- `ANIM_SCALE` - Scale width
- `ANIM_ROTATE` - Rotate shape
- `ANIM_FADE` - Fade opacity

**Easing Types:**
- `"linear"`, `"ease_in"`, `"ease_out"`, `"ease_in_out"`, `"bounce"`

### Animation Control
```cpp
chinScreen_stop_animations(object);
bool animating = chinScreen_is_animating(object);
```

### Progress Bars
```cpp
lv_obj_t* bar = chinScreen_progress_bar(200, 20, 50, "gray", "green", "middle", "center");
chinScreen_progress_set_value(bar, 75);
```

---

## Timer System

### Create and Control Timers
```cpp
void my_timer_callback(void* user_data) {
    Serial.println("Timer fired!");
}

chinScreen_timer_t timer = chinScreen_timer_create(my_timer_callback, 1000, nullptr);
chinScreen_timer_pause(timer);
chinScreen_timer_resume(timer);
chinScreen_timer_delete(timer);
```

### Timer Properties
```cpp
chinScreen_timer_set_period(timer, 500);    // Change to 500ms
uint32_t period = chinScreen_timer_get_period(timer);
bool running = chinScreen_timer_is_running(timer);
```

### Timer Speed Control
```cpp
chinScreen_timer_speed_up(timer, 200);      // Decrease period by 200ms
chinScreen_timer_slow_down(timer, 100);     // Increase period by 100ms
```

### Convenience Functions
```cpp
chinScreen_timer_t timer = chinScreen_repeat_every(1000, callback);
chinScreen_timer_t delayed = chinScreen_call_after(5000, callback);
chinScreen_timer_stop_all();
```

---

## Utilities

### Object Management
```cpp
chinScreen_set_position(object, 50, 50);
chinScreen_delete_object(object);
```

### SD Card Support
```cpp
bool success = chinScreen_init_sd_card(5);  // CS pin 5

// Preload images for better performance
int img_id = chinScreen_preload_image("/image.png");
lv_obj_t* img = chinScreen_show_preloaded_image(img_id, "middle", "center");
```

### Task Handler
```cpp
void loop() {
    chinScreen_task_handler();  // Replaces lv_task_handler()
    delay(5);
}
```

---

## Color System

### Supported Colors

The library supports 140+ HTML color names:

**Basic:** `black`, `white`, `red`, `green`, `blue`, `yellow`, `cyan`, `magenta`, `gray`, `orange`, `purple`, `pink`, `brown`, `navy`, `lime`

**Extended:** `aliceblue`, `antiquewhite`, `aqua`, `aquamarine`, `azure`, `beige`, `bisque`, `blanchedalmond`, `blueviolet`, `burlywood`, `cadetblue`, `chartreuse`, `chocolate`, `coral`, `cornflowerblue`, `cornsilk`, `crimson`, `darkblue`, `darkcyan`, `darkgoldenrod`, `darkgray`, `darkgreen`, `darkkhaki`, `darkmagenta`, `darkolivegreen`, `darkorange`, `darkorchid`, `darkred`, `darksalmon`, `darkseagreen`, `darkslateblue`, `darkslategray`, `darkturquoise`, `darkviolet`, `deeppink`, `deepskyblue`, `dimgray`, `dodgerblue`, `firebrick`, `floralwhite`, `forestgreen`, `fuchsia`, `gainsboro`, `ghostwhite`, `gold`, `goldenrod`, `greenyellow`, `honeydew`, `hotpink`, `indianred`, `indigo`, `ivory`, `khaki`, `lavender`, `lavenderblush`, `lawngreen`, `lemonchiffon`, `lightblue`, `lightcoral`, `lightcyan`, `lightgoldenrodyellow`, `lightgray`, `lightgreen`, `lightpink`, `lightsalmon`, `lightseagreen`, `lightskyblue`, `lightslategray`, `lightsteelblue`, `lightyellow`, `limegreen`, `linen`, `maroon`, `mediumaquamarine`, `mediumblue`, `mediumorchid`, `mediumpurple`, `mediumseagreen`, `mediumslateblue`, `mediumspringgreen`, `mediumturquoise`, `mediumvioletred`, `midnightblue`, `mintcream`, `mistyrose`, `moccasin`, `navajowhite`, `oldlace`, `olive`, `olivedrab`, `orangered`, `orchid`, `palegoldenrod`, `palegreen`, `paleturquoise`, `palevioletred`, `papayawhip`, `peachpuff`, `peru`, `plum`, `powderblue`, `rosybrown`, `royalblue`, `saddlebrown`, `salmon`, `sandybrown`, `seagreen`, `seashell`, `sienna`, `silver`, `skyblue`, `slateblue`, `slategray`, `snow`, `springgreen`, `steelblue`, `tan`, `teal`, `thistle`, `tomato`, `turquoise`, `violet`, `wheat`, `whitesmoke`, `yellowgreen`

### Color Utilities
```cpp
lv_color_t color = getColorByName("forestgreen");
uint8_t count = getColorCount();
const char* name = getColorNameByIndex(5);
```

---

## Debug Functions

```cpp
create_ui(1);  // Simple "chinScreen Works!" message
create_ui(2);  // Color test rectangles
chinScreen_debug_screen_info();  // Print screen dimensions
```

---

## Advanced Features

### Google Fonts (WIP)
```cpp
#define CHINSCREEN_ENABLE_GFONTS
const lv_font_t* font = chinScreen_load_google_font("Roboto", 16);
```

### Sprites (WIP)
```cpp
#define CHINSCREEN_ENABLE_SPRITES
#define CHINSCREEN_SPRITE_MAN
lv_obj_t* sprite = chinScreen_sprite("man_walk_forward-1", 1.0f, "middle", "center");
chinScreen_sprite_change(sprite, "man_forward");
```

---

## Example Usage

```cpp
#define CHINSCREEN_ENABLE_SHAPES
#define CHINSCREEN_ENABLE_ICONS
#define CHINSCREEN_ENABLE_ANIMATIONS
#define CHINSCREEN_ICON_HOME
#define CHINSCREEN_ICON_SETTINGS

#define LVGL_PORT_ROTATION_DEGREE 0
#include <chinScreen.h>

void setup() {
    Serial.begin(115200);
    init_display();
    
    // Rainbow background
    chinScreen_background_rainbow("vertical", "full");
    
    // Animated title
    chinScreen_text("My App", 160, 50, "white", "xlarge");
    
    // Home icon with animation
    lv_obj_t* home = chinScreen_icon_blue("home", 1.5f, "middle", "left");
    chinScreen_animate_advanced(home, ANIM_ROTATE, 0, 360, 3000, true, "ease_in_out");
    
    // Settings button
    lv_obj_t* btn = chinScreen_button("orange", "white", 120, 40, "Settings", settings_callback);
}

void settings_callback(lv_event_t* e) {
    chinScreen_clear();
    chinScreen_background_solid("darkblue");
    chinScreen_text("Settings Page", 160, 240, "white", "large");
}

void loop() {
    chinScreen_task_handler();
    delay(5);
}
```

---

## Notes

- All functions automatically handle display locking for thread safety
- Shapes support optional gradient backgrounds
- Icons support color recoloring (works best with white/light source icons)
- Animations support repeat and multiple easing curves
- Timer system provides simplified interface to LVGL timers
- Extensive color name support with case-insensitive matching

---

## Version History

- **v0.0.2b** - Added comprehensive icon system, animations, timers, multi-color gradients, enhanced media support
- **v0.0.1** - Initial release with basic shapes and text functions
