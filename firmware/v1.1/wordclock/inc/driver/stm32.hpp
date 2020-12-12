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
#pragma once

#include "stm32wb55xx.h"
#include <cstdint>

class STM32 {
	public:
		enum Flags : uint8_t {
			ANALOG = 0x03,
			INPUT = 0x00,
			ALTERNATE = 0x02,
			OUTPUT = 0x01,
			NOPULL = 0x00,
			PULLUP = 0x01,
			PULLDOWN = 0x02,
			LOW = 0x00,
			MEDIUM = 0x01,
			FAST = 0x02,
			HIGH = 0x03,
			PUSHPULL = 0x00,
			OPENDRAIN = 0x01
		};

		static void configPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, Flags u8Mode, Flags u8Drive, Flags u8Pullup, uint8_t AF);
		static void configPinOutput(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinInput(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinAnalog(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinAF(GPIO_TypeDef* oGPIO, uint8_t u8Pin, uint8_t u8AF);
		static void setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, bool bVal);
		static void setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void clrPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static bool getPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void setInterrupt(uint8_t u8Flag, uint8_t u8Priority);
		static uint16_t getADC();
		static uint16_t u16ADC;
};
