#include <assert.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ev3ColorSensor.h>

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

// https://github.com/JakubVanek/lms2012-stuff/ -> ev3sensors/color/metadata.txt
void Ev3ColorSensor::preamble(Ev3ColorSensorMode m) {
  setMode(m); sendMode();
  DELAY(waitTime); // ensures sensor is ready
}
int8_t Ev3ColorSensor::read_byte() {
  int curr = sensorSerial.read();
  if (curr == -1) prevSerialFailed = true;
  return curr;
}
int8_t Ev3ColorSensor::read_pct() {
  const int8_t invalid = -1;
  int curr = read_byte();
  if (curr == -1) return invalid;
  if (curr > 100) return invalid;

  received = true;
  return curr;
}
int16_t Ev3ColorSensor::read_u16() {
  const int16_t invalid = -1;
  int low  = read_byte(); if (low == -1)  return invalid;
  int high = read_byte(); if (high == -1) return invalid;

  received = true;
  return high/*& 0xFF?*/ << 8 | low;
}
Ev3Color Ev3ColorSensor::read_color() {
  int curr = read_byte();
  if (curr == -1)           return Ev3Color::Invalid;
  if (curr > EV3COLORCOUNT) return Ev3Color::None;

  received = true;
  return (Ev3Color)curr;
}
Ev3RGB Ev3ColorSensor::read_rgb() {
  static const Ev3RGB invalid = {
      UINT16_MAX, UINT16_MAX, UINT16_MAX,
  };

  int16_t r = read_u16(); if (r == -1) return invalid;
  int16_t g = read_u16(); if (g == -1) return invalid;
  int16_t b = read_u16(); if (b == -1) return invalid;

  received = true;
  return { (uint16_t)r, (uint16_t)g, (uint16_t)b };
}
//int8_t Ev3ColorSensor::reflectance() {
//  preamble(RED_LIGHT); return read_pct();
//}
//int8_t Ev3ColorSensor::ambient() {
//  preamble(BLUE_LIGHT); return read_pct();
//}
//Ev3Color Ev3ColorSensor::color() {
//  preamble(COLOR); return read_color();
//}
//Ev3RGB Ev3ColorSensor::rgb(uint8_t curr) {
//  preamble(RGB); return read_rgb();
//}

Ev3ColorSensorResult Ev3ColorSensor::read() {
  preamble(mode);

  Ev3ColorSensorResult resp { .mode = mode };
  while (sensorSerial.available()) {
    switch (mode) {
      case RGB:   resp.rgb   = read_rgb();   break;
      case COLOR: resp.color = read_color(); break;

      case RED_LIGHT: case BLUE_LIGHT: {
          resp.intensity = read_pct();
      } break;

      case RAW: /*unsupported*/
      case OFF: received = true; break;
    }
    if (received || prevSerialFailed) break;
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
