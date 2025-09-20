// Example project using EV3 color sensor with Arduino

#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor(8, 7, AMBIENT);

void setup(){
    Serial.begin(9600);
    sensor.begin();
}

void loop(){
    Ev3ColorResult light = sensor.read();
    Serial.println(light.intensity);
}
