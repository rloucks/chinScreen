<img width="289" height="116" alt="image" src="https://github.com/user-attachments/assets/b94c7160-a1c1-4d21-ba04-0b2e1dd884c5" />


**Robust Ardunio ESP32 (S3) library for the JC3248W525EN using LVGL 8.3.11 because it works, 9.3.x does not yet**

> [!NOTE]
> Remember to use the following header to set the orientation:\
> #define LVGL_PORT_ROTATION_DEGREE 0\
> #include <chinScreen.h>
>
> See readme.md in library folder for all functions\
> https://github.com/rloucks/chinScreen/blob/main/libraries/chinScreen/readme.md


> [!IMPORTANT]
> <img width="536" height="386" alt="image" src="https://github.com/user-attachments/assets/e92ab03c-7026-410d-8a15-3082b83bc8dd" />

## Feature Modules

Each feature is disabled by defaul and you must enable them. This is to save on memory space. Includes are marked as .inc file to avoid uneeded compiling.
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

# Basic Commands
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


<img width="377" height="578" alt="image" src="https://github.com/user-attachments/assets/6b3b12e1-1b45-4889-90c9-f3209ad7fb33" />

