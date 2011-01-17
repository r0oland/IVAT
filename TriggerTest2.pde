// Project 10 - Serial controlled RGB Lamp
unsigned long focusTimeCheck, triggerTimeCheck;
int triggerPin = 5;
int focusPin = 3;
boolean fState = false;
boolean tState = false;
boolean shooting = true;

void setup() 
{ 
  pinMode(triggerPin, OUTPUT);
  pinMode(focusPin, OUTPUT);

  /*digitalWrite(focusPin, HIGH);
  delay(500);
  digitalWrite(focusPin, LOW);
  digitalWrite(triggerPin, HIGH);
  delay(500);
  digitalWrite(triggerPin, LOW);
  digitalWrite(focusPin, HIGH);
  digitalWrite(triggerPin, HIGH);
  delay(500);
  digitalWrite(focusPin, LOW);
  digitalWrite(triggerPin, LOW);
  delay(500);
  triggerTimeCheck = millis();*/
} 

void loop() 
{ 
  while (shooting == true)
  {
    if (fState == false)
    {
      digitalWrite(focusPin, HIGH);
      focusTimeCheck = millis(); 
      fState == true; 
    }
      
    if(((millis() - focusTimeCheck) > 500) && (fState == true))
    { 
      digitalWrite(focusPin, LOW);
      tState = true;
    }
    
    if ((tState == true) && (fState == true))
    {
      digitalWrite(triggerPin, HIGH);
      triggerTimeCheck = millis();    
    }
    
    if(((millis() - triggerTimeCheck) > 500) && (tState == true))
    { 
      digitalWrite(triggerPin, LOW);
      fState = false;
      tState = false;
    }
    shooting = false;   
  }
  delay(500);
  shooting = true;   
}


                                                                                                                                                                                           