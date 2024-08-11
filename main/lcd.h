#ifndef LCD_H
#define LCD_H

#include "st7789.h"

extern TFT_t dev;
extern uint16_t last[160*160];

void ST7789(void *pvParameters);
TickType_t FillTest(TFT_t * dev, int width, int height);

#endif
