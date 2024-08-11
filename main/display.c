#include <stdio.h>

#include "window.h"
#include "runtime.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "st7789.h"
#include "lcd.h"
#include "esp_task_wdt.h"

// static uint32_t pixels[160*160];

static int viewportX = 0;
static int viewportY = 0;
static int viewportSize = 3*160;

void w4_windowBoot () {
    int counter = 0;
    do {
        // Player 1
        uint8_t gamepad = 0;
        w4_runtimeSetGamepad(0, gamepad);

        // Player 2
        gamepad = 0;
        w4_runtimeSetGamepad(1, gamepad);

        // Mouse handling
        uint8_t mouseButtons = 0;
        int mouseX = 0;
        int mouseY = 0;
        w4_runtimeSetMouse(160*(mouseX-viewportX)/viewportSize, 160*(mouseY-viewportY)/viewportSize, mouseButtons);
        w4_runtimeUpdate();
    } while (1);
}

int counter = 0;

uint16_t convert(uint32_t c) {
    uint16_t color = rgb565(
        (c & 0xff0000) >> 16,
        (c & 0xff00) >> 8,
         c & 0xff
    );
    return color;
}


void w4_windowComposite (const uint32_t* palette, const uint8_t* framebuffer) {
    // Convert indexed 2bpp framebuffer to XRGB output
    // uint32_t* out = pixels;
    for (int n = 0; n < 160*160/4; ++n) {
        uint8_t quartet = framebuffer[n];
        int color1 = (quartet & 0b00000011) >> 0;
        int color2 = (quartet & 0b00001100) >> 2;
        int color3 = (quartet & 0b00110000) >> 4;
        int color4 = (quartet & 0b11000000) >> 6;

        uint16_t c1 = convert(palette[color1]);
        uint16_t c2 = convert(palette[color2]);
        uint16_t c3 = convert(palette[color3]);
        uint16_t c4 = convert(palette[color4]);
        uint16_t cs[4] = {c1, c2, c3, c4};

        for (int i = n * 4; i < n * 4 + 4; i++) {
            int x = i % 160;
            int y = i / 160;
            uint16_t c = cs[i - n * 4];
            if (c != last[i]) {
                lcdDrawPixel(&dev, x + 40, y + 40, c);
                last[i] = c;
            }
        }
    }
}
