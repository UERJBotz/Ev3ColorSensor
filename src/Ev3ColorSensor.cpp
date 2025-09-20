#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ev3ColorSensor.h>


//! receber modo no begin()
//! conseguir retornar byte em readByte()

#define ASSERT(exp) ( \
        (exp) ? 1 : 0 \
          && Serial.print(__FILE__) && Serial.print(":") \
          && Serial.print(__LINE__)                      \
          && Serial.print(": assertion "#exp" failed\n") \
        )

#define LEN(arr) (sizeof(arr)/sizeof(*arr))
#define DELAY(w) for(uintmax_t t0 = millis(); millis() - t0 <= w;)


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
static_assert(LEN(Ev3ColorNameTable) == Ev3Color::EV3COLORCOUNT);

const String Ev3ColorString(const Ev3Color color) {
    return Ev3ColorNameTable[color];
}


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

int Ev3ColorSensor::readByte() { //! isso precisou ser int (não int8_t), ou isso ou a variável em read()
  int curr = sensorSerial.read();
  prevSerialFailed = (curr == -1);
  return curr;
}

bool Ev3ColorSensor::parseColor(int byte, Ev3Color* out) {
  if (byte == -1)           return false;
  if (byte > EV3COLORCOUNT) return false;

  *out = (Ev3Color)byte; return true;
}
bool Ev3ColorSensor::parseIntensity(int byte, uint8_t* out) {
  if (byte == -1) return false;
  if (byte > 100) return false;

  *out = (uint8_t)byte; return true;
}
//! converter resto das funções da outra branch pra ficar que nem essas

// https://github.com/JakubVanek/lms2012-stuff/ -> ev3sensors/color/metadata.txt
Ev3ColorResult Ev3ColorSensor::read() {
  sendMode();
  DELAY(waitTime); // ensures sensor is ready

  Ev3ColorResult ret = {None};
  while (sensorSerial.available()){
    int currByte = readByte();

    if (prevSerialFailed || done) done = false;
    else {
      switch (mode) {
        case COLOR: {
          done = parseColor(currByte, &ret.color);
        } break;

        case AMBIENT:
        case REFLECT: {
          done = parseIntensity(currByte, &ret.intensity);
        } break;

        case RAW: case RGB: //!
        case CAL: done = true; break;
      }
    }
  }
  return ret;
}

void Ev3ColorSensor::setMode(Ev3ColorSensorMode newMode) { 
  if (newMode == mode) return;

  mode = newMode; 
  if (ASSERT(RED_LIGHT > newMode)) mode = RED_LIGHT;
  if (ASSERT(newMode > LIGHT_OFF)) mode = LIGHT_OFF;
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
