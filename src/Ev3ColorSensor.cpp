#include <assert.h>

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ev3ColorSensor.h>


#define LEN(arr) (sizeof(arr)/sizeof(*arr))
#define DELAY(w) for(uintmax_t t0 = millis(); millis() - t0 <= w;)


static const String Ev3ColorNameTable[] = {
    [None]   = "None",
    [Black]  = "Black",
    [Blue]   = "Blue",
    [Green]  = "Green",
    [Yellow] = "Yellow",
    [Red]    = "Red",
    [White]  = "White",
    [Brown]  = "Brown",
};
static_assert(LEN(Ev3ColorNameTable) == Ev3Color::EV3COLORCOUNT);

const String Ev3ColorString(const Ev3Color color) {
    return Ev3ColorNameTable[color];
}


void Ev3ColorSensor::begin() {
  sensorSerial.begin(2400);

  for(;;) {
    int value = sensorSerial.read();
    if (value != -1) {
      serialValue[1] = serialValue[0];
      serialValue[0] = value;
    }

    if (serialValue[0] == 128 &&
        serialValue[1] == 144) break;
  }
  while (sensorSerial.read() != 4);

  sensorSerial.write(ACK); delay(60);
  sensorSerial.end();
  sensorSerial.begin(57600);
  sendMode();
}

int Ev3ColorSensor::readByte() { //! isso precisou ser int (não int8_t), ou isso ou a variável em read()
  int curr = sensorSerial.read();
  prevSerialFailed = (curr == -1);
  return curr;
}

// https://github.com/JakubVanek/lms2012-stuff/ -> ev3sensors/color/metadata.txt
Ev3Color Ev3ColorSensor::read() {
  //! tipo de retorno errado (fazer tagged union ColorSensorResponse)
  //! converter funções da outra branch pra receber um ponteiro pra estrutura que eles têm que preencher, já preenchido com o primeiro byte

  sendMode();
  DELAY(waitTime); // ensures sensor is ready

  Ev3Color resp = None;
  while (sensorSerial.available()){
    int currByte = readByte();

    if (prevSerialFailed || done) done = false;
    else {
      switch (mode) {
        case COLOR: {
          if (currByte > EV3COLORCOUNT) break;

          done = true; resp = (Ev3Color)currByte;
        } break;
        case RED_LIGHT: case BLUE_LIGHT: {
          if (currByte > 100) break;

          done = true; resp = (Ev3Color)currByte;
        } break;

        case RAW: case RGB: //!
        case OFF: {
          done = true; resp = None;
        } break;
      }
    }
  }
  return resp;
}

void Ev3ColorSensor::setMode(Ev3ColorSensorMode newMode) { 
  if (RED_LIGHT < newMode && newMode > OFF) {
    mode = newMode; 
  } else assert(!"invalid mode");
}

void Ev3ColorSensor::sendMode() {
  sensorSerial.write(0x44);
  sensorSerial.write(0x11);
  sensorSerial.write(0xff ^ 0x44 ^ 0x11);

  for (int n = 0; n < 3; n++) {
    sensorSerial.write(0x43);
    sensorSerial.write(mode & 0x7);
    sensorSerial.write(0xff ^ 0x43 ^ (mode & 0x7));

    sensorSerial.write(NACK);
  }
}

void Ev3ColorSensor::activate(){
  sensorSerial.listen();
}
