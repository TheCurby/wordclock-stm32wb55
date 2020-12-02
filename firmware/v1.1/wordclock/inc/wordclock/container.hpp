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
#include "wordclock/colors.hpp"
#include "config.hpp"

class Container {
	private:
		static constexpr uint16_t u8BufferSize = LEDS * 2;

	public:
		struct s_Point {
				int16_t u8X;
				int16_t u8Y;
				Colors oColors;
		};

		Container();
		void combine(Container& oContainer);
		void merge(Container& oContainer);
		void subtract(Container& oContainer);
		void clear();
		void setWord(int16_t u8X, int16_t u8Y, uint8_t u8Length_l, Colors& oColors, DisplayMode eMode = DisplayMode::Normal);
		void drawImage(const Image& oZ, int16_t u8OffsetX, int16_t u8OffsetY, Colors& oColors_l, DisplayMode eMode = DisplayMode::Normal);
		bool empty();
		void moveX(int16_t s16Val);
		void moveY(int16_t s16Val);
		uint8_t size() const;
		bool add(s_Point& oPoint);
		s_Point remove(uint16_t u16Pos = 0);
		s_Point get(uint16_t u16Pos = 0) const;
		s_Point& getRef(uint16_t u16Pos = 0);

	private:
		uint16_t u16Length;
		s_Point aoPoints[u8BufferSize];
};
