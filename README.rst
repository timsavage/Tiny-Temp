########
TinyTemp
########

Had all the components laying about so this is a bit of a trial project for 
making my first PCB. 

Utilises a HP QDSP-6064 bubble display, a TMP34 temperature sensor, and an 
ATTiny85, will draw very little current for a longer life.


Hardware
========

The bubble displays can be hard to come by but they do pop up on Ebay from 
time to time.

Display is driven by a pair of 74HC595 IC's, one to control segments the other 
to switch transistors to pull Cathodes to ground. As the 74HC595 can be daisy
chained on 3 pins are required to drive the display. The TMP36 uses 1 ADC 
leaving one pin free.

Note that in the schematic the 3.3 Volt regulator used is the LD33V the pin
layout differs from the 78XX series in with:
- Pin 1: GRN
- Pin 2: OUT
- Pin 3: IN

The schematic uses a 78XX series symbol.

The ICSP header is optional and only really in place for convenience.


BOM
---

1x 	HP QDSP-6064 - 7 Segment "Bubble" display
1x	ATTiny 85
1x	TMP34 - Temperature sensor
1x 	LD33V - 3.3V voltage regulator
2x	74HC595 - Shift register
4x	2N3904 - NPN transistor
5x	10k ohm resistor
4x	330 ohm resistor
2x	0.1uF MLC cap


Software
========

Use the install target to build, upload, and set correct fuses for an ATTiny85
using a USBTiny programmer. These settings can be tweaked in the Makefile.

All pins, voltage levels (defaults to 3.3V) can be configured in *defines.h*.
