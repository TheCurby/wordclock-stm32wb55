#pragma once

#include <stdint.h>
#include "util/clock.hpp"
#include "system/stm32.hpp"
#include "stm32wb55xx.h"

class RTClock {
	public:
		static void set(const s_RTC oRTC);    			//Sets the RTC of the STM32
		static s_RTC get();								//Reads the RTC of the STM32

	private:
		static void disableWP();   						//Disables write protexction fpr the RTC
		static uint8_t bcd2val(const uint8_t ucVal);
		static uint8_t val2bcd(const uint8_t ucVal);
};
