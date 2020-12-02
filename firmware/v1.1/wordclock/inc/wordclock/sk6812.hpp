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
#include "driver/clocks.hpp"
#include "wordclock/colors.hpp"

class SK6812 {
	public:
		SK6812();
		void clear();
		void setRed(uint8_t u8Val);
		void setGreen(uint8_t u8Val);
		void setBlue(uint8_t u8Val);
		void setWhite(uint8_t u8Val);
		void set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White);
		void set(Colors& oColors);

#ifdef _SIMULATOR_
		Colors oColors;
#endif

	private:
		uint8_t green[8];
		uint8_t red[8];
		uint8_t blue[8];
		uint8_t white[8];

		void setByte(uint8_t* p, uint8_t u8Val);

		static constexpr uint8_t u8High = (uint32_t) ((CLK_TIMER / CLK_SK6812) - 1) * 50 / 100;
		static constexpr uint8_t u8Low = (uint32_t) ((CLK_TIMER / CLK_SK6812) - 1) * 25 / 100;
};
