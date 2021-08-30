
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
float val; // digital input, high when door is open
int lastHour = 0;

void writeDate(void){
  DateTime now = rtc.now();
  char dateBuffer[12];

  sprintf(dateBuffer,"%02u-%02u-%04u ",now.day(),now.month(),now.year());
  myFile.print(dateBuffer);

  sprintf(dateBuffer,"%02u:%02u:%02u ",now.hour(),now.minute(),now.second());
  myFile.print(dateBuffer);
  myFile.print(" (");
  myFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
  myFile.print(") ");
  myFile.print(",");
}

void writeDateOld(void){
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
  char dateBuffer[12];

  sprintf(dateBuffer,"%02u-%02u-%04u ",now.day(),now.month(),now.year());
  Serial.print(dateBuffer);

  sprintf(dateBuffer,"%02u:%02u:%02u ",now.hour(),now.minute(),now.second());
  Serial.print(dateBuffer);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(",");

}

void printDateOld(void){
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
  myFile = SD.open("test2.txt", FILE_WRITE);
  if (myFile) {
    writeDate();
    myFile.println("Device was started");
    myFile.close();
    Serial.println("write Start done.");
  }
  else{
    Serial.println("writeStart was not possible!");
  }
  myFile.close();

}


void writeData(void){
  myFile = SD.open("test2.txt", FILE_WRITE);
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



// checks if door is/was open/closed. if state changed, it uses function to write data 
void handleDoorState(void){
  if(val){ //then door is open
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
  //digitalWrite(LED_BUILTIN, val); 
}

void writeHourlyState(void){
  myFile = SD.open("test2.txt", FILE_WRITE);
  if (myFile) {
    writeDate();
    myFile.print(",,Device is still active, current state is ");
    if(doorClosed){
      myFile.println("closed");
    }
    else{
      myFile.println("open");
    }
    myFile.close();
    Serial.println("Device still active after 1 hour.");
  }
  else{
    Serial.println("writeState was not possible!");
  }
  myFile.close();
}

void checkHourlyState(void){
  int currentHour = rtc.now().hour();
  if(currentHour!= lastHour){
    writeHourlyState();
    lastHour = currentHour;
  }

}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT); this is also sck?

  pinMode(SwitchDigital, INPUT_PULLUP);
  //while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(9600);
  delay(1000);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //use this if you want to sync time with pc
  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(SDchipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  else{
    Serial.println("SD is connected");
    //boolSDConnected = true;
  }
  writeStart();
  printDate();
  Serial.println("initialization done.");

}

// the loop function runs over and over again forever
void loop() {
  checkHourlyState();
  val = digitalRead(SwitchDigital);
  handleDoorState();
  delay(500); 
}
