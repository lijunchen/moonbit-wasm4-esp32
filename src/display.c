// #include <GLFW/glfw3.h>

#include <wasm_export.h>

#include <stdint.h>
#include <stdio.h>

#include "runtime.h"

extern wasm_module_t wasm_module;
extern wasm_module_inst_t wasm_module_inst;
extern wasm_function_inst_t start;
extern wasm_function_inst_t update;
extern wasm_exec_env_t exec_env;

int64_t frame_start = 0;
int64_t frame_end = 0;
int last_fps = 0;

uint32_t stop = 0;

// extern GLFWwindow* window;

void w4_windowBoot() {
  int counter = 0;
  do {
    if (stop) {
      printf("[WAMS4] stopping\n");
      break;
    }
    counter++;
    if (!wasm_module_inst || !start || !update || !exec_env) {
      continue;
    }
    // Player 1
    // uint8_t gamepad = get_player_state(0);
    // w4_runtimeSetGamepad(0, gamepad);

    // Player 2
    // gamepad = get_player_state(1);
    // printf("player 2 state: %d\n", gamepad);
    // w4_runtimeSetGamepad(1, gamepad);
    // clear_all_player_state();

    w4_runtimeUpdate();
  } while (1);
}

#define rgb565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))

uint16_t convert(uint32_t c) {
  uint16_t color = rgb565((c & 0xff0000) >> 16, (c & 0xff00) >> 8, c & 0xff);
  return color;
}

uint8_t g(const uint8_t* framebuffer, int x, int y) {
  int n = y * 160 + x;
  int index = n / 4;
  int m = index % 4;
  uint8_t quartet = framebuffer[index];
  if (m == 0) {
    return quartet & 0b00000011;
  } else if (m == 1) {
    return (quartet & 0b00001100) >> 2;
  } else if (m == 2) {
    return (quartet & 0b00110000) >> 4;
  } else {
    return (quartet & 0b11000000) >> 6;
  }
}

uint32_t pixels[160 * 160];

void set_pixel(int x, int y, uint32_t color) {
  if (x >= 0 && x < 160 && y >= 0 && y < 160) {
    pixels[y * 160 + x] = color;
  }
}

void w4_windowComposite(const uint32_t* palette, const uint8_t* framebuffer) {
  // Convert indexed 2bpp framebuffer to XRGB output
  // uint32_t* out = pixels;
  uint16_t colors[4] = {convert(palette[0]), convert(palette[1]),
                        convert(palette[2]), convert(palette[3])};

  char fps[4];
  sprintf(fps, "%d", last_fps);
  w4_runtimeText(fps, 140, 0);
  for (int n = 0; n < 160 * 160 / 4; ++n) {
    uint8_t quartet = framebuffer[n];
    int color1 = (quartet & 0b00000011) >> 0;
    int color2 = (quartet & 0b00001100) >> 2;
    int color3 = (quartet & 0b00110000) >> 4;
    int color4 = (quartet & 0b11000000) >> 6;

    uint16_t c1 = colors[color1];
    uint16_t c2 = colors[color2];
    uint16_t c3 = colors[color3];
    uint16_t c4 = colors[color4];

    int x;
    int y;
    x = (n * 4) % 160;
    y = (n * 4) / 160;
    set_pixel(x, y, c1);

    x = (n * 4 + 1) % 160;
    y = (n * 4 + 1) / 160;
    set_pixel(x, y, c2);

    x = (n * 4 + 2) % 160;
    y = (n * 4 + 2) / 160;
    set_pixel(x, y, c3);

    x = (n * 4 + 3) % 160;
    y = (n * 4 + 3) / 160;
    set_pixel(x, y, c4);
  }
}
