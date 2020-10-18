#pragma once

constexpr uint8_t WIDTH = 11;
constexpr uint8_t HEIGHT = 10;
constexpr uint8_t LEDS = (WIDTH * HEIGHT + 4);

//#define USING_WS2812

enum Language : uint8_t {
	Deutsch,
	Nederlands,
	English,
	COUNT_LANG,

	Francais
};
