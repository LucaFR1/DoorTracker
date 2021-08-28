
/
#include <Arduino.h>

//#include "ArduinoLowPower.h"

#define SwitchDigital 7
bool doorClosed;
float val; // digital input, low when door is closed


void writeData(void){
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
  Serial.begin(9600);
  pinMode(SwitchDigital, INPUT_PULLUP);

}

// the loop function runs over and over again forever
void loop() {

  val = digitalRead(SwitchDigital);
  handleDoorState();
  delay(200); 
}
