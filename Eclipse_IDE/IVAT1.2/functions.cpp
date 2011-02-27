#include "functions.h"

//---------> printLcd <------------------------
//handles output on lcd display
void print_lcd(LiquidCrystal* lcd)
{
	lcd->clear();

}

void control_state(byte *main_state)
{
	switch (*main_state)
	{
		case CONFIG_STATE:
			break;
		default:
			break;
	}
}
