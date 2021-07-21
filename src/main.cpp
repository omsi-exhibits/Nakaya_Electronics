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
LedModule ledModules[NUM_LEDMODULES] = {LedModule(LEDPIN1, NUMLEDS), LedModule(LEDPIN2, NUMLEDS),LedModule(LEDPIN4, NUMLEDS), LedModule(LEDPIN4, NUMLEDS)};

// array of arrays of correct pucks per sensor unit
int correctPucks[NUM_SENSORS][OPTIONS] = { {2,8}, {3,6}, {5,1}, {7,4} };

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
      if((data.count(i) == cp1)) {
        ledModules[i].triggerFadeIn1();
        audioOut.playTrack(1); // changed track
        audioOut.playTrackSolo(2); // changed track
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in1 Triggered");
        #endif
      }
      else if (data.count(i) == cp2) {
        ledModules[i].triggerFadeIn2();
        audioOut.playTrack(1); // changed track
        audioOut.playTrackSolo(3); // changed track
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in2 Triggered");
        #endif
      }
      else if (data.lastCount(i) == cp1) {
        ledModules[i].triggerFadeOut1();
        audioOut.playTrack(4); //puck removed
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out1 Triggered");
        #endif
      }
      else if (data.lastCount(i) == cp2) {
        ledModules[i].triggerFadeOut2();
        audioOut.playTrack(4); //puck removed
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out2 Triggered");
        #endif
      } else {
        // number of magnets. Previous was not right and its still not right.
        if(data.count(i) == 0) {
          audioOut.playTrack(4); //puck incorrect removed
        } else {
          audioOut.playTrack(1); //puck incorrect placed
        }

      }

    } // end data changed
  }
}

