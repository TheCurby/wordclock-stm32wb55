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

#include <stdint.h>
#include "util/clock.hpp"
#include "driver/stm32.hpp"
#include "stm32wb55xx.h"

class RTClock {
	public:
		static void set(const s_RTC oRTC);    //Sets the RTC of the STM32
		static s_RTC get();    //Reads the RTC of the STM32
		static int32_t toTimeStamp(const s_RTC& oRTC);

	private:
		static void disableWP();    //Disables write protexction fpr the RTC
		static uint8_t bcd2val(const uint8_t ucVal);
		static uint8_t val2bcd(const uint8_t ucVal);
};
