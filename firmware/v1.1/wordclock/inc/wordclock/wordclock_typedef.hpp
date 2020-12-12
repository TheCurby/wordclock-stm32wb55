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

#include <cstdint>

constexpr uint16_t WIDTH = 11;
constexpr uint16_t HEIGHT = 10;
constexpr uint16_t LEDS = (WIDTH * HEIGHT + 4);

enum DisplayMode : uint8_t {
	Normal,
	Colorful,
	Word,
	Colorful2,
	Word2,
	COUNT_MODE
};

enum Language : uint8_t {
	German,
	Dutch,
	English,
	Italian,
	COUNT_LANG,

	Francais,
	Spanish
};

enum class Menu : uint8_t {
	DCF77,
	Hours,
	Minutes,
	Colors,
	Light,
	Animation,
	Night,
	Lang,
	MENU_END,
	Clock,
	Temp,
	Seconds,
	Test1,
	Test2
};

struct Image {
		uint8_t u8Size;
		struct {
				uint8_t u8X;
				uint8_t u8Y;
		} u8Points[];
};
