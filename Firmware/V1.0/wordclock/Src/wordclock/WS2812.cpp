#include "wordclock/ws2812.hpp"

WS2812::WS2812() {
	clear();
}

void WS2812::setRed(uint8_t u8Val) {
	setByte(red, u8Val);
}

void WS2812::setGreen(uint8_t u8Val) {
	setByte(green, u8Val);
}

void WS2812::setBlue(uint8_t u8Val) {
	setByte(blue, u8Val);
}

void WS2812::set(uint8_t u8Red, uint8_t u8Green, uint8_t u8Blue) {
	setByte(red, u8Red);
	setByte(green, u8Green);
	setByte(blue, u8Blue);
}

void WS2812::set(Colors& oColors) {
	setByte(red, oColors.getRed());
	setByte(green, oColors.getGreen());
	setByte(blue, oColors.getBlue());
}

void WS2812::clear() {
	setRed(0);
	setGreen(0);
	setBlue(0);
}

void WS2812::setByte(uint8_t* p, uint8_t u8Val) {
	for (uint8_t i = 0; i < 8; i++) {
		if (u8Val & (1 << i)) {
			p[7 - i] = u8High;
		}
		else {
			p[7 - i] = u8Low;
		}
	}
}
