#pragma once

#include <stdint.h>
#include "util/clock.hpp"
#include "system/stm32.hpp"
#include "stm32wb55xx.h"

class RTClock {
	public:
		static void set(const s_RTC oRTC);    //Zeit/Datum auf RTC schreiben
		static s_RTC get();    //Zeit/Datum von RTC lesen
		static void disableWP();    //Schreibschutz der RTC disablen

	private:
		static uint8_t bcd2val(const uint8_t ucVal);
		static uint8_t val2bcd(const uint8_t ucVal);
};
