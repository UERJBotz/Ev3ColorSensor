/*
  Author: Tony Albert Lima
  Version: 1.0
  Library made for use Ev3 color sensor with arduino

  This library is distributed under MIT lisence in the hope
  that it will be useful, but WITHOUT ANY WARRANTY;
*/

#ifndef EV3COLORSENSOR_H
#define EV3COLORSENSOR_H

#include "Arduino.h"
#include "SoftwareSerial.h"
#define ACK 0x04
#define NACK 0x02

// Modes define
#define RED_LIGHT 0
#define BLUE_LIGHT 1
#define COLOR 2
#define OFF 5

class Ev3ColorSensor
{
  private:
    int serialValue [4];
    uint8_t mode;
    SoftwareSerial sensorSerial;
    void sendMode();
  public:
    // 
    Ev3ColorSensor(uint8_t rx, uint8_t tx, uint8_t newMode = COLOR): 
      sensorSerial (rx, tx), mode (newMode), serialValue {0,0,0,0}{};
    
    // Start sensor communication
    void begin();
    
    // Returns a color number |
    // 0	No color |
    // 1	Black |
    // 2	Blue |
    // 3	Green |
    // 4	Yellow |
    // 5	Red |
    // 6	White |
    // 7	Brown |
    int8_t read();

    // Acept a difined constant |
    // RED_LIGHT |
    // BLUE_LIGHT |
    // COLOR_LIGHT |
    // OFF |
    void setMode(uint8_t newMode);
};

#endif // EV3COLORSENSOR_H