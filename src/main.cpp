/*
 * Shift Register Test
 * ItsyBits 32u4
 * 
 * Shift in IC- SN74HC165
 * The board has 4 connections to sensor modules. Each module has
 * 2 74HC165 to collect 12 parallel inputs from the read switches
 * and send them out as serial data. The reed switches are pulled
 * low with 10k resistors.
 * 
 * Set 1 of pins
 * SER1 - 14 (MISO)
 * CLK1 - 16 (MOSI)
 * SH/LD - 15 (SCK)
 * 
 * Daniel Smolentsev
 * Last updated: 4/14/21
 */
#include <Arduino.h>
#include <Bounce2.h>
#include <Adafruit_NeoPixel.h>
#include "PuckSensor.h"
#include "LedModule.h"
#include "HeartBeat.h"
#include "Data.h"
// pin defines are stored in hw.h
#include "hw.h"
#define DEBUG

// Declare array of Sensors and leds 
#define NUM_SENSORS 4
#define NUM_LEDMODULES 4
#define OPTIONS 2
// Puck sensor connector numbers increment from right to left on the PCB so they are in descending order here
// this way in the arrays pSensor[1] and ledModules[1] correspond to the same section 
PuckSensor pSensors[NUM_SENSORS] = {PuckSensor(SER4, CLK4, SHLD4), PuckSensor(SER3, CLK3, SHLD3),PuckSensor(SER2, CLK2, SHLD2),PuckSensor(SER1, CLK1, SHLD1)};
LedModule ledModules[NUM_LEDMODULES] = {LedModule(LEDPIN1, NUMLEDS), LedModule(LEDPIN2, NUMLEDS),LedModule(LEDPIN4, NUMLEDS), LedModule(LEDPIN4, NUMLEDS)};

// array of arrays of correct pucks per sensor unit
int correctPucks[NUM_SENSORS][OPTIONS] = { {2,3}, {5,4}, {1,6}, {7,8} };

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

  // once we have a list of puckSensors 
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
        ledModules[i].triggerFadeIn();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in1 Triggered");
        #endif
      }
      else if (data.count(i) == cp2) {
        ledModules[i].triggerFadeIn();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade in2 Triggered");
        #endif
      }
      else if (data.lastCount(i) == cp1) {
        ledModules[i].triggerFadeOut();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out1 Triggered");
        #endif
      }
      else if (data.lastCount(i) == cp2) {
        ledModules[i].triggerFadeOut();
        #ifdef DEBUG
          Serial.print("Section: "); Serial.print(i);
          Serial.println(" Fade out2 Triggered");
        #endif
      }

    } // end data changed
  }

/* OLD code 
  // Logic to determin led animation triggers
  if(data.changed(3)) {
    int count = data.count(3);
    int lastCount = data.lastCount(3);
    Serial.print("count: "); Serial.println(count);
    // int correctPucks[NUM_SENSORS][OPTIONS] = { {7,8}, {6,5}, {4,1}, {2,3} };

    // if the count is correct 
    if(count == correctPucks[3][0]) {
      Serial.println("triggered led fade in [3][0]");
      ledModules[0].triggerFadeIn();
    }
    if(count == correctPucks[3][1]) {
      ledModules[0].triggerFadeIn();
    }
    if((lastCount == correctPucks[3][0]) & (count != correctPucks[3][0])) {
      ledModules[2].triggerFadeOut();
      Serial.println("fade out triggered puck 0");
    }
    if((lastCount == correctPucks[3][1]) & (count != correctPucks[3][1])) {
      ledModules[2].triggerFadeOut();
      Serial.println("fade out triggered puck 1");
    }
  }
  */

}

