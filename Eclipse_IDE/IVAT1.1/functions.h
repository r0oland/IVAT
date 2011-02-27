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

void modulator(unsigned long* time);
void valueChanger (unsigned long *identifier);
void shooting();
int find_first_entry(uint32_t time_array[]);
void time_printer(LiquidCrystal* lcd, uint32_t *displayTime);
void state_changer(int *state, int *interval_);

#endif /* FUNCTIONS_H_ */
