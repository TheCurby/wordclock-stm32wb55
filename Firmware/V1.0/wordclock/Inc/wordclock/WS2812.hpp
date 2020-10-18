#pragma once

#include <cstdint>
#include "system/clocks.hpp"
#include "wordclock/Colors.hpp"

class WS2812 {
	public:
		WS2812();
		void clear();
		void setRed(uint8_t u8Val);
		void setGreen(uint8_t u8Val);
		void setBlue(uint8_t u8Val);
		void set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue);
		void set(Colors& oColors);

	private:
		uint8_t green[8];
		uint8_t red[8];
		uint8_t blue[8];

		void setByte(uint8_t* p, uint8_t u8Val);

		static constexpr uint8_t u8High = (uint32_t) ((CLK_TIMER / CLK_SK6812) - 1) * 50 / 100;
		static constexpr uint8_t u8Low = (uint32_t) ((CLK_TIMER / CLK_SK6812) - 1) * 25 / 100;
};
