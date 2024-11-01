#include "iot_button.h"
#include <inttypes.h>

uint8_t gamepads[4] = {0};

void set_player_state(int player, uint8_t state) { gamepads[player] = state; }

uint8_t get_player_state(int player) { return gamepads[player]; }

void clear_all_player_state() {
  gamepads[0] = 0;
  gamepads[1] = 0;
  gamepads[2] = 0;
  gamepads[3] = 0;
}

extern void set_player_state(int player, uint8_t state);

static void button_left(void* arg, void* usr_data) { set_player_state(0, 16); }

static void button_right(void* arg, void* usr_data) { set_player_state(0, 32); }

static void button_up(void* arg, void* usr_data) { set_player_state(0, 64); }

static void button_down(void* arg, void* usr_data) { set_player_state(0, 128); }

static void button_x(void* arg, void* usr_data) { set_player_state(0, 1); }

void init_button() {
  // create gpio button
  {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = 100,
        .gpio_button_config =
            {
                .gpio_num = GPIO_NUM_2,
                .active_level = 0,
            },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn) {
      printf("Button create failed\n");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_up,
                           (void*)BUTTON_SINGLE_CLICK);
  }
  {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = 100,
        .gpio_button_config =
            {
                .gpio_num = GPIO_NUM_3,
                .active_level = 0,
            },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn) {
      printf("Button create failed\n");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_right,
                           (void*)BUTTON_SINGLE_CLICK);
  }
  {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = 100,
        .gpio_button_config =
            {
                .gpio_num = GPIO_NUM_10,
                .active_level = 0,
            },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn) {
      printf("Button create failed\n");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_left,
                           (void*)BUTTON_SINGLE_CLICK);
  }
  {
    button_config_t gpio_btn_cfg = {
        .type = BUTTON_TYPE_GPIO,
        .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
        .short_press_time = 100,
        .gpio_button_config =
            {
                .gpio_num = GPIO_NUM_11,
                .active_level = 0,
            },
    };
    button_handle_t gpio_btn = iot_button_create(&gpio_btn_cfg);
    if (NULL == gpio_btn) {
      printf("Button create failed\n");
    }
    iot_button_register_cb(gpio_btn, BUTTON_SINGLE_CLICK, button_down,
                           (void*)BUTTON_SINGLE_CLICK);
  }
  printf("unused button_x: %p\n", button_x);
}
