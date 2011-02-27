#include "WProgram.h" //Include arduino headers
#include "LiquidCrystal.h"
#include "macros.h"
#include "functions.h"
//#include <cstdint>

LiquidCrystal lcd(7, 5, 4, 3, 2, 1);


unsigned long intervalTime[] =
{ 4, 0, 0, 0, 0 };
unsigned long delayTime[] =
{ 3, 0, 0, 0, 0 };
unsigned long delayTimeTemp[] =
{ 0, 0, 0, 0, 0 };
unsigned long duration[] =
{ 0, 0, 0, 0, 0 };

unsigned long remainingTime[] =
{ 0, 0, 0, 0, 0 }; //time till the complete shooting is done
unsigned long nextShootTime[] =
{ 0, 0, 0, 0, 0 }; //time till the next pictures is taken
unsigned long shootingReadyTime[] =
{ 0, 0, 0, 0, 0 };

uint32_t amount = START_AMOUNT; //number of pictures that should be taken
uint32_t currentAmount = 0; //number of pictures allready taken

int shootState = 0; //kind of a sub-state, used while taking pictures
int focusStateON = 0; //saves the different focus states

int state = 0; //main state, used to control nearly everything

uint32_t changeTimeShoot = 0; //used to check time between to shots
uint32_t changeTimeState = 0; //used to check time between state changes
uint32_t changeTimeInterval = 0; //used to check time between intervalState changes
uint32_t changeTimeShootState = 0; //used to check time between shootState changes
uint32_t expiredTime = 0;

int interval_ = 1; //if the interval option is enabled, intervalState equals 1
boolean focusStateOFF = false; //if intervalState=OFF this is used to control focus
boolean delayTimeSaved = false;

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
	printLcd(&state);

	while (true)
	{
	if (shootState==0)
	  {
		while ((digitalRead(UP)==HIGH)||(digitalRead(DOWN)==HIGH))
		{
			if ((millis() - changeTimeInterval) > STATE_CHANGE_DELAY)
				{
					state_changer(&state, &interval_);
					printLcd(&state);
					changeTimeInterval = millis();
				}
		}
	  }

	  //-----> ShootingStates <-----------
	  switch (state)
	  {
	  case 0:
		if (digitalRead(LEFT)==HIGH)  //abort shooting
		{
			shootState=0;
			printLcd(&state);
		}

		//-----> begin of shootState switch/case <-----------
		switch (shootState)
		{
		case 0:

		  if (digitalRead(RIGHT)==HIGH) //start shooting
		  {
			shootState=1;
			printLcd(&state);
			delayTimeSaved = false;
		  }
		  break;


		case 1: //delay before the first picture is taken
		  if (delayTimeSaved==false)
		  {
			/*due to the fact, that I don't want to set the delayTime[0] every times
			 I start the shooting, I save the value temporary in delayTimeTemp[]
			 and left the value in delayTime[0] untouched*/
			delayTimeTemp[0] = (delayTime[0] + 1);
			delayTimeSaved = true;
		  }

		  if ((millis() - changeTimeShootState) > 1000)
		  {
			delayTimeTemp[0]--;
			printLcd(&state);
			changeTimeShootState = millis();
		  }

		  if (delayTimeTemp[0] == 0)
		  {
			shootState++;
			printLcd(&state);
			changeTimeShootState = millis();
		  }
		  break; //end switch(shootState) case 1

		case 2: //taking pictures
		  if ((millis() - changeTimeShootState) > 1000)
		  {
			changeTimeShootState = millis();
			nextShootTime[0] = (intervalTime[0] - (expiredTime + 1));
			remainingTime[0] = ((intervalTime[0] * (amount - currentAmount - 1)) + nextShootTime[0]);
			printLcd(&state);
			if(nextShootTime[0] == 1)
			{
			  expiredTime = 0;
			  nextShootTime[0] = 0;
			  remainingTime[0] = ((intervalTime[0] * (amount - currentAmount - 1)) + nextShootTime[0]);
			}
			else
			  expiredTime++;
		  }

		  if (currentAmount < amount)
		  {
			if ((millis() - changeTimeShoot) > (intervalTime[0]*1000))
			{
			  changeTimeShoot = millis();
			  printLcd(&state);
			  /*here the magic happens, focus will be trigger and the picture
			  will be taken. Focus need some time, so ~ let him 1 sec time...
			  Trigger will be faster, depending on your cam...*/
			  shooting (); //start shooting, 500ms to focus, 500ms to trigger
			  currentAmount++;
			}
		  }
		  else
		  {
			currentAmount = 0;
			shootState++;
		  }
		  break; //end switch(shootState) case 2

		case 3: //display that shooting is finished
		  if ((millis() - changeTimeShootState) > 1000)
		  {
			shootingReadyTime[0]++;
			printLcd(&state);
			changeTimeShootState = millis();
		  }

		  if (digitalRead(RIGHT)==HIGH)
		  {
			shootState=0;
			printLcd(&state);
		  }
		  break; //end switch(shootState) case 3
		}
		//-----> end shootState-switch/case <-----------

		break;

	  case 1:
		while ((digitalRead(RIGHT)==HIGH)||(digitalRead(LEFT)==HIGH))
		{
		  if ((millis() - changeTimeInterval) > (STATE_CHANGE_DELAY))
		  {
			interval_ = !interval_;
			printLcd(&state);
			changeTimeInterval = millis();
		  }
		}
		break;

	  case 2:  // set amount
		while ((digitalRead(RIGHT)==HIGH)||(digitalRead(LEFT)==HIGH))
		{
		  duration[0] = intervalTime[0]*amount;
		  valueChanger(&amount);
		}
		break; //end switch(state) case 2

	  case 3:  //set interval
		while ((digitalRead(RIGHT)==HIGH)||(digitalRead(LEFT)==HIGH))
		{
		  duration[0] = intervalTime[0]*amount;
		  valueChanger(intervalTime);
		}
		break; //end switch(state) case 3

	  case 4: //set focus
		while ((digitalRead(RIGHT)==HIGH)||(digitalRead(LEFT)==HIGH))
		{
		  if ((millis() - changeTimeState) > (STATE_CHANGE_DELAY))
		  {
			if (interval_)
			{
			  if (digitalRead(RIGHT)==HIGH)
			  {
				if (focusStateON < 2)
				  focusStateON++;
				else
				  focusStateON = 0;
			  }
			  if (digitalRead(LEFT)==HIGH)
			  {
				if (focusStateON > 0)
				  focusStateON--;
				else
				  focusStateON = 2;
			  }
			}
			else
			{
			  focusStateOFF = !focusStateOFF;
			}
			printLcd(&state);
			changeTimeState = millis();
		  }
		}
		break; //end switch(state) case 4

	  case 5:
		while ((digitalRead(RIGHT)==HIGH)||(digitalRead(LEFT)==HIGH))
		  valueChanger(delayTime);
		break; //end switch(state) case 5

	  }
	}

	return 0;
}

//---------> printLcd <------------------------
//Used to print everything which is displayed on the lcd
void printLcd(int *state)
{
	lcd.clear();

	if (shootState > 0)
	{
		switch (shootState)
		{
		case 1: //display delay before first picture is taken
			modulator(delayTimeTemp);
			time_printer(&lcd, delayTimeTemp);
			break;

		case 2: //display data while shooting
			//LCD row 1
			lcd.print("a:");
			lcd.print(currentAmount);
			lcd.print("/");
			lcd.print(amount);
			lcd.print(" n:");
			modulator(nextShootTime);
			time_printer(&lcd, nextShootTime);

			//LCD row 2
			lcd.setCursor(0, 1);
			lcd.print("r:");
			modulator(remainingTime);
			time_printer(&lcd, remainingTime);
			break;

		case 3: //display time since shooting is ready
			lcd.print("last picture");

			//LCD row 2
			lcd.setCursor(0, 1);
			lcd.print("taken ");
			modulator(shootingReadyTime);
			time_printer(&lcd, shootingReadyTime);
			lcd.print(" ago");
			break;

		default:
			shootState = 0;
			break;
		}
	}
	else
	{
		switch (*state)
		{
		case OVERVIEW:

			lcd.print("I:");
			if (interval_)
			{
				modulator(intervalTime);
				time_printer(&lcd, intervalTime);
			}
			else
				lcd.print("no");

			lcd.setCursor(12, 0);
			lcd.print("F:");
			PrintFocus(state);

			lcd.setCursor(0, 1);

			lcd.print("A:");
			lcd.print(amount);

			lcd.setCursor(10, 1);
			lcd.print("D:");
			modulator(delayTime);
			time_printer(&lcd, delayTime);
			break;

		case CHOOSE_INTERVAL:
			lcd.print("interval?");

			lcd.setCursor(0, 1);
			if (interval_)
				lcd.print("yes");
			else
				lcd.print("no");
			break;

		case AMOUNT:
			lcd.print("amount: ");
			lcd.print(amount);

			lcd.setCursor(0, 1);
			lcd.print("duration: ");
			duration[0] = intervalTime[0] * amount;
			modulator(duration);
			time_printer(&lcd, duration);
			break;

		case INTERVAL_SETTING:
			lcd.print("interval: ");
			modulator(intervalTime);
			time_printer(&lcd, intervalTime);

			lcd.setCursor(0, 1);
			lcd.print("duration: ");
			duration[0] = intervalTime[0] * amount;
			modulator(duration);
			time_printer(&lcd, duration);
			break;

		case FOCUS_SETTING:
			lcd.print("set focus");

			lcd.setCursor(0, 1);
			PrintFocus(state);
			break;

		case DELAY_SETTING:
			lcd.print("set delay");

			lcd.setCursor(0, 1);
			modulator(delayTime);
			time_printer(&lcd, delayTime);
			break;

		default:
			*state = 0;
			break;
		}
	}
}

void PrintFocus(int *state)
{
	if (interval_)
	{
		switch (focusStateON)
		{
		case 0:
			if (*state == 0)
				lcd.print("n");
			else if (*state == 4)
				lcd.print("no");
			break;

		case 1:
			if (*state == 0)
				lcd.print("yo");
			else if (*state == 4)
				lcd.print("yes, once");
			break;

		case 2:
			if (*state == 0)
				lcd.print("ya");
			else if (*state == 4)
				lcd.print("yes, always");
			break;

		default:
			focusStateON = 0;
			break;
		}
	}
	else
	{
		if (focusStateOFF)
		{
			if (*state == 0)
				lcd.print("y");
			else if (*state == 4)
				lcd.print("yes");
		}
		else
		{
			if (*state == 0)
				lcd.print("n");
			else if (*state == 4)
				lcd.print("no");
		}
	}
}






