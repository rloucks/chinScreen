/**
 * @file lv_conf.h
 * Configuration file for v9.3.0
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/* Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH 16

/* Transparent background support */
#define LV_COLOR_SCREEN_TRANSP 1

/* Images pixels with this color will not be drawn if they are chroma keyed */
#define LV_COLOR_CHROMA_KEY lv_color_hex(0x00ff00) /* pure green */

/*=========================
   MEMORY SETTINGS
 *=========================*/

/* Use custom malloc/free */
#define LV_MEM_CUSTOM 1
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h>
#define LV_MEM_CUSTOM_ALLOC   malloc
#define LV_MEM_CUSTOM_FREE    free
#define LV_MEM_CUSTOM_REALLOC realloc

/* Max intermediate memory buffers used internally */
#define LV_MEM_BUF_MAX_NUM 16

/* Use system memcpy/memset */
#define LV_MEMCPY_MEMSET_STD 1

/*====================
   HAL SETTINGS
 *====================*/

#define LV_DISP_DEF_REFR_PERIOD 30      /* ms */
#define LV_INDEV_DEF_READ_PERIOD 30     /* ms */

/* Default DPI */
#define LV_DPI_DEF 130

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

#define LV_DRAW_COMPLEX 1

#define LV_LAYER_SIMPLE_BUF_SIZE          (24 * 1024)
#define LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE (3 * 1024)

#define LV_IMG_CACHE_DEF_SIZE 1

/*=====================
 *  LOGGING & ASSERTS
 *====================*/

#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 1
#endif

#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_OBJ           0

/*==================
 *   FONT USAGE
 *==================*/

#define LV_FONT_MONTSERRAT_8   1
#define LV_FONT_MONTSERRAT_10  1
#define LV_FONT_MONTSERRAT_12  1
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_16  1
#define LV_FONT_MONTSERRAT_18  1
#define LV_FONT_MONTSERRAT_20  1
#define LV_FONT_MONTSERRAT_22  1
#define LV_FONT_MONTSERRAT_24  1
#define LV_FONT_MONTSERRAT_26  1
#define LV_FONT_MONTSERRAT_28  1
#define LV_FONT_MONTSERRAT_30  1
#define LV_FONT_MONTSERRAT_32  1
#define LV_FONT_MONTSERRAT_34  1
#define LV_FONT_MONTSERRAT_36  1
#define LV_FONT_MONTSERRAT_38  1
#define LV_FONT_MONTSERRAT_40  1
#define LV_FONT_MONTSERRAT_42  1
#define LV_FONT_MONTSERRAT_44  1
#define LV_FONT_MONTSERRAT_46  1
#define LV_FONT_MONTSERRAT_48  1

#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 *  TEXT SETTINGS
 *=================*/

#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_"
#define LV_TXT_COLOR_CMD "#"

/*==================
 *  WIDGET USAGE
 *=================*/

#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BTN        1
#define LV_USE_BTNMATRIX  1
#define LV_USE_CANVAS     1
#define LV_USE_CHECKBOX   1
#define LV_USE_DROPDOWN   1
#define LV_USE_IMG        1
#define LV_USE_LABEL      1
#define LV_USE_LINE       1
#define LV_USE_ROLLER     1
#define LV_USE_SLIDER     1
#define LV_USE_SWITCH     1
#define LV_USE_TEXTAREA   1
#define LV_USE_TABLE      1

/* Extra widgets */
#define LV_USE_ANIMIMG    1
#define LV_USE_CALENDAR   1
#define LV_USE_CHART      1
#define LV_USE_COLORWHEEL 1
#define LV_USE_IMGBTN     1
#define LV_USE_KEYBOARD   1
#define LV_USE_LED        1
#define LV_USE_LIST       1
#define LV_USE_MENU       1
#define LV_USE_METER      1
#define LV_USE_MSGBOX     1
#define LV_USE_SPAN       1
#define LV_USE_SPINBOX    1
#define LV_USE_SPINNER    1
#define LV_USE_TABVIEW    1
#define LV_USE_TILEVIEW   1
#define LV_USE_WIN        1  /* still available in 9.3 */

/*==================
 *  THEMES
 *=================*/

#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
    #define LV_THEME_DEFAULT_DARK 0
    #define LV_THEME_DEFAULT_GROW 1
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif

/*==================
 *  LAYOUTS
 *=================*/

#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/*==================
 *  FILESYSTEM & IMG DECODERS
 *=================*/

#define LV_USE_FS_STDIO 1
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER 'S'
    #define LV_FS_STDIO_PATH ""
    #define LV_FS_STDIO_CACHE_SIZE 0
#endif

#define LV_USE_PNG 1
#define LV_USE_BMP 1
#define LV_USE_GIF 1
#define LV_USE_QRCODE 1

/*==================
 *  MISC
 *=================*/

#define LV_USE_SNAPSHOT 1

#endif /*LV_CONF_H*/

#endif /*Enable content*/
