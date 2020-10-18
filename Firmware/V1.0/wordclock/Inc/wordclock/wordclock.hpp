#pragma once

#include <cstdint>
#include <string.h>
#include <stdlib.h>
#include "driver/rtclock.hpp"
#include "driver/spi.hpp"
#include "driver/usart.hpp"
#include "driver/onewire.hpp"
#include "libs/dcf77.hpp"
#include "libs/m95m01.hpp"
#include "libs/ds18x20.hpp"
#include "util/key.hpp"
#include "util/timer.hpp"
#include "util/modbus.hpp"
#include "wordclock/Animation.hpp"
#include "wordclock/Container.hpp"
#include "wordclock/settings.hpp"
#include "wordclock/SK6812.hpp"
#include "wordclock/Ws2812.hpp"
#include "wordclock/Colors.hpp"
#include "wordclock/wordclock_typedef.hpp"

class Hardware {
	public:
		Hardware();
};

class Wordclock : public Hardware {
	public:
		Wordclock();
		void loop();

		struct Image {
				uint8_t u8Size;
				struct {
						uint8_t u8X;
						uint8_t u8Y;
				} u8Points[];
		};

	private:
		enum class Menu : uint8_t {
			Uhr,
			Temp,
			Seconds,
			Test1,
			Test2,
			DCF77,
			Hours,
			Minutes,
			Colors,
			LightUp,
			LightDown,
			Animation,
			Night,
			Lang
		};

		/* Variablen */
		bool bOn;
		uint8_t u8SubMenu;
		uint16_t u16ADC;
		struct {
#ifdef USING_WS2812
				WS2812 LEDs[LEDS];
#else
				SK6812 LEDs[LEDS];
#endif
				uint8_t u8Void[70];
		} oData;
		s_RTC oRTCOld;
		Menu CurrentMenu;

		/* Objekte */
		SPI oSPI;
		M95M01 oEEPROM;
		Settings oSettings;
		USART oUSART;
		OneWire oOneWire;
		DS18X20 oDS18X20;
		DCF77 oDCF77;
		ModbusRTU oModbus;
		Timer tAnimation;
		Timer tTimeout;
		Timer tDisplay;
		Timer tBlink;
		Key oKeyLeft;
		Key oKeyMiddle;
		Key oKeyRight;
		Key oKeyTest;
		Key oKeyRightDouble;
		Container oContainerTmp;
		Container oContainerOld;
		Animation AnimationClock;
		Animation AnimationMenu;

		static const uint8_t au8Mapping[LEDS];
		static const uint16_t u16TimeoutTime;
		static const uint16_t u16BlinkTime;
		static const uint16_t u16TestTime;
		static const uint16_t u16RedrawTime;
		static const uint32_t u32AnimationTime;
		static const Image* aoBuchstaben[24];
		static const Image* aoZiffern[10];
		static const Image ok;
		static const Image error;
		static const Image N;
		static const Image Fragezeichen;
		static const Image Down;
		static const Image Up;

		/* Generic */
		Colors getColors();
		void setLED(int16_t u8X, int16_t u8Y, Colors& oColors);
		void setLED(uint8_t u8N, Colors& oColors);
		void transmit(bool bVal = true);
		void executeModbus();
		bool night();

		/* screen drawing */
		void clrScreen();
		void redraw();
		void drawScreen(Menu MenuScreen);
		void drawHours(const s_RTC& oRTC, Colors& oColors, bool bRandomColor = false);
		void drawMinutes(const s_RTC& oRTC, Colors& oColors, bool bRandomColor = false, bool bDots = true);
		void drawText(uint8_t u8Minutes, Colors& oColors, bool bRandomColor = false, bool bItIs = true);
		void drawAMPM(uint8_t u8Hours, Colors& oColors, bool bRandomColor = false);
		void drawContainer(Container& oContainer_l, uint16_t u16Brightness = 0xff);
		void drawFrame(Colors& oColors_l, bool bRandomColor = false);
		void drawCenter(Colors& oColors);
		void drawImage(const Image& oZ, uint8_t u8OffsetX, uint8_t u8OffsetY, Colors& oColors_l, bool bRandomColor = false);

		/* Menu */
		void getKeys();
		void nextMenu();
		void plusminus(bool bVal);
		void setMenu(Menu Goto);
};
