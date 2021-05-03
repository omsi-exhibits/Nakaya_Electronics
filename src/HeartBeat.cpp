#include "HeartBeat.h"
HeartBeat::HeartBeat(uint8_t pin) {
    mPin = pin;
    state = false;
    timer = 0;
}
void HeartBeat::begin() {
    pinMode(mPin, OUTPUT);
}
void HeartBeat::update() {
    if(millis() - timer >= timeStep) {
        if(state)
            digitalWrite(mPin,HIGH);
        else 
            digitalWrite(mPin, LOW);
        state = !state;
        timer = millis();
    }
}