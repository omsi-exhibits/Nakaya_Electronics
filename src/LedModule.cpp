#include "LedModule.h"
#include <Adafruit_NeoPixel.h>
//#include <Arduino.h>

LedModule::LedModule(uint8_t pin, uint8_t numLeds) {
    mPin = pin;
    mNumLeds = numLeds;
    mTimer = 0;
    mState = IDLE;
    mCount = 0;
    mStrip = Adafruit_NeoPixel(mNumLeds, mPin, NEO_GRBW + NEO_KHZ800);
    mFadeStartColor = Adafruit_NeoPixel::Color(255,0,0,0);
    mCurrentFadeOutDuration = 0;
}
void LedModule::begin() {
    mStrip.begin();
    for(int i = 0; i < mNumLeds; i ++) {
        mStrip.setPixelColor(i, 0,0,0,0);
    }
    mStrip.show();
}
uint8_t LedModule::lerpSingle(unsigned long startTime, unsigned int duration, uint8_t startColor, uint8_t endColor) {
    // lerp a single color channel. For example R for RGB.
    unsigned int deltaTime = millis() - startTime;
    uint8_t currentColor = 0;
    if(deltaTime < duration) {
        float dT = (float)deltaTime / (float)duration; //normalize to 0-1
        currentColor = (uint8_t) round(lerp(dT, 0.0, 1.0, startColor, endColor));
    } 
    return currentColor;
}

void LedModule::update() {
    switch(mState) {
        case FADEIN :
            // calculate a fadeIn Brightness 
            if(millis() - mFadeInStartTime < mFadeInDuration) {
                uint8_t bChan = lerpSingle(mFadeInStartTime, mFadeInDuration, 0, 255);
                //bChan = mStrip.gamma8(bChan);
                uint32_t currentFade = mStrip.Color(0,0,bChan,0);
                mStrip.fill(currentFade);
                mStrip.show();
                Serial.print(F("bChan: "));
                Serial.println(bChan);

            } else {
                triggerIdle();
            }
            /*
            if(millis() - mFadeInStartTime < mFadeInDuration) {
                int deltaTime = millis() - mFadeInStartTime;
                float dT = (float)deltaTime / (float)mFadeInDuration; 
                uint8_t brightness = (uint8_t) round(lerp(dT, 0.0, 1.0, 0, 255));
                brightness = mStrip.gamma8(brightness);
                Serial.print(F("brightness: "));
                Serial.println(brightness);

                uint32_t currentFade = mStrip.Color(0,0,brightness,0);
                mStrip.fill(currentFade);
                mStrip.show();
            } else {
                triggerIdle();
            }
            */
                //mStrip.show();
            break;
        case IDLE :
            break;
        case FADEOUT :
            if((millis() - mFadeOutStartTime) < mCurrentFadeOutDuration) {
                uint8_t u8R = (uint8_t)((mFadeStartColor >> 16) & 0xff),
                                u8G = (uint8_t)((mFadeStartColor >> 8) & 0xff),
                                u8B = (uint8_t)(mFadeStartColor & 0xff);
                uint8_t bChan = lerpSingle(mFadeOutStartTime, mCurrentFadeOutDuration, u8B, 0);
                //bChan = mStrip.gamma8(bChan);
                uint32_t currentFade = mStrip.Color(0,0,bChan,0);
                mStrip.fill(currentFade);
                mStrip.show();
            } else {
                triggerIdle();
            }
            /*
            if(millis() - mFadeOutStartTime < mFadeOutDuration) {
                int deltaTime = millis() - mFadeOutStartTime;
                float dT = (float)deltaTime / (float)mFadeOutDuration; 
                uint8_t brightness = (uint8_t) round(lerp(dT, 0.0, 1.0, 255, 0));
                brightness = mStrip.gamma8(brightness);
                Serial.print(F("brightness: "));
                Serial.println(brightness);

                uint32_t currentFade = mStrip.Color(0,0,brightness,0);
                mStrip.fill(currentFade);
                mStrip.show();
            } else {
                triggerIdle();
            }
            */
            break;
        default :
            Serial.println(F("Undefined state in ledModule!"));
            break;
    }
    /*
    if(millis() - mTimer >= mTimeStep) {
        // do an update else idle
        mCount = (mCount)% mNumLeds;
        if(mCount == 23) {
            for(int i = 0; i < mNumLeds; i ++) {
                mStrip.setPixelColor(i, 0,0,0);
            }
        }
        mStrip.setPixelColor(mCount, 32,0,0,0);
        //mStrip.show();
        mTimer = millis();
        Serial.println("updated leds");
    }
    */
}
void LedModule::triggerIdle() {
    mState = IDLE;
}
void LedModule::triggerFadeOut() {
    // check if fadeIn has not completed and if so progress the fadeOut to the right
    // amount so that it Fades out from where the fade in left off.
    if(mState == FADEIN) {
        mFadeStartColor = mStrip.getPixelColor(0);
        mCurrentFadeOutDuration = (millis() - mFadeInStartTime);
        Serial.print("Current fade duration: ");
        Serial.println(mCurrentFadeOutDuration);
    } else {
        mFadeStartColor = mStrip.getPixelColor(0);
        mCurrentFadeOutDuration = mFadeOutDuration;
    }
    mFadeOutStartTime = millis();
    mState = FADEOUT;
}
void LedModule::triggerFadeIn() {
    // do things on enter fadein state
    // reset fadein details
    mFadeInStartTime = millis();
    mState = FADEIN;
}

float LedModule::lerp (float x, float x0, float x1, float y0, float y1) {
    x = x > x1 ? x1 : x;
    x = x < x0 ? x0 : x;
    return y0 + (y1-y0) * ((x-x0)/(x1-x0));
}