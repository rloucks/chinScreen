/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 ******************************************************************************/
#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "../lvgl-8.3.11/lvgl.h"
#endif

#ifndef COMIC_18
#define COMIC_18 1
#endif
#if COMIC_18
/*-----------------
 *    BITMAPS
 *----------------*/
/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,
    /* U+0021 "!" */
    0x5a, 0x82,
    /* U+0022 "\"" */
    0xb4,
    /* U+0027 "'" */
    0xc0,
    /* U+0028 "(" */
    0x21, 0x49, 0x24, 0x91, 0x10,
    /* U+0029 ")" */
    0x81, 0x12, 0x49, 0x25, 0x40,
    /* U+002A "*" */
    0x23, 0xf7, 0x14,
    /* U+002B "+" */
    0x20, 0x82, 0x3f, 0x20, 0x80,
    /* U+002C "," */
    0x58,
    /* U+002D "-" */
    0xe0,
    /* U+002E "." */
    0x80,
    /* U+002F "/" */
    0x24, 0x84, 0x94, 0x90,
    /* U+0030 "0" */
    0x31, 0x18, 0x61, 0x86, 0x18, 0x5c,
    /* U+0031 "1" */
    0x75, 0x55,
    /* U+0032 "2" */
    0x72, 0x10, 0x44, 0x20, 0x4, 0x3f,
    /* U+0033 "3" */
    0x70, 0x10, 0x4c, 0x0, 0x18, 0x5c,
    /* U+0034 "4" */
    0x46, 0x18, 0x5c, 0x10, 0x41, 0x4,
    /* U+0035 "5" */
    0x7d, 0x8, 0x3c, 0x0, 0x10, 0x5c,
    /* U+0036 "6" */
    0x31, 0x8, 0x3c, 0x82, 0x18, 0x5c,
    /* U+0037 "7" */
    0xf8, 0x44, 0x40, 0x21, 0x8,
    /* U+0038 "8" */
    0x31, 0x14, 0x5c, 0x2, 0x18, 0x5c,
    /* U+0039 "9" */
    0x72, 0x18, 0x5f, 0x10, 0x41, 0x4,
    /* U+003A ":" */
    0xa0,
    /* U+003B ";" */
    0x41, 0x60,
    /* U+003D "=" */
    0xf8, 0x3e,
    /* U+003F "?" */
    0xe1, 0x16, 0x80, 0x8,
    /* U+0041 "A" */
    0xc, 0x44, 0x8a, 0x17, 0xf0, 0x60, 0xc1,
    /* U+0042 "B" */
    0x7c, 0x85, 0xf, 0xe8, 0x50, 0x60, 0xfe,
    /* U+0043 "C" */
    0xc, 0x84, 0x20, 0x82, 0x0, 0x1f,
    /* U+0044 "D" */
    0x7c, 0x85, 0xa, 0x18, 0x30, 0x61, 0x7c,
    /* U+0045 "E" */
    0x7d, 0x8, 0x3e, 0x82, 0x8, 0x1f,
    /* U+0046 "F" */
    0x7a, 0x21, 0xe8, 0x42, 0x10,
    /* U+0047 "G" */
    0x1f, 0x20, 0x40, 0x9f, 0x81, 0x80, 0x82, 0x7c,
    /* U+0048 "H" */
    0x42, 0x86, 0xf, 0xf8, 0x50, 0xa1, 0x42,
    /* U+0049 "I" */
    0x56, 0xaa,
    /* U+004A "J" */
    0x3e, 0x4, 0x8, 0x10, 0x20, 0xa1, 0x38,
    /* U+004B "K" */
    0x81, 0x6, 0x14, 0x9, 0xc, 0x1c, 0x27,
    /* U+004C "L" */
    0x41, 0x8, 0x20, 0x82, 0x8, 0x1f,
    /* U+004D "M" */
    0x43, 0x65, 0x5d, 0x91, 0x81, 0x81, 0x81, 0x81,
    /* U+004E "N" */
    0x43, 0x41, 0x61, 0x51, 0x51, 0x1, 0x85, 0x83,
    /* U+004F "O" */
    0xe, 0x21, 0x41, 0x1, 0x81, 0x81, 0x82, 0x7c,
    /* U+0050 "P" */
    0x7c, 0x85, 0xa, 0x64, 0x1e, 0x20, 0x40,
    /* U+0051 "Q" */
    0x3f, 0x1, 0x41, 0x81, 0x81, 0x82, 0x0, 0x7c,
    0x10, 0x6,
    /* U+0052 "R" */
    0x7c, 0x85, 0xa, 0x2f, 0x91, 0x21, 0x41,
    /* U+0053 "S" */
    0x3a, 0x20, 0xe0, 0x4, 0x3e,
    /* U+0054 "T" */
    0xfe, 0x20, 0x41, 0x2, 0x4, 0x8, 0x10,
    /* U+0055 "U" */
    0x42, 0x85, 0x8, 0x18, 0x30, 0x61, 0x38,
    /* U+0056 "V" */
    0x83, 0x6, 0x14, 0x28, 0x12, 0x14, 0x30,
    /* U+0057 "W" */
    0x41, 0x41, 0x81, 0x81, 0x91, 0xb2, 0xce, 0x82,
    /* U+0058 "X" */
    0x82, 0x65, 0x8, 0x20, 0x5, 0xa0,
    /* U+0059 "Y" */
    0x83, 0x6, 0x12, 0x23, 0x2, 0x4, 0x8,
    /* U+005A "Z" */
    0xe0, 0x70, 0x44, 0x20, 0x4, 0x3f,
    /* U+0061 "a" */
    0x31, 0x10, 0x61, 0x9d, 0x90,
    /* U+0062 "b" */
    0x82, 0xe, 0x24, 0x82, 0x18, 0x5c,
    /* U+0063 "c" */
    0x31, 0x0, 0x20, 0x85, 0xc0,
    /* U+0064 "d" */
    0x4, 0x13, 0xd1, 0x86, 0x19, 0xd9,
    /* U+0065 "e" */
    0x69, 0xe8, 0x7,
    /* U+0066 "f" */
    0x2b, 0xa4, 0x92,
    /* U+0067 "g" */
    0x31, 0x18, 0x5f, 0x4, 0x14, 0x4c,
    /* U+0068 "h" */
    0x41, 0x4, 0x3e, 0x82, 0x18, 0x61,
    /* U+0069 "i" */
    0xbe,
    /* U+006A "j" */
    0x20, 0x12, 0x49, 0x38,
    /* U+006B "k" */
    0x82, 0x8, 0x60, 0x91, 0x86, 0x14,
    /* U+006C "l" */
    0xff,
    /* U+006D "m" */
    0xbe, 0xd1, 0x91, 0x91, 0x91, 0x91,
    /* U+006E "n" */
    0xfa, 0x18, 0x61, 0x86, 0x10,
    /* U+006F "o" */
    0x72, 0x18, 0x61, 0x1, 0xc0,
    /* U+0070 "p" */
    0xb6, 0x63, 0xe8, 0x42, 0x10,
    /* U+0071 "q" */
    0x3d, 0x18, 0x5f, 0x4, 0x10, 0x41,
    /* U+0072 "r" */
    0xba, 0x49, 0x0,
    /* U+0073 "s" */
    0x78, 0x60, 0x1e,
    /* U+0074 "t" */
    0x5d, 0x24, 0x90,
    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x85, 0xc0,
    /* U+0076 "v" */
    0x86, 0x18, 0x4, 0x51, 0x80,
    /* U+0077 "w" */
    0x83, 0x6, 0xce, 0x8d, 0xd0, 0x80,
    /* U+0078 "x" */
    0x91, 0x86, 0x0, 0x92, 0x10,
    /* U+0079 "y" */
    0x46, 0x18, 0x5f, 0x4, 0x11, 0x18,
    /* U+007A "z" */
    0xf1, 0x20, 0x4f
};
/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/
static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 90, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 54, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 72, .box_w = 3, .box_h = 2, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 4, .adv_w = 36, .box_w = 1, .box_h = 2, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 5, .adv_w = 72, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 10, .adv_w = 72, .box_w = 3, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 15, .adv_w = 108, .box_w = 6, .box_h = 4, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 18, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 23, .adv_w = 54, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 24, .adv_w = 72, .box_w = 3, .box_h = 1, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 25, .adv_w = 36, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 72, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 30, .adv_w = 126, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 36, .adv_w = 54, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 56, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 108, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 73, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 85, .adv_w = 36, .box_w = 1, .box_h = 3, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 86, .adv_w = 54, .box_w = 2, .box_h = 6, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 88, .adv_w = 90, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 90, .adv_w = 90, .box_w = 4, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 108, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 121, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 127, .adv_w = 108, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 54, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 163, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 144, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 210, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 217, .adv_w = 108, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 236, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 144, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 126, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 264, .adv_w = 126, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 270, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 281, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 292, .adv_w = 90, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 72, .box_w = 3, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 304, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 36, .box_w = 1, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 72, .box_w = 3, .box_h = 10, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 315, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 36, .box_w = 1, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 144, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 333, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 108, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 343, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 349, .adv_w = 72, .box_w = 3, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 90, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 72, .box_w = 3, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 126, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 374, .adv_w = 108, .box_w = 6, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 379, .adv_w = 108, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 385, .adv_w = 90, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0}
};
/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/
static const uint8_t glyph_id_ofs_list_0[] = {
    0, 1, 2, 0, 0, 0, 0, 3,
    4, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 0, 24, 0, 25
};
/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 32, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 27,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 53,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};
/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;

static const lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 3,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
    .cache = &cache
};

/*-----------------
 *  PUBLIC FONT
 *----------------*/
/*Initialize a public general font descriptor*/
const lv_font_t comic_18 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
    .subpx = LV_FONT_SUBPX_NONE,
    .underline_position = -2,
    .underline_thickness = 1,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
    .user_data = NULL,
};
#endif /*#if COMIC_18*/


