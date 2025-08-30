<img width="530" height="116" alt="image" src="https://github.com/user-attachments/assets/464fcc3b-f911-49a7-acf1-0e1cd95a92b2" />

**Stupid library for the JC3248W525EN using lvgl because it works**

> [!NOTE]
> Remember to use the following header to set the orientation:\
> #define LVGL_PORT_ROTATION_DEGREE 0\
> #include <chinScreen.h>

Basic Function Documentation:\
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
