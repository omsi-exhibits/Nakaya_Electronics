/* Nakaya Electronics Mainboard
 * 
 * Micro: ItsyBitys 32u4
 * 
 * The main board has 4 connections to sensor modules. Each module has
 * 2 SN74HC165 ICs to collect 12 parallel inputs from the read switches
 * and send them out as serial data. The reed switches are pulled
 * low with 10k resistors. 
 * 
 * Upon reading the correct number of magnets on the read switches the sensor board
 * will triggerFadeIn() on the corresponding ledModule
 * 
 * This project uses a simplified Model View Controller design pattern
 * The Model data is stored in the Data object. The LedModule is the view.
 * The controller code is inside the arduino loop().
 * 
 * Daniel Smolentsev
 * Last Major Update: 8/25/21
 * MVP
 */
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "PuckSensor.h"
#include "LedModule.h"
#include "HeartBeat.h"
#include "Data.h"
#include "audio/AudioOut.h"
// Audio 
AudioOut audioOut;

// pin defines are stored in hw.h
#include "hw.h"
// Comment out to disable debug messages
#define DEBUG

// Declare array of Sensors and leds 
#define NUM_SENSORS 4
#define NUM_LEDMODULES 4
#define OPTIONS 2
// Puck sensor connector numbers(order) increment from right to left on the PCB so they are in descending order here
// this way in the arrays pSensor[1] and ledModules[1] correspond to the same section 
PuckSensor pSensors[NUM_SENSORS] = {PuckSensor(SER4, CLK4, SHLD4), PuckSensor(SER3, CLK3, SHLD3),PuckSensor(SER2, CLK2, SHLD2),PuckSensor(SER1, CLK1, SHLD1)};
LedModule ledModules[NUM_LEDMODULES] = {LedModule(LEDPIN1, NUMLEDS), LedModule(LEDPIN2, NUMLEDS),LedModule(LEDPIN3, NUMLEDS), LedModule(LEDPIN4, NUMLEDS)};

// array of arrays of correct pucks to look for per sensor slot
int correctPucks[NUM_SENSORS][OPTIONS] = { {6,2}, {5,1}, {7,3}, {8,4} };

// These timers used to wait for the puck to be fully inserted before checking if it is the correct one
// because on insertion multiple puck sensor readings are triggered and need to be ignored until after the cooldown
unsigned long successCooldownTimers[NUM_SENSORS] = {0,0,0,0};
unsigned long removeCooldownTimers[NUM_SENSORS] = {0,0,0,0};

// PUCK_COOLDOWN is the delay to wait before check wether the correct puck has been inserted and playing the audio feedback.
#define PUCK_COOLDOWN 700

// When all 4 slots have the correct puck inserted and its of the same set
// the coresponding bool array will be all true and a victory sound will play
bool bentleyInSlots[NUM_SENSORS] = {false, false, false, false};
bool libbrechtInSlots[NUM_SENSORS] = {false, false, false, false};

// Data object to store sensor data
Data data = Data();

// Utilities - HeartBeat flashes the built in led every 500ms to let you know the microcontroller has not gotten stuck
HeartBeat heartBeat = HeartBeat(LED_BUILTIN);

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < NUM_SENSORS; i ++) {
    pSensors[i].begin();
    ledModules[i].begin();
  }
  heartBeat.begin();
  audioOut.begin();
  pinMode(HEARTLED, OUTPUT);
}

void loop() {
  heartBeat.update();
  // get inputs from reedswitch sensors
  for(int i = 0; i < NUM_SENSORS; i ++) {
    pSensors[i].update();
    ledModules[i].update();
  }
  
  // updates data for each section
  for(int i = 0; i < NUM_SENSORS; i ++ ) {
    int c = pSensors[i].countMagnets();
    data.setCount(i, c);
    #ifdef DEBUG
      if(data.changed(i)) {
        Serial.print("Data changed in section: ");
        Serial.print(i);
        Serial.print("Count: ");
        Serial.println(c);
      }
    #endif
  }

  // Check all what is inserted in each slot
  for(int i = 0; i < NUM_SENSORS; i ++) {
    if(data.changed(i)) {
      int cp1 = correctPucks[i][0];
      int cp2 = correctPucks[i][1];
       
      // Trigger fade outs on a detected puck remove
      if (data.lastCount(i) == cp1 ) {
        ledModules[i].triggerFadeOut1();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out1 Triggered");
        #endif
      }
      else if (data.lastCount(i) == cp2 ) {
        ledModules[i].triggerFadeOut2();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out2 Triggered");
        #endif
      } 

        // play remove sound when the count is 0 and the sensor has not seen any change in the cooldown timer window
        if(data.count(i) == 0 && removeCooldownTimers[i] == 0) {
          audioOut.playTrack(4); //puck removed
          // clear slot state to false if puck is removed
          libbrechtInSlots[i] = false;
          bentleyInSlots[i] = false;
        }

        // play insert sound when the sensors detect a count greater than zero and last count was zero and its cooled down
        if(data.lastCount(i) == 0 && data.count(i) > 0 && successCooldownTimers[i] == 0) {
          audioOut.playTrack(3); //play forward audio insert track
        }

      // update cooldown timers because a change in magnet count has been detected
      if(data.count(i) > 0) {
        successCooldownTimers[i] = millis();
      }

    } // end data changed

    for(int i = 0; i < NUM_SENSORS; i ++) {
      if(successCooldownTimers[i] != 0) {
        if(millis() - successCooldownTimers[i] >= PUCK_COOLDOWN) {
          int cp1 = correctPucks[i][0];
          int cp2 = correctPucks[i][1];
          if(data.count(i) == cp1) {
            audioOut.playTrack(5+i); 
            ledModules[i].triggerFadeIn1();
            libbrechtInSlots[i] = true;

            // Play achievement audio
            bool playAchievement = true;
            for(int j = 0; j < NUM_SENSORS; j ++) {
              if(libbrechtInSlots[j] == false)
                playAchievement = false;
            }
            if(playAchievement)
              audioOut.playTrack(1);
            // end play achievement

            #ifdef DEBUG
              Serial.print("Section: "); Serial.print(i);
              Serial.println(" FadeIn1 Triggered");
            #endif
          } else if (data.count(i) == cp2) {
            audioOut.playTrack(5+i); 
            ledModules[i].triggerFadeIn2();
            bentleyInSlots[i] = true;
            // Play achievement audio
            bool playAchievement = true;
            for(int j = 0; j < NUM_SENSORS; j ++) {
              if(bentleyInSlots[j] == false)
                playAchievement = false;
            }
            if(playAchievement)
              audioOut.playTrack(2);
            // end play achievement
            #ifdef DEBUG
              Serial.print("Section: "); Serial.print(i);
              Serial.println(" FadeIn2 Triggered");
            #endif
          }
          // turn of the timer
          successCooldownTimers[i] = 0;
        } 
      } // end if successCooldownTimers

      // reset cool down for the remove sound
      if(removeCooldownTimers[i] != 0) {
        if(millis() - removeCooldownTimers[i] >= PUCK_COOLDOWN) {
          removeCooldownTimers[i] = 0;
        }
      }
    } // end cooldownTimers check

  }
}

