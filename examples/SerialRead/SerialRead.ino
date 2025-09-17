#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor(8, 7);

void setup() {
    Serial.begin(9600);
    sensor.begin();
}

void loop() {
    Ev3ColorResult res = sensor.read();
    Ev3Color color = res.color;
    Serial.println(Ev3ColorString(color));
}

