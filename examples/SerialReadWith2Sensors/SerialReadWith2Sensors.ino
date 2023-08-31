// Exemple project using 2 EV3 color sensors with Arduino

#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor1(8, 7);
Ev3ColorSensor sensor2(9, 7);
bool s = true;
uint8_t color;

// colors table from sensor
String tableColor[8] = {
  "No color",
  "Black",
  "Blue",
  "Green",
  "Yellow",
  "Red",
  "White",
  "Brown",
};

void setup(){
  Serial.begin(9600);
  sensor1.begin();
  sensor2.begin();
}

void loop(){
    sensor1.activate();
    Serial.print("S1: ");
    Serial.print(tableColor[sensor1.read()]);
    sensor2.activate();
    Serial.print(", S2: ");
    Serial.println(tableColor[sensor2.read()]);
}