#include <assert.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ev3ColorSensor.h>

static const String Ev3ColorNameTable[] = {
    [None]   = "None",
    [Black]  = "Black",
    [Blue]   = "Blue",
    [Green]  = "Green",
    [Yellow] = "Yellow",
    [Red]    = "Red",
    [White]  = "White",
    [Brown]  = "Brown",
};
const String Ev3ColorString(const Ev3Color color) {
    return Ev3ColorNameTable[color];
}

#define LEN(arr) (sizeof(arr)/sizeof(*arr))
static_assert(LEN(Ev3ColorNameTable) == Ev3Color::EV3COLORCOUNT);
#undef LEN


void Ev3ColorSensor::begin() {
  sensorSerial.begin(2400);

  for(;;) {
    int value = sensorSerial.read();
    if (value != -1) {
      serialValue[1] = serialValue[0];
      serialValue[0] = value;
    }

    if (serialValue[0] == 128 &&
        serialValue[1] == 144) break;
  }
  while (sensorSerial.read() != 4);

  sensorSerial.write(ACK); delay(60);
  sensorSerial.end();
  sensorSerial.begin(57600);
  sendMode();
}

// https://github.com/JakubVanek/lms2012-stuff/ -> ev3sensors/color/metadata.txt
Ev3Color Ev3ColorSensor::read() {
  //! mudar o array para uma local e um membro
  //! tipo de retorno errado (fazer tagged union ColorSensorResponse)

  sendMode();

  // ensures sensor is ready
  for(unsigned long t0 = millis(); millis() - t0 <= waitTime; );

  Ev3Color resp = None;
  while (sensorSerial.available()){
    serialValue[2] = sensorSerial.read();

    if (serialValue[2] != -1 && serialValue[3] == 0) {
      switch(mode) {
        case COLOR: {
          if (serialValue[2] > EV3COLORCOUNT) break;

          serialValue[3] = 1; resp = (Ev3Color)serialValue[2];
        } break;
        case RED_LIGHT: case BLUE_LIGHT: {
          if (serialValue[2] > 100) break;

          serialValue[3] = 1; resp = (Ev3Color)serialValue[2];
        } break;
        case OFF: {
          serialValue[3] = 1; resp = None;
        } break;
      }
    } else {
      serialValue[3] = 0;
    }
  }
  return resp;
}

void Ev3ColorSensor::setMode(Ev3ColorSensorMode newMode) { 
    if (RED_LIGHT < newMode && newMode > OFF) {
      mode = newMode; 
    } else assert(!"invalid mode");
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

void Ev3ColorSensor::activate(){
  sensorSerial.listen();
}
