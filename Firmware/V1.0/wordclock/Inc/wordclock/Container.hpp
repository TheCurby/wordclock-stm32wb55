#pragma once

#include <cstdint>
#include "wordclock/Colors.hpp"
#include "wordclock/wordclock_typedef.hpp"

class Container {
	public:
		struct s_Point {
				int16_t u8X;
				int16_t u8Y;
				Colors oColors;
		};

		Container();
		void combine(Container& oContainer);
		void merge(Container& oContainer);
		void clear();
		void setWord(uint8_t u8X, uint8_t u8Y, uint8_t u8Length_l, Colors& oColors, bool bRandomColor = false);
		void setWord(uint8_t u8Led, uint8_t u8Length_l, Colors& oColors, bool bRandomColor = false);
		bool empty();
		void moveX(int16_t s16Val);
		void moveY(int16_t s16Val);
		uint8_t size() const;
		bool add(s_Point oPoint);
		s_Point remove(uint8_t u8Pos = 0);
		s_Point get(uint8_t u8Pos = 0) const;
		s_Point& getRef(uint8_t u8Pos = 0);

	private:
		uint8_t u8Length;
		s_Point aoPoints[LEDS * 2];
};
