///////////////////////////////////////////////////////////////////////////
// Filename: macros.h
// Author:   Johannes Rebling
// Date:     February 27, 2011
//
// Description: This files defines my constants, special types and global
//              variables.
//
///////////////////////////////////////////////////////////////////////////

#ifndef MACROS_H_
#define MACROS_H_

//typedefs-----------------------------------------------------------------
typedef unsigned long DWORD;

//Button Pins--------------------------------------------------------------
#define UP 11 //button to next state
#define DOWN 0 //button to previous state
#define LEFT 13 //button to decrease quantity/time
#define RIGHT 12 //button to increase quantity/time

#define TRIGGER_PIN 10 //controls the transitor connected to the cam
#define FOCUS_PIN 9 //controls the focus of the cam

//Start Values for variables which aren't initialized as zero
#define START_AMOUNT 100
#define STATE_CHANGE_DELAY 150 //delay between state changes in ms
#define VALUE_CHANGE_DELAY 100
#define VALUE_CHANGE_STEP_SIZE_DELAY 300

//Time camera needs to focus and trigger
#define TRIGGER_TIME 300
//the bigger FOCUS, the longer the cam has time to focus (kind of obvious)
#define FOCUS_TIME 700

//names used for switch: case----------------------------------------------
//used in print_Lcd() function and config_state
enum { OVERVIEW, CHOOSE_INTERVAL, AMOUNT,
	INTERVAL_SETTING, FOCUS_SETTING, DELAY_SETTING };
enum { CONFIG_STATE, DELAY_STATE, SHOOTING_STATE, SHOOTING_DONE };

#endif /* MACROS_H_ */
