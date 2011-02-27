#include "functions.h"

//---------> Modulator <------------------------
/*takes array with time in seconds and compute days, hours...
 time[]=={days, hours, minutes, seconds}*/
void modulator(unsigned long* time)
{
	unsigned long seconds[] =
	{ 86400, 3600, 60 };

	for (int i = 1; i <= 4; i++)
	{
		time[i] = time[i - 1] % seconds[i - 1];
	}

	for (int n = 3; n > 0; n--)
	{
		time[n] = time[n - 1] / seconds[n - 1];
	}
}

//---------> changer <------------------------
void valueChanger(int *state, unsigned long *value_to_change, void* print_lcd_)
{
	unsigned long changeTimeIdent = millis(); //used to check time between settings
	unsigned long changeTimeDoDo = millis(); //tribute to my friend Dodo who helped me a lot

	int step_size = 1;

	while (digitalRead(RIGHT) == HIGH || digitalRead(LEFT) == HIGH)
	{
		if ((millis() - changeTimeIdent) > (VALUE_CHANGE_DELAY))
		{
			if (digitalRead(RIGHT) == HIGH)
			{
				if ((*value_to_change + step_size) < 99999999)
					*value_to_change += step_size;
				else
					*value_to_change = 99999999;
			}
			if (digitalRead(LEFT) == HIGH)
			{
				if ((*value_to_change - step_size) < 99999999)
					*value_to_change -= step_size;
				else
					*value_to_change = 0;
			}

			if ((millis() - changeTimeDoDo) > (VALUE_CHANGE_STEP_SIZE_DELAY))
			{
				step_size++;
				changeTimeDoDo = millis();
			}

			print_lcd_(state);
			changeTimeIdent = millis();
		}
	}
}

//---------> StateChanger <------------------------
void state_changer(int *state, int *interval_)
{
	if (digitalRead(DOWN) == HIGH)
	{
		if ((*state == 1) && !*interval_)
			*state = 4;
		else if (*state < 5)
			*state++;
		else
			*state = 0;
	}
	if (digitalRead(UP) == HIGH)
	{
		if ((*state == 4) && !*interval_)
			*state = 1;
		else if (*state > 0)
			*state--;
		else
			*state = 5;
	}
}

//---------> shooting <------------------------
void shooting()
{
	int ftState = 0;
	unsigned long focusTimeCheck = 0;
	unsigned long triggerTimeCheck = 0;

	while (1)
	{
		switch (ftState)
		{
		case 0:
			digitalWrite(FOCUS_PIN, HIGH);
			focusTimeCheck = millis();
			ftState++;
			break;

		case 1:
			if ((millis() - focusTimeCheck) > FOCUS_TIME)
			{
				digitalWrite(FOCUS_PIN, LOW);
				ftState++;
			}
			break;

		case 2:
			digitalWrite(TRIGGER_PIN, HIGH);
			triggerTimeCheck = millis();
			ftState++;
			break;

		case 3:
			if ((millis() - triggerTimeCheck) > TRIGGER_TIME)
			{
				digitalWrite(TRIGGER_PIN, LOW);
				ftState = 0;
				return;
			}
			break;

		default:
			ftState = 0;
			break;
		}
	}
}

void time_printer(LiquidCrystal* lcd, uint32_t *displayTime)
{
  if (displayTime[1] > 0) //display time 1d44h
  {
    lcd->print(displayTime[1]);
    lcd->print("d");
    lcd->print(displayTime[2]/10);
    lcd->print(displayTime[2]%10);
    lcd->print("h");
  }
  else if (displayTime[2] > 0) //display time 12h44m
  {
    lcd->print(displayTime[2]);
    lcd->print("h");
    lcd->print(displayTime[3]/10);
    lcd->print(displayTime[3]%10);
    lcd->print("m");
  }
  else
  {
    lcd->print(displayTime[3]);
    lcd->print("m");
    lcd->print(displayTime[4]/10);
    lcd->print(displayTime[4]%10);
    lcd->print("s");
  }
}


/*
int find_first_entry(uint32_t *time_array)
{
	int first_entry = 0;
	if (!time_array[1]) //display time 12h44m
	{
		first_entry++;
	}
	if (!time_array[2]) //display time 12h44m
	{
		first_entry++;
	}
	return(first_entry);
}


//---------> TimePrinter <------------------------
void time_printer(LiquidCrystal* lcd, uint32_t displayTime[])
{
	char time_letters[3] = {0, 'd', 'h', 'm', 's'};
	//0 added to match entrys between displayTime[] and time_letter[]

	int8_t first_array_entry = find_first_entry(displayTime);

	lcd->print(first_array_entry);
	lcd->print(time_letters[first_array_entry++]);
	lcd->print(first_array_entry / 10);
	lcd->print(first_array_entry % 10);
	lcd->print(time_letters[first_array_entry++]);
}*/

