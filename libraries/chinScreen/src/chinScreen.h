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

// Font size constants - need to add more fonts
#define FONT_SMALL   &lv_font_montserrat_12
#define FONT_MEDIUM  &lv_font_montserrat_16
#define FONT_LARGE   &lv_font_montserrat_20
#define FONT_XLARGE  &lv_font_montserrat_24


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