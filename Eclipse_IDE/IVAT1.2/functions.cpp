#include "functions.h"

///////////////////////////////////////////////////////////////////////////////
//handles output on lcd display
void print_lcd(LiquidCrystal* lcd)
{
	lcd->clear();
}

///////////////////////////////////////////////////////////////////////////////
void control_state(state *my_states, all_times *all_my_times)
{
	switch (my_states->main)
	{
	case CONFIG_STATE:
		control_config(my_states, all_my_times);
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
void control_config(state *my_states, all_times *all_my_times)
{
	byte up = 1; //down == !up
	if ((digitalRead(UP) == HIGH) || (digitalRead(DOWN) == HIGH))
	{
		DWORD delay = millis();

		while (digitalRead(UP) == HIGH)
			change_config_state(my_states, up, &delay);

		while (digitalRead(DOWN) == HIGH)
			change_config_state(my_states, !up, &delay);
	}

}

///////////////////////////////////////////////////////////////////////////////
void change_config_state(state *my_states, byte direction, DWORD *delay)
{
	if ((millis() - *delay) > STATE_CHANGE_DELAY)
	{
		if (direction)
		{
			switch (my_states->config)
			{
			case OVERVIEW:
			case CHOOSE_INTERVAL:
			case CHOOSE_FOCUS:
				my_states->config++;
				break;

			case SET_DELAY:
				if (my_states->interval)
				{
					my_states->config++;
				}
				else
				{
					my_states->focus ? my_states->config = SET_FOCUS_STYLE
							: my_states->config = OVERVIEW;
				}
				break;
			case SET_INTERVAL:
				my_states->config++;
				break;
			case SET_AMOUNT:
				my_states->focus ? my_states->config = SET_FOCUS_STYLE
						: my_states->config = OVERVIEW;
				break;
			case SET_FOCUS_STYLE:
			case SET_FOCUS_TIME:
			default:
				my_states->config = 0;
				break;
			}
		}
		else //direction==0==down
		{
			switch (my_states->config)
			{
			case OVERVIEW:
				if (my_states->focus)
				{
					my_states->config--;
				}
				else
				{
					my_states->interval ? my_states->config = SET_AMOUNT
							: my_states->config = SET_DELAY;
				}
				break;

			case CHOOSE_INTERVAL:
			case CHOOSE_FOCUS:
			case SET_DELAY:
			case SET_INTERVAL:
			case SET_AMOUNT:
				my_states->config--;
				break;
			case SET_FOCUS_STYLE:
				my_states->interval ? my_states->config = SET_AMOUNT
											: my_states->config = SET_DELAY;
			case SET_FOCUS_TIME:
				my_states->config--;
				break;
			default:
				my_states->config = 0;
				break;
			}
		}
		print_lcd();
	}
}

void init_all_my_times(all_times *all_my_times)
{
	all_my_times->interval_time.raw = START_INTERVAL;
	all_my_times->delay_time.raw = START_INTERVAL;
	all_my_times->focus_time.raw = START_INTERVAL;
}

