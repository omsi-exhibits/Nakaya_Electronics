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
 * This project uses a simplified Model View Controller pattern
 * The Model data is stored in the Data object. The LedModule is the view.
 * The controller code is inside the arduino loop().
 * 
 * Daniel Smolentsev
 * Last Major Update: 5/13/21
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

// array of arrays of correct pucks per sensor unit
int correctPucks[NUM_SENSORS][OPTIONS] = { {2,8}, {3,6}, {5,1}, {7,4} };

//used to wait for the puck to be fully inserted before checking if it is the correct one
unsigned long successCooldownTimers[NUM_SENSORS] = {0,0,0,0};
unsigned long removeCooldownTimers[NUM_SENSORS] = {0,0,0,0};

#define PUCK_COOLDOWN 1000

// Data object to store sensor data
Data data = Data();

int count = 0;
int lastCount = 0;

// Utilities - HeartBeat flashes the built in led to let you know the microcontroller has not gotten stuck
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
  //Serial.print("update time: ");
  //Serial.println(millis() - timer);
  //timer = millis();
  heartBeat.update();
  // get inputs from reedswitch sensors
  for(int i = 0; i < NUM_SENSORS; i ++) {
    pSensors[i].update();
    ledModules[i].update();
  }
  
  // updates data for each section
  for(int i = 0; i < NUM_SENSORS; i ++ ) {
    int c = pSensors[i].countMagnets();
    data.setCount(i, pSensors[i].countMagnets());
    #ifdef DEBUG
      if(data.changed(i)) {
        Serial.print("Data changed in section: ");
        Serial.print(i);
        Serial.print("Count: ");
        Serial.println(c);
      }
    #endif
  }

  // once we have an array of puckSensors 
  // for each section
  // check if data changed
  // check if data matches correctPucks
  // trigger led animations
  // check if lastCount matches correctPucks
  // trigger led fade out animations

  for(int i = 0; i < NUM_SENSORS; i ++) {
    if(data.changed(i)) {
      int cp1 = correctPucks[i][0];
      int cp2 = correctPucks[i][1];

      /*
      if((data.count(i) == cp1)) {
        //ledModules[i].triggerFadeIn1();
        //audioOut.playTrack(1); 
        //audioOut.playTrackSolo(2); 
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in1 Triggered");
        #endif
      }
      else if (data.count(i) == cp2) {
        //ledModules[i].triggerFadeIn2();
        //audioOut.playTrack(1); 
        //audioOut.playTrackSolo(3); 
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in2 Triggered");
        #endif
      }
      */
      // remove this to re-instate
       
      // fade out
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
          //audioOut.playTrack(4); //puck incorrect removed
          audioOut.playTrack((i+1)*2); // play 2, 4, 6, 8 Track. Those are the reverse audio insert tracks
        }

        // play insert sound when the sensors detect a count greater than zero and last count was zero and its coooled down
        if(data.lastCount(i) == 0 && data.count(i) > 0 && successCooldownTimers[i] == 0) {
          audioOut.playTrack((i*2 + 1)); //play track 1, 3, 5, 7 Track. Those are the forward audio insert tracks
        }

      //} // uncomment to re-instate
      // update cooldown timers because a change in magnet count has been detected
      if(data.count(i) > 0) {
        //audioOut.playTrack(1); //play puck inserted sound
        successCooldownTimers[i] = millis();
      }

    } // end data changed

    // if changeTimer elapsed 
    // check if
    for(int i = 0; i < NUM_SENSORS; i ++) {
      if(successCooldownTimers[i] != 0) {
        if(millis() - successCooldownTimers[i] >= PUCK_COOLDOWN) {
          int cp1 = correctPucks[i][0];
          int cp2 = correctPucks[i][1];
          if(data.count(i) == cp1) {
            audioOut.playTrack(13+i); 
            ledModules[i].triggerFadeIn1();
            #ifdef DEBUG
              Serial.print("Section: "); Serial.print(i);
              Serial.println(" FadeIn1 Triggered");
            #endif
          } else if (data.count(i) == cp2) {
            audioOut.playTrack(13+i); 
            ledModules[i].triggerFadeIn2();
            #ifdef DEBUG
              Serial.print("Section: "); Serial.print(i);
              Serial.println(" FadeIn2 Triggered");
            #endif
          }
          // turn of the timer
          successCooldownTimers[i] = 0;
        } // end of 
      }

      // reset cool down for the remove sound
      if(removeCooldownTimers[i] != 0) {
        if(millis() - removeCooldownTimers[i] >= PUCK_COOLDOWN) {
          removeCooldownTimers[i] = 0;
        }
      }
    } // end cooldownTimers check

  }
}

