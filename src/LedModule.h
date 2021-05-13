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
        void triggerFadeIn1();
        void triggerFadeIn2();
        void triggerFadeOut1();
        void triggerFadeOut2();
        uint32_t lerpColor(uint32_t colorStart, uint32_t colorEnd, float deltaTime);
        uint8_t lerpSingle(uint8_t startColor, uint8_t endColor, float deltaTime);
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
        static const int mFadeInDuration = 400;
        static const int mFadeOutDuration = 400;
        unsigned int mCurrentFadeOutDuration;
        uint32_t mFadeStartColor;
        uint32_t mTargetColor;
        const uint32_t mFadeColor1 = Adafruit_NeoPixel::Color(0,0,255);
        const uint32_t mFadeColor2 = Adafruit_NeoPixel::Color(255,200,200);

};

#endif
