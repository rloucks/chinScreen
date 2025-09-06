// chinScreen.h - Header for Arduino IDE
#ifndef CHINSCREEN_H
#define CHINSCREEN_H

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations
int chinScreen_init(void);
int chinScreen_write_area(int x1, int y1, int x2, int y2, uint8_t *data, size_t data_len);
int chinScreen_fill_screen(uint32_t color);
void chinScreen_deinit(void);

#ifdef __cplusplus
}
#endif

#endif // CHINSCREEN_H