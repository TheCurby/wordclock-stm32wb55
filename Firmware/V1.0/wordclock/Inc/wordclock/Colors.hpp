#pragma once

#include <cstdint>
#include <stdlib.h>
#include "wordclock_typedef.hpp"

class Colors {
	public:
		Colors();
		Colors(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White);

		void clear();
		bool isClear();
		void dimm(uint16_t u16Val);
		void setRandom();
		void set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White);
		void setRedOnly(uint8_t u8Val);
		void setGreenOnly(uint8_t u8Val);
		void setBlueOnly(uint8_t u8Val);
		void setWhiteOnly(uint8_t u8Val);
		void setRed(uint8_t u8Val);
		void setGreen(uint8_t u8Val);
		void setBlue(uint8_t u8Val);
		void setWhite(uint8_t u8Val);
		uint8_t getRed() const;
		uint8_t getGreen() const;
		uint8_t getBlue() const;
		uint8_t getWhite() const;

	private:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint8_t white;
};
