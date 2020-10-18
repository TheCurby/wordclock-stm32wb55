#pragma once

#include "stm32wb55xx.h"

class SPI {
	public:
		SPI(SPI_TypeDef* oSPI_l, uint8_t ucPrescaler);
		void init(uint8_t ucPrescaler);
		void clearRXFIFO();
		uint8_t send(const uint8_t cVal);
		void sendDMA(const uint8_t cVal);

	private:
		SPI_TypeDef* oSPI;
};
