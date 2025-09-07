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



### Basic Function Documentation:

**init_display()**\
Starts the whole thing up

**create_ui(int)**\
Starts the UI, but allows for debugging\
  0 = Nothing\
  1 = Screen Text Test\
  2 = Color Test

**chinScreen_backlight(int)**\
Backlight control I think\
  0 = On\
  1 = Off
  
**chinScreen_brightness(int)**\
Set how bright the screen is\
  0-255 control

**chinScreen_rectangle(Background Color, Border Color, Width, Height, H Position, V Position);**\
**chinScreen_triangle(Background Color, Border Color, Width, Height, H Position, V Position);**\
**chinScreen_circle(Background Color, Border Color, radius, H Position, V Position);**\

<img width="377" height="578" alt="image" src="https://github.com/user-attachments/assets/6b3b12e1-1b45-4889-90c9-f3209ad7fb33" />

