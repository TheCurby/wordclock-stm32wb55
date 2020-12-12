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
#include "libs/onewire.hpp"
#include "config.hpp"

#ifdef USING_DS18B20
#define DS_FAMILY 0x28
#else
#define DS_FAMILY 0x10
#endif

class DS18X20 {
	public:
		DS18X20(OneWire& oOneWire_l);
		bool change();
		void getPower();
		bool readTemp();
		bool readTempParallel();
		int16_t getTempRaw() ;
		int16_t getTemp() ;
		float getTempFloat() ;
#ifdef USING_DS18B20
		bool setResolution(uint8_t u8Resolution);
#endif

	protected:
		bool bParasitePower;
		uint8_t u8Status;
		int16_t s16Temp;
		int16_t s16TempOld;
		Timer tTimeout;
		OneWire::s_ROMCode oROMCode;
		OneWire& oOneWire;

		struct {
				uint8_t au8Temp[2];
				uint8_t u8TH;
				uint8_t u8TL;
#ifndef USING_DS18B20
				uint8_t au8Void[2];
				uint8_t u8CountRemain;
				uint8_t u8CountPer;
#else
				uint8_t u8Config;
				uint8_t au8Void[3];
#endif
				uint8_t u8CRC;
		} scratchpad;

		bool convert();
		bool startConvert();
		bool read();
		bool write();

		static constexpr uint16_t u16Timeout = 5000;
};
