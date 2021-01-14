////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.1
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

#include "util/timer.hpp"
#include "util/clock.hpp"

class DCF77 {
	public:
		DCF77();
		bool receive(bool bVal);
		s_RTC getRTC(int8_t s8TimeZone = -1);

	protected:
		bool bSecond;
		bool bSense;
		uint8_t u8Pos;
		uint8_t au8Data[8];
		Timer t;
		s_RTC oRTC;
		s_RTC oRTC_prev;

		void Reset();
		s_RTC Decode();
		bool Validate();
		uint8_t getBCD(uint8_t u8Pos_l, uint8_t u8Length);
		uint8_t bcd2val(uint8_t ucVal);
		uint8_t val2bcd(uint8_t ucVal);
};
