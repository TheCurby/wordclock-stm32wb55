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

#include "driver/stm32.hpp"
#include "driver/usart.hpp"

class OneWire {
	public:
		typedef struct {
				uint8_t u8Family;
				uint8_t au8Code[6];
				uint8_t u8CRC;
		} s_ROMCode;

		OneWire(USART& oUSART_l);    //Initialize the OneWire. It needs an already initialized USART object.
		bool reset();
		void setBool(const bool bVal);
		bool getBool();
		void setByte(const uint8_t u8Val);
		uint8_t getByte();

		bool matchROM(s_ROMCode& oROMCode);
		bool skipROM();
		bool readROM(s_ROMCode& oROMCode);
		uint8_t calcCRC(uint8_t* u8Data, uint8_t u8Length);

	protected:
		USART& oUSART;
};
