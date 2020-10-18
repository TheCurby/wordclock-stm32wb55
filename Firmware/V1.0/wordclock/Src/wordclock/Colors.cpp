#include "wordclock/Colors.hpp"

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

void Colors::setRandom() {
	uint8_t u8Tmp;

#define set(val)\
		if (val != 0) {\
			u8Tmp = val;\
\
			do {\
				val = rand() % u8Tmp;\
			} while (val == 0);\
		}

	set(red);
	set(green);
	set(blue);
	set(white);
#undef set
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
