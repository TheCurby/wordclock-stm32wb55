////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>
#include <string.h>
#include <stdlib.h>
#include "wordclock/ws2812b.hpp"
#include "driver/hardware.hpp"
#include "driver/rtclock.hpp"
#include "driver/spi.hpp"
#include "driver/usart.hpp"
#include "libs/onewire.hpp"
#include "libs/dcf77.hpp"
#include "libs/m95m01.hpp"
#include "libs/ds18x20.hpp"
#include "util/key.hpp"
#include "util/timer.hpp"
#include "wordclock/animation.hpp"
#include "wordclock/container.hpp"
#include "wordclock/settings.hpp"
#include "wordclock/sk6812.hpp"
#include "wordclock/colors.hpp"
#include "config.hpp"

struct Data{
#ifdef USING_WS2812
	WS2812B LEDs[LEDS];
#else
	SK6812 LEDs[LEDS];
#endif
	uint8_t u8Void[70];
};

class Wordclock {
	public:
		/* Variablen */
		bool bOn;
		bool bNewData;
		bool bDir;
		uint8_t u8SubMenu;
		uint16_t u16ADC;
		int16_t s16TempOld;
		s_RTC oRTCOld;
		Menu CurrentMenu;
		Data oData;
		Data oDataSend;

		/* Objekte */
		Hardware oHardware;
		SPI oSPI;
		M95M01 oEEPROM;
		Settings oSettings;
		USART oUSART;
		OneWire oOneWire;
		DS18X20 oDS18X20;
		DCF77 oDCF77;
		Timer tAnimation;
		Timer tTimeout;
		Timer tDisplay;
		Timer tBlink;
		Timer tTemp;
		Key oKeyLeft;
		Key oKeyMiddle;
		Key oKeyRight;
		Key oKeyTest;
		Key oKeyRightDouble;
		Container oContainerTmp;
		Container oContainerOld;
		Container oContainerZiffer[2];
		Animation AnimationClock;
		Animation AnimationMenu;

		static const Menu MenuMapping[];
		static const uint8_t au8Mapping[LEDS];
		static const uint16_t u16TimeoutTime;
		static const uint16_t u16BlinkTime;
		static const uint16_t u16TestTime;
		static const uint16_t u16RedrawTime;
		static const uint32_t u32AnimationTime;
		static const Image* aoBuchstaben[24];
		static const Image* aoZiffern[10];
		static const Image antenna;
		static const Image error;
		static const Image ok;
		static const Image Night;
		static const Image question;
		static const Image Down;
		static const Image Up;

		/*public*/
		Wordclock();
		~Wordclock();
		bool loop();

		/* Generic */
		uint8_t getDimmValue();
		void setLED(int16_t u8X, int16_t u8Y, Colors& oColors);
		void setLED(uint16_t u8N, Colors& oColors);
		void transmit(bool bVal = true);
		bool night();

		/* screen drawing */
		void clrScreen();
		void redraw();
		void drawScreen(Menu MenuScreen);
		void drawHours(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode = DisplayMode::Normal);
		void drawMinutes(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode = DisplayMode::Normal, bool bDots = true);
		void drawText(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode = DisplayMode::Normal, bool bItIs = true);
		void drawAMPM(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode = DisplayMode::Normal);
		void drawDots(Container& oContainer, uint8_t u8Size, Colors& oColors, DisplayMode eMode = DisplayMode::Normal);
		void drawContainer(Container& oContainer_l, uint16_t u16Brightness = 0xff);
		void drawFrame(Colors& oColors_l);
		void drawCenter(Colors& oColors);

		/* Menu */
		void getKeys();
		void nextMenu();
		void plusminus(bool bVal);
		void setMenu(Menu Goto);
};
