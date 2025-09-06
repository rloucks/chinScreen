// lv_port.c - Fixed for LVGL 9.3.0 and Arduino
#include "lvgl.h"
#include "chinScreen.h"

// Display configuration
#define DISP_HOR_RES 320
#define DISP_VER_RES 240
#define DISP_BUF_SIZE (DISP_HOR_RES * DISP_VER_RES)

// Global display buffer - only one buffer to avoid conflicts
static lv_color_t *buf1;
static lv_display_t *disp;

// Transport buffer for communication (separate from LVGL buffer)
static uint8_t *transport_buffer;

// Forward declarations - FIXED: Correct signature for LVGL 9.3.0
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_map);

void lv_port_disp_init(void)
{
    // Allocate LVGL display buffer
    buf1 = (lv_color_t*)malloc(DISP_BUF_SIZE * sizeof(lv_color_t));
    if (buf1 == NULL) {
        printf("ERROR: Failed to allocate display buffer\n");
        return;
    }

    // Allocate separate transport buffer for communication
    transport_buffer = (uint8_t*)malloc(DISP_BUF_SIZE * 3); // RGB888 = 3 bytes per pixel
    if (transport_buffer == NULL) {
        printf("ERROR: Failed to allocate transport buffer\n");
        free(buf1);
        return;
    }

    // Create display
    disp = lv_display_create(DISP_HOR_RES, DISP_VER_RES);
    if (disp == NULL) {
        printf("ERROR: Failed to create display\n");
        free(buf1);
        free(transport_buffer);
        return;
    }

    // CRITICAL FIX: Set display buffers - using only buf1, buf2 set to NULL to avoid conflicts
    lv_display_set_buffers(disp, buf1, NULL, DISP_BUF_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);

    // FIXED: Set flush callback with correct signature
    lv_display_set_flush_cb(disp, disp_flush);
    
    printf("lv_port_disp_init completed successfully\n");
}

// FIXED: Display flush callback with correct LVGL 9.3.0 signature
static void disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *color_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t pixel_count = w * h;
    
    // Convert color data to RGB888 for transport
    // FIXED: Cast uint8_t* back to lv_color_t* for processing
    lv_color_t *from = (lv_color_t*)color_map;
    uint8_t *to = transport_buffer;
    
    for (uint32_t i = 0; i < pixel_count; i++) {
        lv_color_t c = from[i];
        
#if LV_COLOR_DEPTH == 16
        // Convert RGB565 to RGB888
        to[i * 3 + 0] = ((c.red << 3) | (c.red >> 2));     // Red
        to[i * 3 + 1] = ((c.green << 2) | (c.green >> 4)); // Green  
        to[i * 3 + 2] = ((c.blue << 3) | (c.blue >> 2));   // Blue
#elif LV_COLOR_DEPTH == 24
        // FIXED: Direct access to color channels (no .ch in LVGL 9.3.0)
        to[i * 3 + 0] = c.red;
        to[i * 3 + 1] = c.green;
        to[i * 3 + 2] = c.blue;
#else
        #error "Unsupported color depth"
#endif
    }
    
    // Send data to display using chinScreen library
    chinScreen_write_area(area->x1, area->y1, area->x2, area->y2, transport_buffer, pixel_count * 3);
    
    // Tell LVGL that flushing is done
    lv_display_flush_ready(disp);
}

// Cleanup function
void lv_port_disp_deinit(void)
{
    if (buf1) {
        free(buf1);
        buf1 = NULL;
    }
    
    if (transport_buffer) {
        free(transport_buffer);
        transport_buffer = NULL;
    }
}