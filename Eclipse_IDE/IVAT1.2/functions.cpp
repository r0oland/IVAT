#include "functions.h"
#include "macros.h"

//---------> printLcd <------------------------
//handles output on lcd display
void print_lcd(LiquidCrystal* lcd)
{
	lcd->clear();

}

void control_state(byte *global_state_)
{
	switch (*global_state_)
	{
		case CONFIG_STATE:

			break;
		default:
			break;
	}
}
