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
#include "wordclock/container.hpp"

Container::Container() {
	clear();
}

void Container::combine(Container& oContainer) {
	for (uint16_t i = 0; i < oContainer.size(); i++) {
		add(oContainer.getRef(i));
	}
}

void Container::merge(Container& oContainer) {
	bool bInclude;

	for (uint16_t i = 0; i < oContainer.size(); i++) {
		s_Point& oPoint = oContainer.getRef(i);
		bInclude = false;

		for (uint16_t j = 0; j < u16Length; j++) {
			if (aoPoints[j].u8X == oPoint.u8X && aoPoints[j].u8Y == oPoint.u8Y) {
				bInclude = true;
				break;
			}
		}

		if (!bInclude) {
			add(oPoint);
		}
	}
}

void Container::subtract(Container& oContainer){
	bool bInclude;

	for (uint16_t j = u16Length; j-- > 0;) {
		bInclude = false;

		for (uint16_t i = 0; i < oContainer.size(); i++) {
			s_Point& oPoint = oContainer.getRef(i);
			if (aoPoints[j].u8X == oPoint.u8X && aoPoints[j].u8Y == oPoint.u8Y) {
				bInclude = true;
				break;
			}
		}

		if (!bInclude) {
			remove(j);
		}
	}
}

void Container::clear() {
	u16Length = 0;
}

uint8_t Container::size() const {
	return u16Length;
}

bool Container::empty() {
	if (u16Length != 0) {
		return false;
	}
	else {
		return true;
	}
}

void Container::moveX(int16_t s16Val) {
	for (uint16_t i = 0; i < u16Length; i++) {
		aoPoints[i].u8X += s16Val;
	}
}

void Container::moveY(int16_t s16Val) {
	for (uint16_t i = 0; i < u16Length; i++) {
		aoPoints[i].u8Y += s16Val;
	}
}

bool Container::add(s_Point& oPoint) {
	if (u16Length >= u8BufferSize) {
		return false;
	}
	else {
		aoPoints[u16Length] = oPoint;
		u16Length++;

		return true;
	}
}

Container::s_Point Container::remove(uint16_t u16Pos) {
	if(u16Pos >= u16Length){
		return aoPoints[0];
	}

	s_Point oPoint = aoPoints[u16Pos];

	for (uint16_t i = u16Pos; i < u16Length - 1; i++) {
		aoPoints[i] = aoPoints[i + 1];
	}
	u16Length--;

	return oPoint;
}

Container::s_Point Container::get(uint16_t u16Pos) const {
	return aoPoints[u16Pos];
}

Container::s_Point& Container::getRef(uint16_t u16Pos) {
	return aoPoints[u16Pos];
}

void Container::setWord(int16_t u8X, int16_t u8Y, uint8_t u8Length_l, Colors& oColors, DisplayMode eMode) {
	s_Point oPoint;
	oPoint.oColors = oColors;

	if (eMode == DisplayMode::Word || eMode == DisplayMode::Word2) {
		oPoint.oColors.setRandom(eMode);
	}

	for (uint16_t i = 0; i < u8Length_l; i++) {
		if (eMode == DisplayMode::Colorful || eMode == DisplayMode::Colorful2) {
			oPoint.oColors = oColors;
			oPoint.oColors.setRandom(eMode);
		}

		oPoint.u8X = u8X + i;
		oPoint.u8Y = u8Y;

		add(oPoint);
	}
}

void Container::drawImage(const Image& oZ, int16_t u8OffsetX, int16_t u8OffsetY, Colors& oColors_l, DisplayMode eMode) {
	s_Point oPoint;
	oPoint.oColors = oColors_l;

	if (eMode == DisplayMode::Word || eMode == DisplayMode::Word2) {
		oPoint.oColors.setRandom(eMode);
	}

	for (uint16_t i = 0; i < oZ.u8Size; i++) {
		if (eMode == DisplayMode::Colorful || eMode == DisplayMode::Colorful2) {
			oPoint.oColors = oColors_l;
			oPoint.oColors.setRandom(eMode);
		}

		oPoint.u8X = oZ.u8Points[i].u8X + u8OffsetX;
		oPoint.u8Y = oZ.u8Points[i].u8Y + u8OffsetY;
		add(oPoint);
	}
}
