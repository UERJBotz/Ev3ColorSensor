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

enum Ev3ColorSensorMode : uint8_t {
    RED_LIGHT  = 0,
    BLUE_LIGHT = 1,
    COLOR      = 2,
    OFF        = 5,
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

const String Ev3ColorNameTable[] = {
    [None]   = "None",
    [Black]  = "Black",
    [Blue]   = "Blue",
    [Green]  = "Green",
    [Yellow] = "Yellow",
    [Red]    = "Red",
    [White]  = "White",
    [Brown]  = "Brown",
};

#define LEN(arr) (sizeof(arr)/sizeof(*arr))
static_assert(LEN(Ev3ColorNameTable) == Ev3Color::EV3COLORCOUNT);
#undef LEN


class Ev3ColorSensor {
  private:
    int serialValue[4];
    uint8_t mode, waitTime;
    SoftwareSerial sensorSerial;

    void sendMode();
  public:
    Ev3ColorSensor(uint8_t rx, uint8_t tx, uint8_t wt = 5, uint8_t newMode = COLOR): 
      sensorSerial(rx, tx), mode(newMode), serialValue{0,0,0,0}, waitTime(wt){};

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
