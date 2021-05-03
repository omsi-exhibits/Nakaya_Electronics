#ifndef HEARBEAT_H
#define HEARTBEAT_H
#include <Arduino.h>
/*
 * Flashes a led / digitalPin on and off at a rate of timeStep
 * The code is none blocking. Its function is to provide a constat
 * Flash of the onboard LED. This helps to identify if the code is
 * running or as halted.
 */

class HeartBeat {
    public:
        HeartBeat(uint8_t pin);
        void update();
        void begin();
    private:
        unsigned long timer;
        uint8_t mPin;
        bool state;
        const int timeStep = 500;
};

#endif