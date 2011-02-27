/* IVAT 1.21

Johannes Rebling,  03/2010 - 12/2010
supported by C3D2 in Dresden, Germany

Visit wiki.c3d2.de/IVAT to see the full documentation!

This program and the connected arduino-clone
act as (interval) trigger for digital photocameras (like my
Canon EOS 400D, for a full list of supported models visit the
documentation page). 
You are able to select the time between two taken 
pictures and the number of pictures taken at all.
These will be done by four buttons which will increase
or decrease either the time or the quantity.
The output display is realized with a 16x2 LCD-Display (16 signs 
per row, 2 rows).*/

#include <LiquidCrystal.h>
//used to control the LCD easily, for more information visit ???

LiquidCrystal lcd(2, 4, 5, 6, 7, 8);
//LiquidCrystal(rs, enable, d4, d5, d6, d7) 
/*rs: pin that is connected to the RS pin on the LCD
  enable: pin that is connected to the enable pin on the LCD
  d4, d5, d6, d7: the numbers of the Arduino 
  pins that are connected to the corresponding data pins on the LCD*/

//--------->>>definiton of the I/O-pins<<<--------------------
const int UpKey = 11; //button to next state
const int DownKey = 1; //button to previous state
const int LeftKey = 13; //button to decrease quantity/time
const int RightKey = 12; //button to increase quantity/time

/*my camera has a mini-klinke connector, so there are 3 wires I have to take into
account. One wire is ground, one takes care of the fokus and one is the trigger. So if
you connect either the focus or the trigger wire to ground, the cam will eiter focus
it will take a picture. Furthermore, the focus wire acts a as alarm to wake up the cam 
from standby. Otherwise it wont take a picture when the trigger ist connected to ground.
I control the cam with two transistors which are controled by the microcontroler and which
will connect or split focus and trigger to ground.*/

const int triggerPin = 10; //controls the transitor connected to the cam
const int focusPin = 9; //controls the focus of the cam 

//--------->>>time arrays<<<--------------------
/*All times are save in arrays. The first entry, array[0], allways stores the complete time 
value in seconds, so if the stored time is 2:34min, then array[0]==154.
The rest of the arrays are filled as follows:
array[]={complete time value, days, hours, minutes, seconds}*/
unsigned long intervalTime[] = {4,0,0,0,0}; 
unsigned long delayTime[] = {3,0,0,0,0};
unsigned long delayTimeTemp[] = {0,0,0,0,0}; 
unsigned long duration[] = {0,0,0,0,0}; 
unsigned long remainingTime[] = {0,0,0,0,0}; //time till the complete shooting is done
unsigned long nextShootTime[] = {0,0,0,0,0}; //time till the next pictures is taken
unsigned long shootingReadyTime[] = {0,0,0,0,0};

//--------->>>other variables<<<--------------------
unsigned long amount = 100;  //number of pictures that should be taken
unsigned long currentAmount = 0; //number of pictures allready taken

int state = 0; //main state, used to control nearly everything
int shootState = 0; //kind of a sub-state, used while taking pictures
int focusStateON = 0; //saves the different focus states
int stateChangeDelay = 150; //delay between state changes  

unsigned long changeTimeShoot = 0; //used to check time between to shots
unsigned long changeTimeState = 0; //used to check time between state changes
unsigned long changeTimeInterval = 0; //used to check time between intervalState changes
unsigned long changeTimeShootState = 0; //used to check time between shootState changes
unsigned long expiredTime = 0;

boolean intervalState = true; //if the interval option is enabled, intervalState equals 1 
boolean focusStateOFF = false; //if intervalState=OFF this is used to control focus
boolean delayTimeSaved = false;

void setup()
{  
  lcd.begin(16, 2);
  //Setting if the pins either input or output pins 
  pinMode(triggerPin,OUTPUT);
  pinMode(focusPin,OUTPUT);
  pinMode(UpKey,INPUT);
  pinMode(DownKey,INPUT);
  pinMode(LeftKey,INPUT);
  pinMode(RightKey,INPUT);
  PrintLCD();
}

void loop() 
{
  if (shootState==0)
  {
    while ((digitalRead(UpKey)==HIGH)||(digitalRead(DownKey)==HIGH))
    {
      StateChanger();
    }
  }

  //-----> ShootingStates <-----------
  switch (state) 
  {
  case 0:
    if (digitalRead(LeftKey)==HIGH)  //abort shooting
    {
        shootState=0; 
        PrintLCD();
    }
        
    //-----> begin of shootState switch/case <-----------
    switch (shootState)
    {
    case 0:
    
      if (digitalRead(RightKey)==HIGH) //start shooting
      {
        shootState=1; 
        PrintLCD();
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
        PrintLCD();
        changeTimeShootState = millis();
      }

      if (delayTimeTemp[0] == 0)
      {
        shootState++;
        PrintLCD();
        changeTimeShootState = millis();
      } 
      break; //end switch(shootState) case 1

    case 2: //taking pictures
      if ((millis() - changeTimeShootState) > 1000) 
      { 
        changeTimeShootState = millis();        
        nextShootTime[0] = (intervalTime[0] - (expiredTime + 1));
        remainingTime[0] = ((intervalTime[0] * (amount - currentAmount - 1)) + nextShootTime[0]);
        PrintLCD();
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
          PrintLCD();
          /*here the magic happens, focus will be trigger and the picture
          will be taken. Focus need some time, so ~ let him 1 sec time...
          Trigger will be faster, depending on your cam...*/
          shooting (500, 500); //start shooting, 500ms to focus, 500ms to trigger
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
        PrintLCD();
        changeTimeShootState = millis();
      }
      
      if (digitalRead(RightKey)==HIGH)
      {
        shootState=0; 
        PrintLCD;
      }
      break; //end switch(shootState) case 3
    }    
    //-----> end shootState-switch/case <-----------

    break; //end switch(state) case 0

  case 1: 
    while ((digitalRead(RightKey)==HIGH)||(digitalRead(LeftKey)==HIGH)) 
    {
      if ((millis() - changeTimeInterval) > (stateChangeDelay)) 
      {
        intervalState = !intervalState;
        PrintLCD();
        changeTimeInterval = millis();
      }
    }
    break; //end switch(state) case 1
    
  case 2:  // set amount
    while ((digitalRead(RightKey)==HIGH)||(digitalRead(LeftKey)==HIGH)) 
    { 
      duration[0] = intervalTime[0]*amount;
      changer(&amount);
    } 
    break; //end switch(state) case 2
    
  case 3:  //set interval  
    while ((digitalRead(RightKey)==HIGH)||(digitalRead(LeftKey)==HIGH)) 
    {
      duration[0] = intervalTime[0]*amount;
      changer(&intervalTime[0]);
    }
    break; //end switch(state) case 3

  case 4: //set focus
    while ((digitalRead(RightKey)==HIGH)||(digitalRead(LeftKey)==HIGH))
    {
      if ((millis() - changeTimeState) > (stateChangeDelay)) 
      {
        if (intervalState==true) 
        { 
          if (digitalRead(RightKey)==HIGH)
            if (focusStateON < 2)
              focusStateON++;
            else
              focusStateON = 0;
          if (digitalRead(LeftKey)==HIGH)
            if (focusStateON > 0)
              focusStateON--;
            else
              focusStateON = 2;   
        }
        else 
        {
          focusStateOFF = !focusStateOFF;  
        }
        PrintLCD();
        changeTimeState = millis();
      }
    }
    break; //end switch(state) case 4

  case 5:
    while ((digitalRead(RightKey)==HIGH)||(digitalRead(LeftKey)==HIGH)) 
      changer(&delayTime[0]);
    break; //end switch(state) case 5

  }//-----> end state-switch/case <-----------
}//close loop();

//---------> Changer <------------------------
int changer (unsigned long *identifier) //identifier has to be a unsigned value
{
  unsigned long changeTimeIdent = millis(); //used to check time between settings
  unsigned long changeTimeDoDo = millis(); //tribute to my friend Dodo who helped me a lot
  unsigned long changeTimePrint = millis();
  int counter = 1;

  while (digitalRead(RightKey)==HIGH || digitalRead(LeftKey)==HIGH) 
  {
    if ((millis() - changeTimeIdent) > (100)) 
    {
      if (digitalRead(RightKey)==HIGH)
      {
        if ((*identifier + counter) < 99999999)
          *identifier += counter; 
        else
          *identifier = 99999999;
      }
      if (digitalRead(LeftKey)==HIGH)
      {
        if ((*identifier - counter) < 99999999)
          *identifier -= counter;
        else
          *identifier = 0;
      }

      if ((millis() - changeTimeDoDo) > (400)) 
      {
        counter++;
        changeTimeDoDo = millis();   
      }

      PrintLCD();
      changeTimeIdent = millis();
    }   
  }
}

//---------> PrintLCD <------------------------
//Used to print everything which is displayed on the lcd
void PrintLCD() 
{
  lcd.clear();

  if(shootState > 0)
  {
    switch(shootState)
    {
    case 1: //display delay before first picture is taken
      Modulator(delayTimeTemp);
      TimePrinter(delayTimeTemp); 
      break;

    case 2: //display data while shooting
      //LCD row 1
      lcd.print("a:");
      lcd.print(currentAmount);
      lcd.print("/");
      lcd.print(amount);
      
      //LCD row 2
      lcd.setCursor(0,1);
      lcd.print("n:");
      Modulator(nextShootTime);
      TimePrinter(nextShootTime);
      lcd.print(" r:");
      Modulator(remainingTime);
      TimePrinter(remainingTime);
      break;

    case 3: //display time since shooting is ready
      lcd.print("last picture");
      
      //LCD row 2
      lcd.setCursor(0,1);
      lcd.print("taken ");
      Modulator(shootingReadyTime);
      TimePrinter(shootingReadyTime);
      lcd.print(" ago");
      break;

    default:
      shootState = 0;
      break;
    }
  }
  else
  {
    switch(state)
    {
    case 0: //overview
      //prints the menu for the actual state

      //LCD row 1
      //print interval status
      lcd.print("I:");
      if (intervalState) 
      {
        Modulator(intervalTime);
        TimePrinter(intervalTime);
      }
      else 
        lcd.print("no");

      //print focus status
      lcd.setCursor(12,0);
      lcd.print("F:");
      PrintFocus();

      //LCD row 2
      //print amount
      lcd.setCursor(0,1); 
      lcd.print("A:");
      lcd.print(amount);

      //print delay
      lcd.setCursor(10,1);
      lcd.print("D:");
      Modulator(delayTime);
      TimePrinter(delayTime);
      break;

    case 1: //interval yes/no
      //first line
      lcd.print("interval?");

      //second line    
      lcd.setCursor(0,1);
      if (intervalState)
        lcd.print("yes");
      else
        lcd.print("no");
      break;

    case 2: //amount setting & show duration 
      //first line
      lcd.print("amount: ");
      lcd.print(amount);

      //second line
      lcd.setCursor(0,1);
      lcd.print("duration: ");
      duration[0] = intervalTime[0]*amount;
      Modulator(duration);
      TimePrinter(duration);
      break;

    case 3: //interval setting
      //first line
      lcd.print("interval: ");
      Modulator(intervalTime);
      TimePrinter(intervalTime);

      //second line
      lcd.setCursor(0,1); 
      lcd.print("duration: ");
      duration[0] = intervalTime[0]*amount;
      Modulator(duration);
      TimePrinter(duration);
      break;

    case 4: //focus
      //first line
      lcd.print("set focus");

      //second line
      lcd.setCursor(0,1);
      PrintFocus();
      break;

    case 5: //delay
      //first line
      lcd.print("set delay");

      //second line
      lcd.setCursor(0,1);
      Modulator(delayTime);
      TimePrinter(delayTime);
      break;

    default:
      state = 0;
      break;
    }
  }  
}

void PrintFocus()
{
  if (intervalState)
  {  
    switch(focusStateON)
    {
    case 0:
      if (state == 0)
        lcd.print("n");
      else if (state == 4)
        lcd.print("no");
      break;

    case 1:
      if (state == 0)
        lcd.print("yo");
      else if (state == 4)
        lcd.print("yes, once");
      break;

    case 2:
      if (state == 0)
        lcd.print("ya");
      else if (state == 4)
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
      if (state == 0)
        lcd.print("y");
      else if (state == 4)
        lcd.print("yes");
    }
    else
    {
      if (state == 0)
        lcd.print("n");
      else if (state == 4)
        lcd.print("no");
    } 
  }
}

//---------> StateChanger <------------------------
void StateChanger()
{
  if ((millis() - changeTimeInterval) > (stateChangeDelay)) 
  {
    if (digitalRead(DownKey)==HIGH)
    {
      if ((state==1) && (intervalState==false))
        state = 4;
      else if (state < 5)
        state++;
      else
        state = 0;
      PrintLCD();
    }
    if (digitalRead(UpKey)==HIGH)
    {
      if ((state == 4) && (intervalState == false))
        state = 1;
      else if (state > 0)
        state--;
      else
        state = 5;
      PrintLCD();
    }
    changeTimeInterval = millis();
  }
}

//---------> Modulator <------------------------
/*calculates times that are given in seconds and puts 
them into an array in the following maner: 
array[]=={seconds, seconds/10, minutes/10, minutes, hours}*/
void Modulator(unsigned long time[])
{
  unsigned long seconds[] = {
    86400, 3600, 60};

  for (int i=1; i<=4; i++)
  {
    time[i] = time[i-1]%seconds[i-1];
  }

  for (int n=3; n>0; n--)
  {
    time[n] = time[n-1]/seconds[n-1];
  }
}

//---------> TimePrinter <------------------------
void TimePrinter(unsigned long displayTime[])
{
  if (displayTime[1] > 0) //display time 1d44h
  { 
    lcd.print(displayTime[1]);
    lcd.print("d");
    lcd.print(displayTime[2]/10);
    lcd.print(displayTime[2]%10);
    lcd.print("h");
  }
  else if (displayTime[2] > 0) //display time 12h44m
  {
    lcd.print(displayTime[2]);
    lcd.print("h");
    lcd.print(displayTime[3]/10);
    lcd.print(displayTime[3]%10);
    lcd.print("m");
  }
  else
  {
    lcd.print(displayTime[3]);
    lcd.print("m");
    lcd.print(displayTime[4]/10);
    lcd.print(displayTime[4]%10);
    lcd.print("s");
  }
}

//---------> shooting <------------------------
int shooting (int focusTime, int triggerTime)
{
  int ftState = 0;
  boolean shootState = true;
  unsigned long focusTimeCheck, triggerTimeCheck;
  
  while(shootState == true)
  {
    switch(ftState)
    {
    case 0:
      digitalWrite(focusPin, HIGH);
      focusTimeCheck = millis(); 
      ftState++;
      break;

    case 1:
      if((millis() - focusTimeCheck) > focusTime)
      {
        digitalWrite(focusPin, LOW);
        ftState++;
      }
      break;

    case 2: 
      digitalWrite(triggerPin, HIGH);
      triggerTimeCheck = millis();
      ftState++;
      break;

    case 3:
      if((millis() - triggerTimeCheck) > triggerTime)
      {
        digitalWrite(triggerPin, LOW);
        ftState = 0;
        shootState = false;
      }
      break;

    default:
      ftState = 0;
      break;
    }
  }
}
