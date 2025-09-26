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
//

// Init's
#pragma once
#include <Arduino.h>
#include "includes/lvgl-8.3.11/src/lvgl.h"
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <ctype.h>
#include <SD.h>
#include <FS.h>

#ifdef CHINSCREEN_ENABLE_CURSIVE
#include "includes/fonts/cursive.inc"
#define FONT_CURSIVE &cursive
#endif

#ifdef CHINSCREEN_ENABLE_COMIC_REGULAR
#include "includes/fonts/comic_24.inc"
#define FONT_COMIC_REGULAR &comic_24
#endif

#ifdef CHINSCREEN_ENABLE_COMIC_SMALL
#include "includes/fonts/comic_18.inc"
#define FONT_COMIC_SMALL &comic_18
#endif

#ifdef CHINSCREEN_ENABLE_BRUSH_REGULAR
#include "includes/fonts/brush_24.inc"
#define FONT_BRUSH_REGULAR &brush_24
#endif

#ifdef CHINSCREEN_ENABLE_PEN_REGULAR
#include "includes/fonts/pen_24.inc"
#define FONT_PEN_REGULAR &pen_24
#endif

#ifdef CHINSCREEN_ENABLE_ICEBERG
#include "includes/fonts/iceberg_24.inc"
#define FONT_ICEBERG &iceberg_24
#endif

#ifdef CHINSCREEN_ENABLE_TYPEWRITER
#include "includes/fonts/typewriter.inc"
#define FONT_TYPEWRITER &typewriter
#endif

#ifdef CHINSCREEN_ENABLE_DOTS
#include "includes/fonts/dots.inc"
#define FONT_DOTS &dots
#endif

#ifdef CHINSCREEN_ENABLE_BEAST
#include "includes/fonts/beast.inc"
#define FONT_BEAST &beast
#endif

#ifdef CHINSCREEN_ENABLE_MODERN
#include "includes/fonts/modern.inc"
#define FONT_MODERN &modern
#endif

#ifdef CHINSCREEN_ENABLE_ALADIN
#include "includes/fonts/aladin.inc"
#define FONT_ALADIN &aladin
#endif

#ifdef CHINSCREEN_ENABLE_CREEPY
#include "includes/fonts/creepy.inc"
#define FONT_CREEPY &creepy
#endif

#ifdef CHINSCREEN_ENABLE_CYOA_SMALL
#include "includes/fonts/cyoa_18.inc"
#define FONT_CYOA_SMALL &cyoa_18
#endif

#ifdef CHINSCREEN_ENABLE_CYOA_REGULAR
#include "includes/fonts/cyoa_20.inc"
#define FONT_CYOA_REGULAR &cyoa_20
#endif

#ifdef CHINSCREEN_ENABLE_CYOA_LARGE
#include "includes/fonts/cyoa_24.inc"
#define FONT_CYOA_LARGE &cyoa_24
#endif




// Font size constants - need to add more fonts
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24
#define FONT_PIXEL_S &lv_font_unscii_8
#define FONT_PIXEL_B &lv_font_unscii_16


// special includes - do not change this order!!
#include "includes/colors.h"
#include "includes/basic.h"
#include "includes/inputs.h"
#include "includes/callbacks.h"
#include "includes/helpers.h"

// these can be added or removed
#ifdef CHINSCREEN_ENABLE_SHAPES
#include "includes/shapes.inc"
#endif

#ifdef CHINSCREEN_ENABLE_MEDIA
#include "includes/media.inc"
#endif

#ifdef CHINSCREEN_ENABLE_ICONS
#include "includes/icons.inc"
#endif

#ifdef CHINSCREEN_ENABLE_ANIMATED_ICONS
#include "includes/animated_icons.inc"
#endif

#ifdef CHINSCREEN_ENABLE_ANIMATIONS
#include "includes/animations.inc"
#endif

#ifdef CHINSCREEN_ENABLE_ANIMATED_BACKGROUNDS
#include "includes/animated_backgrounds.inc"
#endif

#ifdef CHINSCREEN_ENABLE_ANIMATED_OBJECTS 
//for objects overlay when using animated backgrounds
#include "includes/animation_objects.inc"
#endif

#ifdef CHINSCREEN_ENABLE_EXAMPLES
#include "includes/example-calls.inc"
#endif

#ifdef CHINSCREEN_ENABLE_DEBUG
#include "includes/debug.inc"
#endif

/////////////////////////////////////////////////////////////
// New Untested Items
/////////////////////////////////////////////////////////////

#ifdef CHINSCREEN_ENABLE_GFONTS
#include "includes/google_fonts.inc"
#endif

#ifdef CHINSCREEN_ENABLE_SPRITES
#include "includes/sprites.inc"
#endif