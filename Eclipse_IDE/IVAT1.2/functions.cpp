#include "functions.h"
///////////////////////////////////////////////////////////////////////////////
//handles output on lcd display
void print_lcd(LiquidCrystal* lcd)
{
	lcd->clear();

}

///////////////////////////////////////////////////////////////////////////////
void control_state(state *my_states, all_times *my_times)
{
	switch (my_states->main)
	{
		case CONFIG_STATE:
			control_config(my_states, my_times);
			break;
		case DELAY_STATE:
			break;
		case SHOOTING_STATE:
			break;
		case SHOOTING_DONE:
			break;
		default:
			my_states->main = 0;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
void control_config(state *my_states, all_times *my_times)
{
	byte up = 1; //down == !up
	if ((digitalRead(UP)==HIGH)||(digitalRead(DOWN)==HIGH))
	{
		DWORD delay = millis();

		while(digitalRead(UP)==HIGH)
			change_config_state(my_states, up, &delay);

		while(digitalRead(DOWN)==HIGH)
			change_config_state(my_states, !up, &delay);
	}

}

///////////////////////////////////////////////////////////////////////////////
void change_config_state(state *my_states, byte direction, DWORD *delay)
{
	if ((millis() - *delay) > STATE_CHANGE_DELAY)
	{
		if(direction)
		{
			switch (my_states->config)
			{
				case OVERVIEW:
				case CHOOSE_INTERVAL:
				case CHOOSE_FOCUS:
					my_states->config++;
					break;

				case SET_DELAY:
				case SET_INTERVAL:
				case SET_AMOUNT:
				case SET_FOCUS_STYLE:
				case SET_FOCUS_TIME:
					my_states->config++;
					break;

				default:
					my_states->config = 0;
					break;
			}
		}
		else
		{

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void init_my_times(all_times *my_times)
{
	/*for (int i=0;i<7;i++)
	{
		time_array[i].raw = 0;
		time_array[i].days = 0;
		time_array[i].hours = 0;
		time_array[i].minutes = 0;
		time_array[i].seconds = 0;
		time_array[i].changed = false;
	}

	time_array[0].raw =*/


}


