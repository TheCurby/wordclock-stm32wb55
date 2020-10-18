#include "wordclock/sk6812.hpp"

SK6812::SK6812() {
	clear();
}

void SK6812::setRed(uint8_t u8Val) {
	setByte(red, u8Val);
}

void SK6812::setGreen(uint8_t u8Val) {
	setByte(green, u8Val);
}

void SK6812::setBlue(uint8_t u8Val) {
	setByte(blue, u8Val);
}

void SK6812::setWhite(uint8_t u8Val) {
	setByte(white, u8Val);
}

void SK6812::set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue, uint8_t u8White) {
	setByte(red, u8Red);
	setByte(green, u8Green);
	setByte(blue, u8Blue);
	setByte(white, u8White);
}

void SK6812::set(Colors& oColors) {
	setByte(red, oColors.getRed());
	setByte(green, oColors.getGreen());
	setByte(blue, oColors.getBlue());
	setByte(white, oColors.getWhite());
}

void SK6812::clear() {
	setRed(0);
	setGreen(0);
	setBlue(0);
	setWhite(0);
}

void SK6812::setByte(uint8_t* p, uint8_t u8Val) {
	for (uint8_t i = 0; i < 8; i++) {
		if (u8Val & (1 << i)) {
			p[7 - i] = u8High;
		}
		else {
			p[7 - i] = u8Low;
		}
	}
}
