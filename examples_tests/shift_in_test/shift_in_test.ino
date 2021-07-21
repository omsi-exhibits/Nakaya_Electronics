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

#define SER1 12
#define CLK1 11
#define SHLD1 10

bool blink = false;

void setup() {
  Serial.begin(115200);
  pinMode(SER1, OUTPUT);
  pinMode(CLK1, OUTPUT);
  pinMode(SHLD1, OUTPUT);
  
}

void loop() {
  // Write pulse to load pin
  digitalWrite(SHLD1, LOW); // Enables the parallel inputs
  delayMicroseconds(5);
  digitalWrite(SHLD1, HIGH); // Inhibits parallel inputs
  delayMicroseconds(5);

  // Clock inhibit is already tied to ground physically
  // Clocking is acomplished by a low to high transition
  byte incoming = myShiftIn(SER1, CLK1, LSBFIRST);
  Serial.print("B1: ");
  Serial.print(incoming, BIN);
  //delay(2);
  incoming = myShiftIn(SER1, CLK1, LSBFIRST);
  Serial.print("B2: ");
  Serial.println(incoming, BIN);
  //Serial.write(incoming);
  
  if(blink) {
      digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
  blink = !blink;
  delay(200);
} // end loop


uint8_t myShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
  uint8_t value = 0;
  uint8_t i;
  for (i = 0; i < 8; ++i) {
    digitalWrite(clockPin, LOW);
    if (bitOrder == LSBFIRST)
      value |= digitalRead(dataPin) << i;
    else
      value |= digitalRead(dataPin) << (7 - i);
    digitalWrite(clockPin, HIGH);
  }
  return value;
}
