# Introduction
<div style="float:left;"><img src="/images/IMG3.JPG" align="left" width="200"></div>

**ℹ️`This wordclock project contains a simulator. This simulator is using the same sources as the STM32 project. So you can explore this project without building it.`**

**⚠️`This project contains some small smd parts. You should be able to solder VQFN parts.`**

This wordclock is based on a STM32WB55CEU6 microcontroller placed on a custom printed circuit board.
It is designed using the STM32CubeIDE without LL or HAL Driver, just CMSIS.
It is strongly encapsulated in classes and written in C++.
This project only provides a pcb and the firmware/software. An eclosure has to be designed by yourself.
You have to solder the LEDs directly on the pcb and no stripes are needed. You can use SK6812 or WS2812B.
You can use DS18B20 or DS18S20.

<img src="/images/simulator.jpg" align="right" >On the PCB are three capacitive touch sensors. The microcontroller provides the possibility to use internal touch sensors but I decided to use MTCH105 instead. The sensors are placed directly in copper. As an alternative there is a socket to plug in mechanical switches.

<div style="float:left;"><img src="/images/img4.jpg" align="left" width="200"></div>The STM32WB55CEU6 provides the possibility of BLE connectivity on chip. The antenna is implemented on the PCB. There is no app programmed yet and no support implemented in the firmware yet. Feel free to contribute this. 
The wordclock is designed to work without external tools like apps or remote control. The goal is that every functionality can be configured using the cap keys of the clock and an app should only be optional and should not deliver additional possibilities.

Of course you can use any DCF77 module you want. But the PCB and firmware is designed to use the following module: https://de.elv.com/dcf-empfangsmodul-dcf-2-091610

The clock can be powered using a 5V to 18V supply. 

<table border="0" style="border:0px;">
<tr><td>
<img src="/images/IMG1.JPG" align="left" >
</td><td>
<img src="/images/IMG2.JPG" align="right" >
</td></tr></table>

# ✔ Features
* Shows Time, Seconds and temperature (0.2°C steps using the dots to display)
* Enable / Disable the output like an old television with a red dot indicating
* Enable / Disable the DCF77 receiver
* Setting the clock manually
* Setting Color (RGBW)
  * Five different color modes:
    * Normal -> whole screen gets a color mixed by choosen colors
	* Each word gets a different color -> color is randomly mixed by choosen colors 
	* Each letter gets a different color -> color is randomly mixed by choosen colors 
	* Each word gets a different color -> one color from choosen colors 
	* Each letter gets a different color -> one color from choosen colors 
* Setting the LDR configuration for automatic luminosity correction
* Choosing the animation (switching between 5 minute steps)
  * No Animation
  * Random Animation
  * Fading in/out
  * Moving with random direction
  * Plopping randomly in/out
  * Falling down
  * Green Matrix
  * Teletype
* Enable / Disable night mode
* Supported languages: German, Dutch, English, Italian

# ⌛ In progress
* implementing languages (French, Spanish)
* finalize description
* implementing BLE support (firmware, app) and app connectivity
* commentating whole source code

# 🏁 Features planed for next version
None.
  
# ℹ️ Things that won't be implemented 
There are many wordclock projects out there. Many of them have hundreds of functionalities and are overengineered in my opinion. This project is meant to provide only useful features. So the following things **won't** be implemented:
Of course there is no guarantee that I will change my mind.

* Games like tetris etc.
* Updates over Air: Because everyone needs a ST-Link for Development/Deployment, it is very cheap and you can get it almost everywhere so buy it 😉
* Scrolling text and weather report: In my opinion the LED matrix is not suitable for this kind of stuff
* Overlays
* Sounds
* remote control: Because everyone has a smartphone today

# ⚠️ Issues
Firmware:
* Each dots gets different color if they should get the same in Colorful2 oder Word2 mode

# 💻 Simulator
The simulator of this project is using the same sources as the microcontroller project. Any changes in these sources will appear on the next build of the simulator. For this, the directory structure must remain the same.
The three bottons on the bottom correspond to the touch screens on the pcb of the wordclock.
The simulator is giving you some info what you can do with these buttons.


# 💾 Layouts
For every language there is a .svg file in the project. The font used is Source Code Pro Semibold. The .svg files have the right spaces between the letters and dots, but pay attention to the size and position of the frame and hidden elements. The .svg files are meant to use for plotters.

**Attention: Every front has a '°C' on the last line in __one__ letter place! In the tables below it takes __two__ places!**

<table>
	<tr>
		<td>English</td><td>German</td><td>Dutch</td><td>Italian</td><td>Spanish</td>
	</tr>
	<tr>
		<td><pre>IT-IS--AMPM
--QUARTER--
TWENTYFIVE-
HALF-TEN-TO
PAST---NINE
ONESIXTHREE
FOURFIVETWO
EIGHTELEVEN
SEVENTWELVE
TEN℃-OCLOCK</pre></td>
		<td><pre>ES-IST-AMPM
ZEHNZWANZIG
FÜNFVIERTEL
--NACHVOR--
HALB-ZWÖLF-
ZWEI-SIEBEN
-DREI--FÜNF
ELFNEUNVIER
-ACHTZEHN--
-SECHS℃-UHR</pre></td>
		<td><pre>HET-IS-VIJF
TIEN---AMPM
OVER--KWART
VOOR---OVER
HALF---ÉÉN-
TWEE---DRIE
VIERVIJFZES
ZEVEN-NEGEN
ACHTTIENELF
TWAALF℃-UUR</pre></td>
	<td><pre>SONO-LE----
È-LUNA-DUEZ
TREOTTONOVE
DIECIUNDICI
DODICISETTE
QUATTROCSEI
CINQUE-MENO
E-UN-QUARTO
VENTICINQUE
DIECI℃MEZZA
</pre></td>
	<td><pre>TODO</pre></td>
	</tr>
</table>

# 📃 Firmware
TODO: Description following like how to change the mapping of the leds or how to add animations ...

# PCB
The pcb is designed in KiCad. All libraries for symbols and footprints are delivered  with the project and there are no libs used directly from the KiCad installation.
Each LED has a 100nF capacitor but since there is a very big GND and +5V plane I never used or needed them. Nonetheless, it is **recommended** to equip them.

The board size is 370x370mm. The distances of the LEDs are:
* LED -> PCB edge: 50mm
* LED -> LED (x axis): 27mm
* LED -> LED (y axis): 30mm