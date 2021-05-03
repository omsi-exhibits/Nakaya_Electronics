#ifndef LEDMODULE_H
#define LEDMODULE_H
#include <Arduino.h>
#include <Adafruit_Neopixel.h>

enum LEDSTATE { IDLE, FADEIN, FADEOUT };

class LedModule {
    public:
        LedModule(uint8_t pin, uint8_t numLeds);
        void begin();
        void update();
        void triggerIdle();
        void triggerFadeIn();
        void triggerFadeOut();
        uint8_t lerpSingle(unsigned long startTime, unsigned int duration, uint8_t startColor, uint8_t endColor);
        float lerp(float x, float x0, float x1, float y0, float y1);

    private:
        Adafruit_NeoPixel mStrip;
        LEDSTATE mState;
        uint8_t mPin;
        uint8_t mNumLeds;
        int mCount;
        unsigned long mTimer;
        static const int mTimeStep = 80;
        // Fade Related
        unsigned long mFadeInStartTime;
        unsigned long mFadeOutStartTime;
        static const int mFadeInDuration = 2000;
        static const int mFadeOutDuration = 2000;
        int mCurrentFadeOutDuration;
        uint32_t mFadeStartColor;

};

#endif
