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
#include <stdlib.h>
#include "config.hpp"

class Colors {
	public:
		Colors();
		Colors(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White);

		void clear();
		bool isClear();
		void dimm(uint16_t u16Val);
		void setRandom(DisplayMode eMode);
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
