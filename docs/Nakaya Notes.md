# Nakaya Electronics Details

## Theory of Operation
The Nakaya Electronics are composed of a mainboard that has a ItsyBitsy32u4 microcontroller is socketed into it. The mainboard connects to 4 sensor boards and 4 led light boards. Each Sensor board has 12 reed switches that are read by 2 shift in registers.  Each light board has a 24 rgbw neoPixel ring attached. The software for the microcontroller checks each Sensor board for the snow pucks that correspond as a correct answer to the description of the snow for that section. Each section has 2 answers that work. When a correct answer is placed in the slot the light board fades in a color and then does a sparkle animation. When the piece is removed the light fades out.  

The way the software works is that there are Sensor object instances that each loop read the sensor data store it in a data object
and then there is some logic that checks wether or not a correct sensor has been just places or has been just removed. This in turn
calls the led modules trigger functions that begin fading in or fading out. The same logic is also responsible used to trigger sounds
to play when the pieces are inserted or removed. If all 4 of the same type of snowflake are in the correct slots a victory sound will play.

### Sensor Board
- Shift Register x 2
- Reed Switches
- Phoenix Connectors
- Spacers - Between board and Milkplex
- Stand Offs - Between board and bottom plate

### Task Actions
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
- [x] Make Wire Harnesses
- [x] Label sensor connectors on main board
- [x] Cover label for prototype name
- [x] Install Boards into cabinet 
- [x] Wire management

### Audio 
The wavTrigger is the audio player that interfaces with the main board via a 5pin rectangular connector. KK254 series molex connector.
This connector carries a serial signal over TX and RX lines. The connector also has 5v and gnd to power the board. The wavTrigger needs a pad solder birdged to allow for power intake from the serial input connector.

Software wise an AudioOut object is used to trigger sounds from the main code. It has a private instance of wavTrigger that is the hardware module that
plays wav files. The wavTrigger library has a line un-commented to allow interfacing via the hardware Serial1. The audio files are stereo wav files. 

### Sensor board Physical Details
 10” by 10” square board size, sensing distance desired at least 5/8". 

### Game Pieces
- Each set has 8 game pucks. Each game puck will have 1 more magnet then the
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

----

The main loop:
- Scan the sensors every timeStep interval & insert results into DataObject
- Update Data that holds sensor results
- Run logic on Data 
- - If a corresponding puck is detected, Trigger LedModule FadeIn
- - If puck is removed since lastStep Trigger LedModule FadeOut
- - Update Cooldown timers for audio triggers
- - Trigger audio clips

## LED Power consumption
(leds per ring) * (mA per led on full brightness) * (num rings)
24 * 60 * 4 = 5760 
5.7A max consumption
The power supply selected is LRS-75-5 a 14A 5v power supply

