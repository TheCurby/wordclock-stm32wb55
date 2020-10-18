# Introduction
This wordclock is based on a STM32WB55CEU6 microcontroller placed on a custom printed circuit board.
It is designed using the STM32CubeIDE without LL or HAL Driver, just CMSIS.
It is strongly encapsulated in classes and written in C++.
This project only provide a pcb and the software. An eclosure has to be designed by yourself.
You have to solder the LEDs directly on the pcb and no strips are needed. You can use SK6812 (**strongly** recommended since WS2812 are not tested and debugged **yet**) or WS2812.

On the PCB are three capacitive touch sensors. The mikrocontroller provides the possibility to use internal touch sensors but I decided to use MTCH105 instead. The sensors are placed directly in copper.

The STM32WB55CEU6 provides the possibility of BLE connectivity on chip. The antenna is implemented on the PCB but **not tested** yet. There is also no app programmed yet and no support implemented in the firmware yet. Feel free to contribute this. 
The wordclock is designed to work without external tools like apps or remote control. The goal is that every functionality can be configured using the cap keys of the clock and an app should only be optional and should not deliver additional possibilities.

Of course you can use any DCF77 module you want. But the PCB is designed to use the following module: https://de.elv.com/dcf-empfangsmodul-dcf-2-091610

![Front](/images/IMG1.JPG)

![Back](/images/IMG2.JPG)

# ✔ Features
* Shows Time, Seconds and temperature (0.2°C  using the dots to show)
* Enable / Disable the output like on old televisions with a red dot indicating
* Enable / Disable the DCF77 receiver
* Setting the clock manually
* Setting Color
  * Enable / Disable colorful mode
* Setting the LDR configuration for automatic luminosity correction
* Choosing the animation (switching between 5 minute steps)
  * No Animation
  * Random Animation
  * Moving with random direction
  * Plopping randomly in/out
  * Falling down
  * Green Matrix
  * Teletype
* Enable / Disable night mode
* Supported languages: German, Dutch, English

# ⌛ In progress
* workout description
* placing three mechanical switches on the back as an alternative for touch sensors
* commentating whole source code
* implementing BLE support and app connectivity (firmware, pcb already finished but not tested)
* new PCB design with
  * ambilight on board spotlighting directly on the wall
  * switching off power from LEDs in night mode
  * maybe: __replacing STM32WB55 with different generic microcontroller and addid wifi with external component__
  * maybe: wifi support

# ⚠️ Issues
Implemented but not tested yet:
* English screen
* WS2812 support

PCB:
* touch keys affect each other sometimes


# 💾 Layouts
For every language there is a .svg file in the project. The font used is Source Code Pro Semibold. The .svg files have the right spaces between the chars and dots, but pay attention to the size and position of the frame and hidden elements. The .svg files are meant to use for plotters.

**Attention: Every front has a '°C' on the last line in __one__ char place! In the tables below it takes __two__ places!**

## English
IT-IS--AMPM\
--QUARTER--\
TWENTYFIVE-\
HALF-TEN-TO\
PAST---NINE\
ONESIXTHREE\
FOURFIVETWO\
EIGHTELEVEN\
SEVENTWELVE\
TEN°C-OCLOCK

## German
ES-IST-AMPM\
ZEHNZWANZIG\
FÜNFVIERTEL\
--NACHVOR--\
HALB-ZWÖLF-\
ZWEI-SIEBEN\
-DREI--FÜNF\
ELFNEUNVIER\
-ACHTZEHN--\
-SECHS-°C-UHR; <---

## Nederlands
HET-IS-VIJF\
TIEN---AMPM\
OVER--KWART\
VOOR---OVER\
HALF---ÉÉN-\
TWEE---DRIE\
VIERVIJFZES\
ZEVEN-NEGEN\
ACHTTIENELF\
TWAALF°C-UUR\

# 📃 Firmware
TODO

# PCB
The pcb is designed in KiCad. All libraries for symbols and footprints are delivered  with the project and there are no libs used directly from the KiCad installation.
Each LED has a 100nF capacitor but since there is a very big GND and +5V plane I never used or needed them. Nonetheless, it is **recommended** to equip them.

The board size is 370x370mm. The distances of the LEDs are:
* LED -> PCB edge: 50mm
* LED -> LED (x axis): 27mm
* LED -> LED (y axis): 30mm