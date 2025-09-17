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
        Ev3Color color;
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

    Invalid = -1,
};

struct Ev3RGB { uint16_t r, g, b; };
struct Ev3ColorSensorResult {
    enum Ev3ColorSensorMode mode;
    union {
        Ev3RGB rgb;
        Ev3Color color;
        uint8_t intensity;
    };
};

const String Ev3ColorString(const Ev3Color color);

class Ev3ColorSensor {
  private:
    void sendMode();
    void preamble(Ev3ColorSensorMode m);
    int8_t read_byte();
    int8_t read_pct();
    int16_t read_u16();

    Ev3RGB   read_rgb();
    Ev3Color read_color();

    SoftwareSerial sensorSerial;
    int serialValue[2] = {0};
    bool prevSerialFailed = false;
    bool received = false;

    Ev3ColorSensorMode mode = Ev3ColorSensorMode::COLOR;
    uint8_t waitTime;
  public:
    Ev3ColorSensor(uint8_t rx, uint8_t tx, Ev3ColorSensorMode newMode = COLOR, uint8_t wt = 5):
      sensorSerial(rx, tx), mode(newMode), waitTime(wt){}

    // Starts sensor communication
    void begin();

    // Reads and returns the current color
    Ev3ColorSensorResult read();

    // Changes the sensor's mode of operation based on a constant
    void setMode(Ev3ColorSensorMode newMode);

    // Changes which sensor will be read
    void activate();
};

#endif // _EV3COLORSENSOR_H_
