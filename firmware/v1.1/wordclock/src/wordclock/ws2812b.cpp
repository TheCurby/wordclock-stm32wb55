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
#include "wordclock/ws2812b.hpp"

WS2812B::WS2812B() {
	clear();
}

void WS2812B::setRed(uint8_t u8Val) {
	setByte(red, u8Val);
}

void WS2812B::setGreen(uint8_t u8Val) {
	setByte(green, u8Val);
}

void WS2812B::setBlue(uint8_t u8Val) {
	setByte(blue, u8Val);
}

void WS2812B::set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue) {
	setByte(red, u8Red);
	setByte(green, u8Green);
	setByte(blue, u8Blue);
}

void WS2812B::set(Colors& oColors) {
	setByte(red, oColors.getRed());
	setByte(green, oColors.getGreen());
	setByte(blue, oColors.getBlue());

#ifdef _SIMULATOR_
	this->oColors = oColors;
#endif
}

void WS2812B::clear() {
	setRed(0);
	setGreen(0);
	setBlue(0);
}

void WS2812B::setByte(uint8_t* p, uint8_t u8Val) {
	for (uint8_t i = 0; i < 8; i++) {
		if (u8Val & (1 << i)) {
			p[7 - i] = u8High;
		}
		else {
			p[7 - i] = u8Low;
		}
	}
}
