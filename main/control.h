#ifndef CONTROL_H
#define CONTROL_H

#include <inttypes.h>

extern uint8_t gamepads[4];

void set_player_state(int player, uint8_t state);
uint8_t get_player_state(int player);
void clear_all_player_state();
void init_button();

#endif
