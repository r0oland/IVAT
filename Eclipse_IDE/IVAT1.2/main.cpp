#include "WProgram.h" //Include arduino headers
#include "LiquidCrystal.h"
#include "macros.h"
#include "functions.h"

LiquidCrystal lcd(7, 5, 4, 3, 2, 1);

DWORD amount = START_AMOUNT; //number of pictures that should be taken
DWORD currentAmount = 0; //number of pictures allready taken

///MAIN///
int main()
{
	//initialize the Arduino library
	init();

	//initialize lcd display
	lcd.begin(16, 2);

	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(FOCUS_PIN, OUTPUT);
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);

	//display the default setting in overview state
	print_lcd(&lcd); //

	state my_states =	{ 0, 0, true, true, YA };
	all_times all_my_times;
	memset (&all_my_times, 0 ,sizeof(all_my_times));

	init_all_my_times(&all_my_times);

	while (true)
	{
		control_state(&my_states, &all_my_times);
	}
	return 0;
}


