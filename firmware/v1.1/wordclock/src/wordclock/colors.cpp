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
#include "wordclock/colors.hpp"

Colors::Colors() {
	clear();
}

Colors::Colors(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White) {
	set(u8Red, u8Green, u8Blue, u8White);
}

void Colors::clear() {
	red = 0;
	green = 0;
	blue = 0;
	white = 0;
}

bool Colors::isClear() {
	if (red == 0 && green == 0 && blue == 0 && white == 0) {
		return true;
	}

	return false;
}

void Colors::setWhiteOnly(uint8_t u8Val) {
	clear();
#ifdef USING_WS2812
	red = u8Val;
	green = u8Val;
	blue = u8Val;
#else
	white = u8Val;
#endif
}

void Colors::setWhite(uint8_t u8Val) {
	white = u8Val;
}

uint8_t Colors::getWhite() const {
	return white;
}

void Colors::set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White) {
	red = u8Red;
	green = u8Green;
	blue = u8Blue;
	white = u8White;
}

void Colors::dimm(uint16_t u16Val) {
	uint8_t u8tmp;

	if (u16Val == 0) {
		clear();
		return;
	}

#define set(val) u8tmp = val; \
			val = val * u16Val / 0xff;\
			if(val == 0 && u8tmp != 0){\
				val = 1;\
			}

	set(red);
	set(green);
	set(blue);
	set(white);
#undef set
}

#define LIMIT 0x10
void Colors::setRandom(DisplayMode eMode) {
	uint8_t u8red = red;
	uint8_t u8green = green;
	uint8_t u8blue = blue;
	uint8_t u8white = white;

	if (isClear()) {
		return;
	}

	if (eMode == DisplayMode::Colorful || eMode == DisplayMode::Word) {
		if(!(red > LIMIT || green > LIMIT || blue > LIMIT || white > LIMIT)){
			return;
		}

		do {
			if (u8red != 0) {
				red = rand() % u8red;
			}
			if (u8green != 0) {
				green = rand() % u8green;
			}
			if (u8blue != 0) {
				blue = rand() % u8blue;
			}
			if (u8white != 0) {
				white = rand() % u8white;
			}
		} while (isClear() || !(red > LIMIT || green > LIMIT || blue > LIMIT || white > LIMIT));
	}
	else if (eMode == DisplayMode::Colorful2 || eMode == DisplayMode::Word2) {
		do {
			red = rand() % 2 == 0 ? u8red : 0;
			green = rand() % 2 == 0 ? u8green : 0;
			blue = rand() % 2 == 0 ? u8blue : 0;
			white = rand() % 2 == 0 ? u8white : 0;
		} while (isClear());
	}
}

void Colors::setRedOnly(uint8_t u8Val) {
	clear();
	red = u8Val;
}

void Colors::setGreenOnly(uint8_t u8Val) {
	clear();
	green = u8Val;
}

void Colors::setBlueOnly(uint8_t u8Val) {
	clear();
	blue = u8Val;
}

void Colors::setRed(uint8_t u8Val) {
	red = u8Val;
}

void Colors::setGreen(uint8_t u8Val) {
	green = u8Val;
}

void Colors::setBlue(uint8_t u8Val) {
	blue = u8Val;
}

uint8_t Colors::getRed() const {
	return red;
}

uint8_t Colors::getGreen() const {
	return green;
}

uint8_t Colors::getBlue() const {
	return blue;
}
