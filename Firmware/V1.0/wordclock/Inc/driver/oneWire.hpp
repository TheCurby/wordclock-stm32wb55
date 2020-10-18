#pragma once

#include "system/stm32.hpp"
#include "driver/usart.hpp"

class OneWire {
	public:
		typedef struct {
				uint8_t u8Family;
				uint8_t au8Code[6];
				uint8_t u8CRC;
		} s_ROMCode;

		OneWire(USART& oUSART_l);
		bool reset();
		void setBool(const bool bVal);
		bool getBool();
		void setByte(const uint8_t u8Val);
		uint8_t getByte();

		bool matchROM(s_ROMCode& oROMCode);
		bool skipROM();
		bool readROM(s_ROMCode& oROMCode);
		uint8_t calcCRC(uint8_t* u8Data, uint8_t u8Length);

	protected:
		USART& oUSART;
};
