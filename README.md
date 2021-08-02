# Nakaya Electronics Details

## Theory of Operation
The Nakaya Electronics are composed of a mainboard that a ItsyBitsy32u4 microcontroller is socketed into it. The mainboard connects to 4 sensor boards and 4 led light boards. Each Sensor board has 12 reed switches that are read by 2 shift in registers.  Each light board has a 24 rgbw neoPixel ring attached. The software for the microcontroller checks each Sensor board for the snow pucks that correspond as a correct answer to the description of the snow for that section. Each section has 2 answers that work. When a correct answer is placed in the slot the light board fades in a color and then does a sparkle animation. When the piece is removed the light fades out.  

The way the software works is that there are Sensor object instances that each loop read the sensor data store it in a data object
and then there is some logic that checks wether or not a correct sensor has been just places or has been just removed. This in turn
calls the led modules trigger functions that begin fading in or fading out.
Power supply

### Updating software
The software is built using platformio. You can use Arduino IDE by copying the src folder
and renaming it to Nakaya and renaming the main.cpp file to Nakaya.ino. This will allow
you to use Arduino to make modifications. You will need to install any missing libraries
through the Arduino Library manager.

### Project File Organization
- docs : documentation and pictures
- src : source code
- eagle : pcb files
- examples_tests : code to help test and trouble shoot
- readme.md : basic project info its the file you are reading
- other directories are used by platformio and vscode

### Sensor Board
 10” by 10” square board size, sensing distance at 5/8" and less. 
- Shift Register x 2
- Reed Switches
- Phoenix Connectors
- Spacers - Between board and Milkplex
- Stand Offs - Between board and bottom plate

## Main board
- reverse voltage protection
- phoenix connectors
- MCU is Adafruit ItsyBitsy32u4


### Audio Option
Speaker + Amp + Sd Card + Wav Trigger = ~$250


### Game Pieces
- Each set will have 8 game pucks. Each game puck will have 1 more magnet then the
previous piece. So 1 + 2 +3 +4 +5 +6 +7 +8 = 36 magnets per set.
3 sets total = 108 magnets.
- The sensors count how many magnets are in the game piece to determine what
piece they are. 
- Each Sensor section can have 2 corresponding game pieces that trigger a success animation.

## Shift Registers

74hc165 inputs are enabled by a low level at the shift/load input.
clocking is accomplished by a low-to-high transition of the clock (CLK) input
while SH/LD is held high, and CLK INH is held low. (In my design CLK INH is
tied to ground via a trace)

Parallel load is inhibited when SH/LD is held high. While SH/LD is low the parallel
inputs to the register are enabled.

The SN74HC165 I am using needs clock to pulse from LOW to HIGH. Opposite of arduino
shiftIn().

### Task Tracking
- [x] Test Magnets
- [x] Determine requirements
- [x] Picked Magnets
- [~] Order Parts for building Bread Board (Prototypes)
- [x] Design Sensor Board
- [x] Design Main Board
- [x] Order boards
- [x] Build Out board
- [x] Test the setup
- [x] Program the Interactions
- [ ] Make Wire Harnesses
- [ ] Install Board 
- ...

### Code TODO:
- [x] Code mvp
- [x] Test multiple shift registers at same time
- [x] Build out more connectors on the Board
- [x] Figure out why the ledModule.begin() does not work from setup
- [~] Figure out fade in / out state transitions. Really close.
- - Fadeout leaves some leds on...bug 
- [x] Debounce inputs

### The Minimal Viable Product Specs:
- Scan the sensors every timeStep interval & insert results into DataObject
- Update Data that holds sensor results
- Run logic on Data 
- - If a corresponding puck is detected, Trigger LedModule leds to FadeIn
- - If puck is removed since lastStep Trigger LedModule leds to FadeOut

## LED Animations
Search for NeoPixel Examples?
Fade In, Fade Out, Pulse between colors.
Sparkles? 
Gama Correction? 
