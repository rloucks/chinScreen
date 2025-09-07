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
#include "includes/shapes.h"
#include "includes/media.h"
#include "includes/animations.h"
#include "includes/google_fonts.h"
#include "includes/example-calls.h"
#include "includes/debug.h"


/////////////////////////////////////////////////////////////
// New Untested Items
/////////////////////////////////////////////////////////////
