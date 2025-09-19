/*
  Author: Tony Albert Lima
  Version: 1.0
  Library made for use Ev3 color sensor with arduino

  This library is distributed under MIT license in the hope
  that it will be useful, but WITHOUT ANY WARRANTY;
*/

#ifndef _EV3COLORSENSOR_H_
#define _EV3COLORSENSOR_H_

#include <Arduino.h>
#include <SoftwareSerial.h>


#define ACK  0x04
#define NACK 0x02

// https://github.com/JakubVanek/lms2012-stuff/ -> ev3color/program.h
enum Ev3ColorSensorMode : uint8_t {
    RED_LIGHT  = 0, // COL-REFLECT
    BLUE_LIGHT = 1, // COL-AMBIENT
    COLOR      = 2, // COL-COLOR
    RAW        = 3, // REF-RAW
    RGB        = 4, // RGB-RAW
    OFF        = 5, // COL-CAL
};

enum Ev3Color : int8_t {
    None   = 0,
    Black  = 1,
    Blue   = 2,
    Green  = 3,
    Yellow = 4,
    Red    = 5,
    White  = 6,
    Brown  = 7,
    EV3COLORCOUNT,
};

const String Ev3ColorString(const Ev3Color color);

class Ev3ColorSensor {
  private:
    SoftwareSerial sensorSerial;
    uint8_t waitTime;

    int serialValue[2] = {0};
    bool prevSerialFailed = false;
    bool done = false;

    int readByte();
    bool Ev3ColorSensor::parse_color(int byte, Ev3Color* out);

    Ev3ColorSensorMode mode = Ev3ColorSensorMode::COLOR;
    void sendMode();

  public:
    Ev3ColorSensor(uint8_t rx, uint8_t tx, Ev3ColorSensorMode newMode = COLOR, uint8_t wt = 5):
      sensorSerial(rx, tx), mode(newMode), waitTime(wt){};

    // Starts sensor communication
    void begin();

    // Reads and returns the current color
    Ev3Color read();

    // Changes the sensor's mode of operation based on a constant
    void setMode(Ev3ColorSensorMode newMode);

    // Changes which sensor will be read
    void activate();
};

#endif // _EV3COLORSENSOR_H_
