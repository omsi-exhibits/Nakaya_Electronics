#ifndef PUCKSENSOR_H
#define PUCKSENSOR_H
#include <Arduino.h>

class PuckSensor {
    public:
        PuckSensor(uint8_t ser, uint8_t clk, uint8_t shld);
        int countMagnets();
        void update();
        void begin();
    private:
        uint8_t mSer, mClk, mShld;
        unsigned long mTimer;
        const unsigned int mTimeStep = 50;
        byte reedData[2];
        uint8_t myShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
};
#endif