#include "PuckSensor.h"

PuckSensor::PuckSensor(uint8_t ser, uint8_t clk, uint8_t shld) {
  mSer = ser;
  mClk = clk;
  mShld = shld;
  mTimer = 0;
}
void PuckSensor::begin() {
  // Should be called from the Arduino setup function
  pinMode(mSer, OUTPUT);
  pinMode(mClk, OUTPUT);
  pinMode(mShld, OUTPUT);
}
void PuckSensor::update() {
  if(millis() - mTimer > mTimeStep) {
    // Write pulse to load pin
    digitalWrite(mShld, LOW); // Enables the parallel inputs
    delayMicroseconds(5);
    digitalWrite(mShld, HIGH); // Inhibits parallel inputs
    delayMicroseconds(5);
    // Read the two bytes from the the shift registers
    reedData[0] = myShiftIn(mSer, mClk, LSBFIRST);
    reedData[1] = myShiftIn(mSer, mClk, LSBFIRST);
    mTimer = millis();
  }
}

uint8_t PuckSensor::myShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  uint8_t value = 0;
  uint8_t i;
  for (i = 0; i < 8; ++i) {
    digitalWrite(clockPin, LOW);
    if (bitOrder == LSBFIRST)
      value |= digitalRead(dataPin) << i;
    else
      value |= digitalRead(dataPin) << (7 - i);
    digitalWrite(clockPin, HIGH);
  }
  return value;
}
int PuckSensor::countMagnets() {
  // count the number of magnets the reed switches detect
  int numMagnets = 0;
  // byte 1
  int numBytes = 2;
  byte b;
  for(int bi = 0; bi < numBytes; bi ++) {
      b = reedData[bi];
      for(int i = 0; i < 8; i++) {
          if (b & B00000001) numMagnets ++;
          b = b>>1; // bit shift the bits in b one to the right
      }
  }
  return numMagnets;
}