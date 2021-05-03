/*
 * Shift Register Test
 * ItsyBits 32u4
 * 
 * PISO IC - 74HC165
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

#define SER1 12
#define CLK1 11
#define SHLD1 10

#define LEDPIN 9
#define NUMLEDS 24

#define HEARTLED 13
bool beat = false;

bool blink = false;
bool beginFlag = false;

// Declare Sensors
PuckSensor pS = PuckSensor(SER1, CLK1, SHLD1);
// Delcare LED modules
LedModule ledModule = LedModule(LEDPIN, NUMLEDS);
// Utilities
HeartBeat heartBeat = HeartBeat(LED_BUILTIN);

//Data data = Data();

// store puck specific number of magnets in this array
int count = 0;
int lastCount = 0;

void setup() {
  Serial.begin(115200);
  pS.begin();
  ledModule.begin();
  heartBeat.begin();
  pinMode(HEARTLED, OUTPUT);
}

void loop() {
  if(!beginFlag) {
    // quick hack for calling the begin once inside loop
    ledModule.begin();
    beginFlag = true;
  }
  // get inputs from reedswitch sensors
  heartBeat.update();
  ledModule.update();
  pS.update();

  lastCount = count;
  count = pS.countMagnets();

  // On a change of magnet count check if puck is the right one
  // for the corresponding section and trigger led animation
  if(count != lastCount) {
    Serial.print("count: "); Serial.println(count);
    if(count == 2) {
      ledModule.triggerFadeIn();
    }
    if((lastCount == 2) & (count != 2)) {
      ledModule.triggerFadeOut();
      Serial.println("fade out triggered");
    }
  }
  // do logic, light up output ledModules


}

