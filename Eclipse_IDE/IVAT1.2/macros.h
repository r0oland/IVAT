////////////////////////////////////////////////////////////////////////////////
// Filename: macros.h
// Author:   Johannes Rebling
// Date:     February 27, 2011
//
// Description: This files defines my constants, special types and global
//              variables.
//
////////////////////////////////////////////////////////////////////////////////


#ifndef MACROS_H_
#define MACROS_H_

//---typedefs---////////////////////////////////////////////////////////////////
typedef unsigned long DWORD;

//---#defines---////////////////////////////////////////////////////////////////
//---push button pins---
#define UP 11 //button to next state
#define DOWN 0 //button to previous state
#define LEFT 13 //button to decrease quantity/time
#define RIGHT 12 //button to increase quantity/time
//---camera pins---
#define TRIGGER_PIN 10 //controls the transitor connected to the cam
#define FOCUS_PIN 9 //controls the focus of the cam
//---delay times---
#define STATE_CHANGE_DELAY 150 //delay between state changes in ms
#define VALUE_CHANGE_DELAY 100
#define VALUE_CHANGE_STEP_SIZE_DELAY 300

//---start values---
#define START_AMOUNT 100
#define START_INTERVAL 5
#define FOCUS_TIME 700

//Time camera needs to focus and trigger
#define TRIGGER_TIME 300
//the bigger FOCUS, the longer the cam has time to focus (kind of obvious)
#define FOCUS_TIME 700

//---enums---///////////////////////////////////////////////////////////////////
//used in print_Lcd() function and config_state
enum
{
	OVERVIEW,
	CHOOSE_INTERVAL,
	CHOOSE_FOCUS,
	SET_DELAY,
	SET_INTERVAL,
	SET_AMOUNT,
	SET_FOCUS_STYLE,
	SET_FOCUS_TIME
};
//used in control_state
enum
{
	CONFIG_STATE,
	DELAY_STATE,
	SHOOTING_STATE,
	SHOOTING_DONE
};
//focus_style YA=yes, always focus; YO=yes, focus once; NO=no focus
enum
{
	YA,
	YO,
	NO
};

//---structs---/////////////////////////////////////////////////////////////////
struct time
{
	DWORD raw;
	DWORD days;
	DWORD hours;
	DWORD minutes;
	DWORD seconds;
	boolean changed;
};

struct all_times
{
	time interval_time;
	time delay_time;
	time delay_time_temp;
	time duration;
	time remaining_time;
	time next_shoot;
	time shooting_ready;
};

struct state
{
	byte main;
	byte config;
	boolean interval;
	boolean focus;
	byte focus_style;
} my_states =
{ 0, 0, true, true, YA };

#endif /* MACROS_H_ */
