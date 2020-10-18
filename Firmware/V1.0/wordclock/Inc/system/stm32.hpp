#pragma once

#include "stm32wb55xx.h"

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

		static void configPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, Flags u8Mode, Flags u8Drive, Flags u8Pullup, Flags AF);
		static void configPinOutput(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinInput(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinAnalog(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void configPinAF(GPIO_TypeDef* oGPIO, uint8_t u8Pin, uint8_t u8AF);
		static void setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, bool bVal);
		static void setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void clrPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static bool getPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin);
		static void setInterrupt(uint8_t u8Flag, uint8_t u8Priority);
};
