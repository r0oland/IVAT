#include "WProgram.h" //Include arduino headers
#include "LiquidCrystal.h"
#include "macros.h"
#include "functions.h"

LiquidCrystal lcd(7, 5, 4, 3, 2, 1);

struct my_time
{
	DWORD raw;
	DWORD days;
	DWORD hours;
	DWORD minutes;
	DWORD seconds;
	int changed;
};

DWORD amount = START_AMOUNT; //number of pictures that should be taken
DWORD currentAmount = 0; //number of pictures allready taken

///MAIN///
int main()
{
	//Initialize the Arduino library
	init();

	lcd.begin(16, 2);

	pinMode(TRIGGER_PIN, OUTPUT);
	pinMode(FOCUS_PIN, OUTPUT);
	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);

	print_lcd(&lcd);

	byte main_state = 0;
	while (true)
	{
		control_state(&main_state);
	}
	return 0;
}
