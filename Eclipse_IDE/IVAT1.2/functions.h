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
void control_state(byte *main_state);

#endif /* FUNCTIONS_H_ */
