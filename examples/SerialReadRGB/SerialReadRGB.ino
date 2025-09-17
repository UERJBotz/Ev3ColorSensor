#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor(8, 7);

void setup() {
    Serial.begin(9600);
    sensor.begin();
    sensor.setMode(Ev3ColorSensorMode::RGB);
}

void loop() {
    Ev3ColorSensorResult color = sensor.read();
    Ev3RGB rgb = color.rgb;

    Serial.println(rgb.r); Serial.print(" ");
    Serial.println(rgb.g); Serial.print(" ");
    Serial.println(rgb.b); Serial.print(" ");
    Serial.println("");
}

