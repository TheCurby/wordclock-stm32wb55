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
#include "libs/onewire.hpp"

OneWire::OneWire(USART& oUSART_l) : oUSART(oUSART_l) {
}

bool OneWire::reset() {
	oUSART.setBaudrate(9600);
	oUSART.send(0xF0);

	if (oUSART.get() != 0xF0) {
		oUSART.setBaudrate(115200);
		return true;
	}
	else {
		return false;
	}
}

void OneWire::setBool(const bool bVal) {
	oUSART.send(bVal ? 0xff : 0x00);
	oUSART.get();
}

bool OneWire::getBool() {
	oUSART.send(0xff);

	if (oUSART.get() == 0xff) {
		return true;
	}
	else {
		return false;
	}
}

void OneWire::setByte(const uint8_t u8Val) {
	for (uint8_t i = 0; i < 8; i++) {
		if (u8Val & (1 << i)) {
			setBool(true);
		}
		else {
			setBool(false);
		}
	}
}

uint8_t OneWire::getByte() {
	uint8_t u8Result = 0;

	for (uint8_t i = 0; i < 8; i++) {
		if (getBool()) {
			u8Result |= (1 << i);
		}

	}

	return u8Result;
}

uint8_t OneWire::calcCRC(uint8_t* u8Data, uint8_t u8Length) {
	uint_fast8_t u8CRC = 0;
	uint_fast8_t u8Tmp;

	for (uint8_t i = 0; i < u8Length; i++) {
		for (uint8_t j = 0; j < 8; j++) {
			u8Tmp = (u8Data[i] >> j) & 1;
			u8Tmp = u8Tmp ^ (u8CRC & 1);

			u8CRC ^= (u8Tmp << 4);
			u8CRC ^= (u8Tmp << 3);
			u8CRC >>= 1;

			if (u8Tmp > 0) {
				u8CRC |= 0x80;
			}
		}
	}

	return u8CRC;
}

bool OneWire::readROM(s_ROMCode& oROMCode) {
	if (reset()) {
		setByte(0x33);

		oROMCode.u8Family = getByte();
		for (uint8_t i = 0; i < 6; i++) {
			oROMCode.au8Code[i] = getByte();
		}
		oROMCode.u8CRC = getByte();

		uint8_t u8CRC = calcCRC(&oROMCode.u8Family, 7);
		if (oROMCode.u8CRC != u8CRC) {
			return false;
		}

		return true;
	}
	else {
		return false;
	}
}

bool OneWire::matchROM(s_ROMCode& oROMCode) {
	uint8_t* u8p = &oROMCode.u8Family;

	oROMCode.u8CRC = calcCRC(&oROMCode.u8Family, 7);

	if (reset()) {
		setByte(0x55);

		for (uint8_t i = 0; i < 64; i++) {
			bool bVal = u8p[i / 8] & (1 << (i % 8)) ? true : false;
			setBool(bVal);

			if (!getBool()) {
				return false;
			}
		}

		return true;
	}
	else {
		return false;
	}
}

bool OneWire::skipROM() {
	if (reset()) {
		setByte(0xcc);
		return true;
	}
	else {
		return false;
	}
}
