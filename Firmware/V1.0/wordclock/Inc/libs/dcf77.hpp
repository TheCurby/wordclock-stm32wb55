#pragma once

#include "util/timer.hpp"
#include "util/clock.hpp"

class DCF77 {
	protected:
		static constexpr uint8_t u8BufferSize = 64;

	public:
		DCF77();
		bool receive(bool bVal);
		s_RTC getRTC();

	protected:
		bool bSecond;
		bool bSense;
		uint8_t u8Pos;
		uint8_t au8Data[8];
		uint8_t au8Input[u8BufferSize];
		Timer t;
		Timer tDelay;
		s_RTC oRTC;
		s_RTC oRTC_prev;

		void Reset();
		s_RTC Decode();
		bool Validate();
		uint8_t getBCD(uint8_t u8Pos_l, uint8_t u8Length);
		uint8_t bcd2val(uint8_t ucVal);
		uint8_t val2bcd(uint8_t ucVal);
};
