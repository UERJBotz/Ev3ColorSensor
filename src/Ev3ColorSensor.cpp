#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Ev3ColorSensor.h"

void Ev3ColorSensor::begin(){
  sensorSerial.begin(2400);
  while (true) {
    int value = sensorSerial.read();
    if (value != -1) {
      serialValue[1] = serialValue[0];
      serialValue[0] = value;
    }

    if (serialValue[0] == 128 && serialValue[1] == 144) {
      while (sensorSerial.read() != 4);
      sensorSerial.write(ACK);
      delay(60);
      sensorSerial.end();
      sensorSerial.begin(57600);
      break;
    }
  }
  sendMode();
}

int8_t Ev3ColorSensor::read(){
  sendMode();
  int8_t resp;
  while (sensorSerial.available()){
    serialValue[2] = sensorSerial.read();

    if (serialValue[2] != -1 && serialValue[3] == 0) {
      if((mode == COLOR) && serialValue[2] <= 7){ // color
        serialValue[3] = 1;
        resp = serialValue[2];
      }
      else if((mode == RED_LIGHT || mode == BLUE_LIGHT) && serialValue[2] <= 100){ // light red or blue
        serialValue[3] = 1;
        resp = serialValue[2];
      }
      else if(mode == OFF){ // off
        serialValue[3] = 1;
        resp = 0;
      }
    }
    else {
      serialValue[3] = 0;
    }
  }
  if (mode == COLOR && (resp < 0 || resp > 7))
    resp = 0;
  return resp;
}

void Ev3ColorSensor::setMode(uint8_t newMode){
  if (2 >= newMode && newMode >= 0 || newMode == 5)
    mode = newMode;
}

void Ev3ColorSensor::sendMode() {

  sensorSerial.write(0x44);
  sensorSerial.write(0x11);
  sensorSerial.write(0xff ^ 0x44 ^ 0x11);

  for (int n = 0; n < 3; n++) {
    sensorSerial.write(0x43);
    sensorSerial.write(mode & 0x7);
    sensorSerial.write(0xff ^ 0x43 ^ (mode & 0x7));

    sensorSerial.write(NACK);
  }
}