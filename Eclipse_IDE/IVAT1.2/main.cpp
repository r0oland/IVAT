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

byte global_state_ = 0;

void PrintFocus(int *state);
void printLcd(int *state);
void (*print_lcd_)(int *state);

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

	while (true)
	{
		control_state(&global_state_);
	}
	return 0;
}
