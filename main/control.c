#include <inttypes.h>

uint8_t gamepads[4] = {0};

void set_player_state(int player, uint8_t state) {
    gamepads[player] = state;
}

uint8_t get_player_state(int player) {
    return gamepads[player];
}

void clear_all_player_state() {
    gamepads[0] = 0;
    gamepads[1] = 0;
    gamepads[2] = 0;
    gamepads[3] = 0;
}
