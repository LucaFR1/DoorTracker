#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//#include "ArduinoLowPower.h"

#define SwitchDigital 7
bool doorClosed;
float val; // digital input, low when door is closed


void writeData(void){

  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  if(doorClosed){
    Serial.println("door was closed, now open");


      }
  else{
    Serial.println("door was open, now closed");
  }
}


// checks if door is/was open/closed. if state changed, it uses function to write data 
void handleDoorState(void){
  if(val){ //then door is closed
    if (doorClosed==true)
    {
      writeData();
    }
    doorClosed = false;
  }
  else{
    if (doorClosed==false)
    {
      writeData();
    doorClosed = true;
    }
  }
  digitalWrite(LED_BUILTIN, val); 
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(SwitchDigital, INPUT_PULLUP);
  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

}

// the loop function runs over and over again forever
void loop() {

  val = digitalRead(SwitchDigital);
  handleDoorState();
  delay(200); 
}
