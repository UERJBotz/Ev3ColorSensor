
# LEGO EV3 Color Sensor Adapter for Arduino

This project implements an adaptation of the LEGO EV3 Color Sensor for Arduino using serial communication. The code provided allows you to interface with the EV3 Color Sensor and read color values using an Arduino board.


## Prerequisites

To use this code and adapt the LEGO EV3 Color Sensor for Arduino, you will need the following:

- Arduino board (compatible with SoftwareSerial library)
- LEGO EV3 Color Sensor


## Installation

Follow these steps to set up the project:

* Connect the LEGO EV3 Color Sensor to the Arduino board using jumper wires. Connect the RX pin of the sensor to a digital pin on the Arduino (e.g., pin 8) and connect the TX pin of the sensor to another digital pin on the Arduino (e.g., pin 7).

* Open your Arduino IDE and create a new sketch.

* Download the Ev3ColorSensor library files (Ev3ColorSensor.h and Ev3ColorSensor.cpp) and place them in your Arduino libraries directory.

* Include the Ev3ColorSensor library by adding the following line at the top of your sketch:

    ```c++
    #include "Ev3ColorSensor.h"
    ```

    
## Usage

The Ev3ColorSensor class provides the following methods:

* begin(): Initializes the sensor communication.
* read(): Reads the color value from the sensor and returns an integer representing the color detected.
* setMode(uint8_t newMode): Sets the mode of the color sensor. The available modes are RED_LIGHT (0), BLUE_LIGHT (1), COLOR (2), and OFF (5). Use the appropriate constant when calling this method.

## Example

Here's an example of how to use the Ev3ColorSensor library in your Arduino sketch:

```c++
#include "Ev3ColorSensor.h"

Ev3ColorSensor sensor(8, 7);

// Colors table from the sensor
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

void setup() {
  Serial.begin(9600);
  sensor.begin();
}

void loop() {
  uint8_t color = sensor.read();
  Serial.println(tableColor[color]);
}

```

This example sets up the sensor, initializes the serial communication, and continuously reads the color value from the sensor, printing the corresponding color name to the Serial Monitor.
## Author

- [@TonyALima](https://github.com/TonyALima)

## License

This project is under the license [MIT](./LICENSE).


