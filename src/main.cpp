
#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"

#include <SPI.h>
#include <SD.h>

RTC_DS1307 rtc;

File myFile;
const int SDchipSelect = 4;


char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//#include "ArduinoLowPower.h"

#define SwitchDigital 7
bool doorClosed;
float val; // digital input, low when door is closed
bool boolSDConnected = false;

void writeDate(void){
  DateTime now = rtc.now();

  myFile.print(now.year(), DEC);
  myFile.print('/');
  myFile.print(now.month(), DEC);
  myFile.print('/');
  myFile.print(now.day(), DEC);
  myFile.print(" (");
  myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
  myFile.print(") ");
  myFile.print(now.hour(), DEC);
  myFile.print(':');
  myFile.print(now.minute(), DEC);
  myFile.print(':');
  myFile.print(now.second(), DEC);
  myFile.print(",");
}

void printDate(void){
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
  Serial.print(",");
}


void writeStart(void){
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    writeDate();
    myFile.println("Device was started");
    myFile.close();
    Serial.println("done.");
  }


}


void writeData(void){

  if(!boolSDConnected){
    if (!SD.begin(SDchipSelect)) {
      Serial.println("reinitialization failed!");
      return;
    }
    else{
      Serial.println("SD is connected again");
      boolSDConnected = true;
      return;
    }
  }

  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    if(!boolSDConnected){
      Serial.println("SD is connected now first time!");
      boolSDConnected = true;
    }

    writeDate();
    if(doorClosed){
      myFile.println("door was closed, now open");
      Serial.println("door was closed, now open");
        }
    else{
      myFile.println("door was open, now closed");
      Serial.println("door was open, now closed");
    }
    myFile.close();
    Serial.println("done.");
  }
  else{
    Serial.println("No SD connected");
    boolSDConnected = false;
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

void checkSDconnected(void){
  myFile = SD.open("test.txt");
  if (myFile) {
  Serial.println("No SD is there!");
  return;
}
else{
  Serial.println("SD is connected");
  boolSDConnected = true;
  }
  if (!boolSDConnected){
    Serial.println("No SD Connected");
  }
  myFile.close();
  return;
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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(SDchipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  else{
    Serial.println("SD is connected");
    boolSDConnected = true;
    return;
  }
  Serial.println("initialization done.");
  writeStart();
  printDate();
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("new cycle");
  val = digitalRead(SwitchDigital);
  handleDoorState();
  delay(500); 
}
