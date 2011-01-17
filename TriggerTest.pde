// Project 10 - Serial controlled RGB Lamp
char buffer[18];
int focusTime, triggerTime, betweenTime;
int triggerPin = 5;
int focusPin = 3;

void setup() 
{ 
  Serial.begin(9600);
  Serial.flush();
  pinMode(triggerPin, OUTPUT);
  pinMode(focusPin, OUTPUT);
} 
void loop() 
{ 
  if (Serial.available() > 0) {
    int index=0;
    delay(100); // let the buffer fill up
    int numChar = Serial.available();
    if (numChar>15) {
      numChar=15;
    }
    while (numChar--) {
      buffer[index++] = Serial.read();
    }
    splitString(buffer);

  }
}
void splitString(char* data) {
  Serial.print("Data entered: ");
  Serial.println(data);
  char* parameter; 
  parameter = strtok (data, " ,");
  while (parameter != NULL) {
    setLED(parameter);
    parameter = strtok (NULL, " ,");
  }
  // Clear the text and serial buffers
  for (int x=0; x<16; x++) {
    buffer[x]='\0';
  }
  Serial.flush();
}

void setLED(char* data) 
  {  
  int timeArray[3] = {0,0,0};

  if ((data[0] == 'f') || (data[0] == 'F')) 
    {
    int Ans = strtol(data+1, NULL, 10);
    timeArray[0] = Ans;
    Serial.print("focus is set to: ");
    Serial.println(timeArray[0]);
    }
     
  if ((data[0] == 'b') || (data[0] == 'B')) 
    {
    int Ans = strtol(data+1, NULL, 10);
    timeArray[1] = Ans;
    Serial.print("between is set to: ");
    Serial.println(timeArray[1]);
    }

  if ((data[0] == 't') || (data[0] == 'T')) 
    {
    int Ans = strtol(data+1, NULL, 10);
    timeArray[2] = Ans;
    Serial.print("trigger is set to: ");
    Serial.println(timeArray[2]);
    }
    
  digitalWrite(focusPin, HIGH);
  delay(timeArray[0]);
  digitalWrite(focusPin, LOW);
  delay(timeArray[1]);
  digitalWrite(triggerPin, HIGH);
  delay(timeArray[2]);
  digitalWrite(triggerPin, LOW);
  }

