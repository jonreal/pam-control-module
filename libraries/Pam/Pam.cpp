#include "Pam.h"
#include <Wire.h>

Pam::Pam(char i2c_address){
  _address = i2c_address;
  _pams[0].a_sol = 0;
  _pams[0].b_sol = 0;
  _pams[1].a_sol = 0;
  _pams[1].b_sol = 0;
  _pams[2].a_sol = 0;
  _pams[2].b_sol = 0;
  _pams[3].a_sol = 0;
  _pams[4].b_sol = 0;
}

void Pam::begin(void) {
  Wire.begin();
  Wire.beginTransmission(_address);
  Wire.write(byte(0x0));               // turn off all i2c bus channels
  Wire.endTransmission();
  delay(100);
}

void Pam::setupChannel(char chan, int a_sol_pin, int b_sol_pin) {
  pinMode(a_sol_pin, OUTPUT);
  digitalWrite(a_sol_pin, LOW);
  _pams[chan].a_sol = a_sol_pin;
  pinMode(b_sol_pin, OUTPUT);
  digitalWrite(b_sol_pin, LOW);
  _pams[chan].b_sol = b_sol_pin;
}

void Pam::setValves(char chan, int u) {
  if (u == 1) {
    digitalWrite(_pams[chan].a_sol,HIGH);
    digitalWrite(_pams[chan].b_sol,LOW);
  } else if (u == -1) {
    digitalWrite(_pams[chan].a_sol,LOW);
    digitalWrite(_pams[chan].b_sol,HIGH);
  } else if (u == 0) {
    digitalWrite(_pams[chan].a_sol,LOW);
    digitalWrite(_pams[chan].b_sol,LOW);
  }
}

float Pam::getPressure(char chan) {
  setI2cBusChannel(chan);
  delay(10);

  Wire.requestFrom(SENSOR_I2C_ADDY,4);

  char index = 0;
  unsigned char buffer[4];
  while (Wire.available()) {
    buffer[index++] = Wire.read();
  }
  unsigned int bits = (((unsigned int) (buffer[0] & ~0xC0)) << 8)
              | (unsigned int) (buffer[1]);
  return ((float) bits - 1638.4)*0.01144409;
}


void Pam::setI2cBusChannel(char chan) {
  Wire.beginTransmission(_address);
  Wire.write(byte(chan + 4));
  Wire.endTransmission();
}


