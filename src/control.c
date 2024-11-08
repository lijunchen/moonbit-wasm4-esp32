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
