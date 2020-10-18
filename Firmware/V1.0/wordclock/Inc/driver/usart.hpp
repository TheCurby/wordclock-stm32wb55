#pragma once

#include <stdint.h>
#include "stm32wb55xx.h"

class USART {
	public:
		USART(USART_TypeDef* oUSART_l, uint32_t u32Baudrate);
		void init8N1(uint32_t u32Baudrate);
		void setBaudrate(uint32_t u32Baudrate);
		void send(const uint8_t u8Val);
		uint8_t get();

	protected:
		USART_TypeDef* oUSART;
};
