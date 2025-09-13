// Example project using EV3 color sensor with Arduino

#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor(8, 7);

void setup(){
    Serial.begin(9600);
    sensor.begin();
}

void loop(){
    uint8_t color = sensor.read();
    Serial.println(Ev3ColorString(color));
}
