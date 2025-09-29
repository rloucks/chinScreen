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
//   Version: 0.3b
//
//   Hardware based for JC3248W525EN touch screen ESP32-S3 Panel
//
//

// Init's
#pragma once
#include <Arduino.h>
#include "includes/lvgl-8.3.11/src/lvgl.h"

#include "includes/lvgl-8.3.11/src/extra/libs/gif/lv_gif.h"
#include "display.h"
#include "esp_bsp.h"
#include "lv_port.h"
#include <ctype.h>
#include <SD.h>
#include <FS.h>

#ifdef CHINSCREEN_ENABLE_CURSIVE
#include "includes/fonts/cursive.inc"
#define FONT_CURSIVE &cursive
#include "includes/fonts/cursive_20.inc"
#define FONT_CURSIVE_20 &cursive_20
#include "includes/fonts/cursive_24.inc"
#define FONT_CURSIVE_24 &cursive_24
#endif

#ifdef CHINSCREEN_ENABLE_COMIC
#include "includes/fonts/comic_24.inc"
#define FONT_COMIC_18 &comic_24
#include "includes/fonts/comic_20.inc"
#define FONT_COMIC_20 &comic_20
#include "includes/fonts/comic_24.inc"
#define FONT_COMIC_24 &comic_24
#endif

#ifdef CHINSCREEN_ENABLE_BRUSH
#include "includes/fonts/brush_18.inc"
#define FONT_BRUSH_18 &brush_18
#include "includes/fonts/brush_20.inc"
#define FONT_BRUSH_20 &brush_20
#include "includes/fonts/brush_24.inc"
#define FONT_BRUSH_24 &brush_24
#endif

#ifdef CHINSCREEN_ENABLE_EMOJI
#include "includes/fonts/emoji_18.inc"
#define FONT_EMOJI_18 &emoji_18
#include "includes/fonts/emoji_20.inc"
#define FONT_EMOJI_20 &emoji_20
#include "includes/fonts/emoji_24.inc"
#define FONT_EMOJI_24 &emoji_24
#endif

#ifdef CHINSCREEN_ENABLE_BITJI
#include "includes/fonts/bitji_18.inc"
#define FONT_BITJI_18 &bitji_18
#include "includes/fonts/bitji_20.inc"
#define FONT_BITJI_20 &bitji_24
#include "includes/fonts/bitji_24.inc"
#define FONT_BITJI_24 &bitji_24
#endif

#ifdef CHINSCREEN_ENABLE_BLOCKHEAD
#include "includes/fonts/blockhead_18.inc"
#define FONT_BLOCKHEAD_18 &blockead_18
#include "includes/fonts/blockhead_20.inc"
#define FONT_BLOCKHEAD_20 &blockead_20
#include "includes/fonts/blockhead_24.inc"
#define FONT_BLOCKHEAD_24 &blockead_24
#endif

#ifdef CHINSCREEN_ENABLE_DOTS
#include "includes/fonts/dots.inc"
#define FONT_DOTS &dots
#include "includes/fonts/dots_20.inc"
#define FONT_DOTS_20 &dots_20
#include "includes/fonts/dots_24.inc"
#define FONT_DOTS_24 &dots_24
#endif

#ifdef CHINSCREEN_ENABLE_MONSTER
#include "includes/fonts/monster.inc"
#define FONT_MONSTER &monster
#include "includes/fonts/monster_20.inc"
#define FONT_MONSTER_20 &monster
#include "includes/fonts/monster_24.inc"
#define FONT_MONSTER_24 &monster_24
#endif

#ifdef CHINSCREEN_ENABLE_MODERN
#include "includes/fonts/modern.inc"
#define FONT_MODERN &modern
#include "includes/fonts/modern_20.inc"
#define FONT_MODERN_20 &modern_20
#include "includes/fonts/modern_24.inc"
#define FONT_MODERN_24 &modern_24
#endif

#ifdef CHINSCREEN_ENABLE_ALADIN
#include "includes/fonts/aladin.inc"
#define FONT_ALADIN &aladin
#include "includes/fonts/aladin_20.inc"
#define FONT_ALADIN_20 &aladin_20
#include "includes/fonts/aladin_24.inc"
#define FONT_ALADIN_24 &aladin_24
#endif

#ifdef CHINSCREEN_ENABLE_CREEPY
#include "includes/fonts/creepy_18.inc"
#define FONT_CREEPY_18 &creepy_18
#include "includes/fonts/creepy.inc"
#define FONT_CREEPY &creepy
#include "includes/fonts/creepy_24.inc"
#define FONT_CREEPY_24 &creepy_24
#endif



//Font size constants - need to add more fonts
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