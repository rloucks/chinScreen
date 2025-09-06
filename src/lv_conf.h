#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   COLOR SETTINGS
 *====================*/
#define LV_COLOR_DEPTH 24
#define LV_COLOR_16_SWAP 0
#define LV_COLOR_SCREEN_TRANSP 0
#define LV_COLOR_CHROMA_KEY lv_color_hex(0x00ff00)

/*=========================
   MEMORY SETTINGS
 *=========================*/
#define LV_MEM_CUSTOM 1
#if LV_MEM_CUSTOM == 0
    #define LV_MEM_SIZE (48U * 1024U)
    #define LV_MEM_ADR 0
    #if LV_MEM_ADR == 0
        #undef LV_MEM_POOL_INCLUDE
        #undef LV_MEM_POOL_ALLOC
    #endif
#else
    #define LV_MEM_CUSTOM_INCLUDE <stdlib.h>
    #define LV_MEM_CUSTOM_ALLOC   malloc
    #define LV_MEM_CUSTOM_FREE    free
    #define LV_MEM_CUSTOM_REALLOC realloc
#endif

/*====================
   HAL SETTINGS
 *====================*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR millis()
#endif

/*================
 * LOG SETTINGS
 *================*/
#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
    #define LV_LOG_PRINTF 1
    #define LV_LOG_USE_TIMESTAMP 1
    #define LV_LOG_TRACE_MEM       1
    #define LV_LOG_TRACE_TIMER     1
    #define LV_LOG_TRACE_INDEV     1
    #define LV_LOG_TRACE_DISP_REFR 1
    #define LV_LOG_TRACE_EVENT     1
    #define LV_LOG_TRACE_OBJ_CREATE 1
    #define LV_LOG_TRACE_LAYOUT    1
    #define LV_LOG_TRACE_ANIM      1
    #define LV_LOG_TRACE_CACHE     1
#endif

/*=================
 * ASSERT SETTINGS
 *=================*/
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         1
#define LV_USE_ASSERT_MEM_INTEGRITY 1
#define LV_USE_ASSERT_OBJ           1

/*==================
 * COMPILER SETTINGS
 *==================*/
#define LV_COMPILER_VLA_SUPPORTED 1
#define LV_COMPILER_TYPEOF_COMPATIBLE 1

/*====================
 * FEATURE CONFIGURATION
 *====================*/

/*-------------
 * Drawing
 *-----------*/
#define LV_DRAW_SW 1
#if LV_DRAW_SW == 1
    #define LV_DRAW_SW_ASM LV_DRAW_SW_ASM_NONE
    #define LV_DRAW_SW_UNROLL_FILTER_ROWS 1
    #define LV_DRAW_SW_UNROLL_RADIUS_FILTER 1
    #define LV_DRAW_SW_COMPLEX 1
    #define LV_DRAW_SW_SHADOW_CACHE_SIZE 0
    #define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4
    #define LV_DRAW_SW_GRADIENT_CACHE_DEF_SIZE 0
    #define LV_DRAW_SW_GRADIENT_STOP_COUNT_LIMIT 16
#endif

#define LV_USE_GPU_ARM2D 0
#define LV_USE_GPU_STM32_DMA2D 0
#define LV_USE_GPU_SWM341_DMA2D 0
#define LV_USE_GPU_NXP_PXP 0
#define LV_USE_GPU_NXP_VG_LITE 0
#define LV_USE_GPU_SDL 0

/*-----------
 * Logging
 *-----------*/
#define LV_USE_LOG 1

/*-----------
 * Asserts
 *-----------*/
#define LV_USE_ASSERT_NULL      1
#define LV_USE_ASSERT_MALLOC    1
#define LV_USE_ASSERT_STYLE     0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ       0

/*-------------
 * Others
 *-----------*/
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0
#define LV_USE_REFR_DEBUG   0
#define LV_USE_USER_DATA    1
#define LV_ENABLE_GC        0

/*===================
 * FONT USAGE
 *==================*/
#define LV_FONT_MONTSERRAT_8  1
#define LV_FONT_MONTSERRAT_10 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_22 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_26 1
#define LV_FONT_MONTSERRAT_28 1
#define LV_FONT_MONTSERRAT_30 1
#define LV_FONT_MONTSERRAT_32 1
#define LV_FONT_MONTSERRAT_34 1
#define LV_FONT_MONTSERRAT_36 1
#define LV_FONT_MONTSERRAT_38 1
#define LV_FONT_MONTSERRAT_40 1
#define LV_FONT_MONTSERRAT_42 1
#define LV_FONT_MONTSERRAT_44 1
#define LV_FONT_MONTSERRAT_46 1
#define LV_FONT_MONTSERRAT_48 1

#define LV_FONT_MONTSERRAT_28_COMPRESSED 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_16_CJK            0

#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

#define LV_FONT_DEFAULT &lv_font_montserrat_14

#define LV_FONT_FMT_TXT_LARGE   1
#define LV_USE_FONT_COMPRESSED  1
#define LV_USE_FONT_SUBPX       1
#if LV_USE_FONT_SUBPX
    #define LV_FONT_SUBPX_BGR    0
#endif

#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 * TEXT SETTINGS
 *===============*/
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_TXT_BREAK_CHARS " ,.;:-_"
#define LV_TXT_LINE_BREAK_LONG_LEN 0
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3
#define LV_TXT_COLOR_CMD "#"
#define LV_USE_BIDI 0
#if LV_USE_BIDI
    #define LV_BIDI_BASE_DIR_DEF  LV_BASE_DIR_AUTO
#endif
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*===================
 * WIDGET USAGE
 *==================*/
#define LV_USE_WIDGETS_BUILTIN 1
#if LV_USE_WIDGETS_BUILTIN
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
    #define LV_USE_LIST       1
    #define LV_USE_MENU       1
    #define LV_USE_METER      1
    #define LV_USE_MSGBOX     1
    #define LV_USE_ROLLER     1
    #define LV_USE_SLIDER     1
    #define LV_USE_SPAN       1
    #define LV_USE_SPINBOX    1
    #define LV_USE_SPINNER    1
    #define LV_USE_SWITCH     1
    #define LV_USE_TEXTAREA   1
    #define LV_USE_TABLE      1
    #define LV_USE_TABVIEW    1
    #define LV_USE_TILEVIEW   1
    #define LV_USE_WIN        1
#endif

/*==================
 * THEME USAGE
 *================*/
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
    #define LV_THEME_DEFAULT_DARK     0
    #define LV_THEME_DEFAULT_GROW     1
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif
#define LV_USE_THEME_BASIC    1
#define LV_USE_THEME_MONO     1

/*==================
 * LAYOUTS
 *================*/
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/*==================
 * 3RD PARTY LIBRARIES
 *================*/

/*File system interfaces*/
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER '\0'
    #define LV_FS_STDIO_PATH ""
    #define LV_FS_STDIO_CACHE_SIZE 0
#endif

#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER '\0'
    #define LV_FS_POSIX_PATH ""
    #define LV_FS_POSIX_CACHE_SIZE 0
#endif

#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER '\0'
    #define LV_FS_WIN32_PATH ""
    #define LV_FS_WIN32_CACHE_SIZE 0
#endif

#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER '\0'
    #define LV_FS_FATFS_CACHE_SIZE 0
#endif

/*PNG decoder library*/
#define LV_USE_PNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/*JPG + split JPG decoder library*/
#define LV_USE_SJPG 0

/*GIF decoder library*/
#define LV_USE_GIF 0

/*QR code library*/
#define LV_USE_QRCODE 0

/*FreeType library*/
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
    #define LV_FREETYPE_CACHE_SIZE (16 * 1024)
    #define LV_FREETYPE_SBIT_CACHE 0
    #define LV_FREETYPE_CACHE_FT_FACES 0
    #define LV_FREETYPE_CACHE_FT_SIZES 0
    #define LV_FREETYPE_CACHE_FT_GLYPH 0
#endif

/*Rlottie library*/
#define LV_USE_RLOTTIE 0

/*FFmpeg library for image decoding and playing videos*/
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
    #define LV_FFMPEG_AV_DUMP_FORMAT 0
#endif

/*==================
 * OTHERS
 *================*/

/*1: Enable API to take snapshot for object*/
#define LV_USE_SNAPSHOT 0

/*1: Enable Monkey test*/
#define LV_USE_MONKEY 0

/*1: Enable grid navigation*/
#define LV_USE_GRIDNAV 0

/*1: Enable lv_obj fragment*/
#define LV_USE_FRAGMENT 0

/*1: Support using images as font in label or span widgets */
#define LV_USE_IMGFONT 0

/*1: Enable a published subscriber based messaging system */
#define LV_USE_MSG 0

/*1: Enable Pinyin input method*/
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
    #define LV_IME_PINYIN_USE_K9_MODE      1
    #define LV_IME_PINYIN_USE_HELP_PAGE    1
    #define LV_IME_PINYIN_DICT "lv_100k_dict.bin"
    #define LV_IME_PINYIN_CAND_TEXT_NUM    6
#endif

/*1: Enable file explorer*/
#define LV_USE_FILE_EXPLORER                     0
#if LV_USE_FILE_EXPLORER
    #define LV_FILE_EXPLORER_PATH_MAX_LEN        (128)
    #define LV_FILE_EXPLORER_QUICK_ACCESS        1
#endif

/*==================
 * EXAMPLES
 *================*/
#define LV_BUILD_EXAMPLES 1

/*Enable the examples to be built with the library*/
#define LV_USE_DEMO_WIDGETS    0
#if LV_USE_DEMO_WIDGETS
    #define LV_DEMO_WIDGETS_SLIDESHOW 0
#endif

#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0
#define LV_USE_DEMO_BENCHMARK          0
#define LV_USE_DEMO_STRESS             0
#define LV_USE_DEMO_MUSIC              0
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/