<img width="530" height="116" alt="image" src="https://github.com/user-attachments/assets/464fcc3b-f911-49a7-acf1-0e1cd95a92b2" />

**Stupid library for the JC3248W525EN using lvgl because it works**

> [!NOTE]
> Remember to use the following header to set the orientation:\
> #define LVGL_PORT_ROTATION_DEGREE 0\
> #include <chinScreen.h>

## Initialization
```
init_display()
```
Initializes the display panel, sets rotation, and turns on the backlight.

**init_display();**
Display Controls
Backlight Control
```
chinScreen_backlight(1); // On
chinScreen_backlight(0); // Off
```
Set Brightness
```
chinScreen_brightness(128); // Range 0-255
```

Initializes SD Card
```
chinScreen_init_sd_card();
```

---

## Backgrounds

Solid Color
```
chinScreen_background_solid("red");
```

Gradient Background
```
chinScreen_background_gradient("blue", "cyan", "horizontal");
```
---

## Text Functions

Draw Text
```
chinScreen_text("Hello World", 50, 50, "yellow", "large");
```

Font Sizes

Size	Function
small	"small"
medium	"medium"
large	"large"
xlarge	"xlarge"

---

## Shapes

All shapes support alignment:

Vertical: "top", "middle", "bottom"

Horizontal: "left", "center", "right"

Rectangle
```
lv_obj_t* rect = chinScreen_rectangle("blue", "white", 100, 50, "top", "center", "cyan", "horizontal");
```

Circle
```
lv_obj_t* circle = chinScreen_circle("red", "white", 40, "middle", "left");
```
Triangle

```
lv_obj_t* triangle = chinScreen_triangle("green", "black", 60, 50, "bottom", "right");
```


Hexagon
```
lv_obj_t* hex = chinScreen_hexagon("orange", "purple", 50, "middle", "center");
```

Oval / Ellipse
```
lv_obj_t* oval = chinScreen_oval("pink", "navy", 80, 40, "top", "left");
```

Shape Text
Add text inside a shape:

```
chinScreen_shape_text(circle, "OK", "white", "medium");
```

---

## Action Items

Buttons
```
lv_obj_t* btn = chinScreen_button("green", "white", 100, 50, "Click Me", example_button_callback, "middle", "center");
```
callback is a function with signature:

```
void example_button_callback(lv_event_t* e) {
    Serial.println("Button clicked!");
}
```

Sliders
```
lv_obj_t* slider = chinScreen_slider(200, 20, 0, 100, 50, "gray", "blue");
int val = chinScreen_slider_get_value(slider);
```



Toggle Switches
```
lv_obj_t* sw = chinScreen_toggle(true, "gray", "white");
bool state = chinScreen_toggle_get_state(sw);
```

---

## Animations
```
chinScreen_animate(rect, ANIM_MOVE_X, 0, 100, 1000, true);
```

Advanced Animations

```
chinScreen_animate_advanced(circle, ANIM_ROTATE, 0, 360, 2000, true, "ease_in_out");
```

Stop Animations
```
chinScreen_stop_animations(circle);
```

Check if Animating
```
bool running = chinScreen_is_animating(circle);
```

Animation Types

Type	Description
ANIM_MOVE_X	Move horizontally
ANIM_MOVE_Y	Move vertically
ANIM_SCALE	Scale width
ANIM_ROTATE	Rotate shape
ANIM_FADE	Fade opacity

Progress Bar
```
lv_obj_t* bar = chinScreen_progress_bar(200, 20, 50, "gray", "green");
```


---

## Utilities
Clear Screen

```
chinScreen_clear();
```

Set Position
```
chinScreen_set_position(rect, 50, 50);
```

Delete Object
```
chinScreen_delete_object(rect);
```

Example UI
```
create_ui(1); // Simple debug message
create_ui(2); // Test color rectangles
```

---

## Colors
Use the following color names:

black, white, red, green, blue, yellow, cyan, magenta, gray, orange, purple, pink, brown, navy, lime

---

## Notes
All drawing and animation functions automatically lock the display for thread safety.

Shapes can have optional gradient backgrounds.

Alignments follow LVGL conventions for easy placement.

Animations support repeat and easing paths.



