/*
 * functions.h
 *
 *  Created on: 25.02.2011
 *      Author: r0oland
 */

#include "WProgram.h" //Include Arduino headers
#include "macros.h"
#include "LiquidCrystal.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void print_lcd(LiquidCrystal* lcd);
void control_state(state *my_states, all_times *my_times);
void control_config(state *my_states, all_times *my_times);
void change_config_state(state *my_states, byte direction, DWORD *delay);
void init_my_times(all_times *my_times);

#endif /* FUNCTIONS_H_ */
