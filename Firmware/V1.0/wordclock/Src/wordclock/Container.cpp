#include "wordclock/Container.hpp"

Container::Container() {
	clear();
}

void Container::combine(Container& oContainer) {
	for (uint8_t i = 0; i < oContainer.size(); i++) {
		add(oContainer.get(i));
	}
}

void Container::merge(Container& oContainer) {
	for (uint8_t i = 0; i < oContainer.size(); i++) {
		s_Point& oPoint = oContainer.getRef(i);
		bool bInclude = false;

		for (uint16_t j = 0; j < u8Length; j++) {
			if (aoPoints[j].u8X == oPoint.u8X && aoPoints[j].u8Y == oPoint.u8Y) {
				bInclude = true;
				break;
			}
		}

		if (!bInclude) {
			add(oContainer.get(i));
		}
	}
}

void Container::clear() {
	u8Length = 0;
}

uint8_t Container::size() const {
	return u8Length;
}

bool Container::empty() {
	if (u8Length != 0) {
		return false;
	}
	else {
		return true;
	}
}

void Container::moveX(int16_t s16Val) {
	for (uint8_t i = 0; i < u8Length; i++) {
		aoPoints[i].u8X += s16Val;
	}
}

void Container::moveY(int16_t s16Val) {
	for (uint8_t i = 0; i < u8Length; i++) {
		aoPoints[i].u8Y += s16Val;
	}
}

bool Container::add(s_Point oPoint) {
	if (u8Length >= LEDS * 2) {
		return false;
	}
	else {
		aoPoints[u8Length] = oPoint;
		u8Length++;

		return true;
	}
}

Container::s_Point Container::remove(uint8_t u8Pos) {
	s_Point oPoint = aoPoints[u8Pos];

	for (uint8_t i = u8Pos; i < u8Length - 1; i++) {
		aoPoints[i] = aoPoints[i + 1];
	}
	u8Length--;

	return oPoint;
}

Container::s_Point Container::get(uint8_t u8Pos) const {
	return aoPoints[u8Pos];
}

Container::s_Point& Container::getRef(uint8_t u8Pos) {
	return aoPoints[u8Pos];
}

void Container::setWord(uint8_t u8Led, uint8_t u8Length_l, Colors& oColors, bool bRandomColor) {
	setWord(u8Led % WIDTH, u8Led / WIDTH, u8Length_l, oColors, bRandomColor);
}

void Container::setWord(uint8_t u8X, uint8_t u8Y, uint8_t u8Length_l, Colors& oColors, bool bRandomColor) {
	s_Point oPoint;

	for (uint8_t i = 0; i < u8Length_l; i++) {
		oPoint.oColors = oColors;
		if (bRandomColor) {
			oPoint.oColors.setRandom();
		}

		oPoint.u8X = u8X + i;
		oPoint.u8Y = u8Y;

		if (!add(oPoint)) {
			return;
		}
	}
}
