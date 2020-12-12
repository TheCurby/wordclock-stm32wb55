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
#include "driver/spi.hpp"

SPI::SPI(SPI_TypeDef* oSPI_l, uint8_t ucPrescaler) {
	oSPI = oSPI_l;
	init(ucPrescaler);
}

void SPI::init(uint8_t ucPrescaler) {
	//oSPI->CR1 |= SPI_CR1_SSM;
	oSPI->CR1 = SPI_CR1_MSTR;
	oSPI->CR1 |= ((uint16_t) ucPrescaler << 3);
	oSPI->CR2 |= (0b0111ul << 8);
	oSPI->CR2 |= SPI_CR2_SSOE;
	oSPI->CR2 |= SPI_CR2_FRXTH;
	//oSPI->CR1 |= SPI_CR1_CPHA
	oSPI->CR1 |= SPI_CR1_SPE;
}

void SPI::clearRXFIFO() {
	volatile uint8_t u8Result;

	for (uint8_t i = 0; i < 4; i++) {
		u8Result = oSPI->DR;
	}

	//u8Result = u8Result;
}

uint8_t SPI::send(const uint8_t cVal) {
	uint8_t u8Result;

	*(volatile uint8_t*) &oSPI->DR = cVal;
	while (!(oSPI->SR & SPI_SR_RXNE));
	u8Result = oSPI->DR;

	return u8Result;
}

void SPI::sendDMA(const uint8_t cVal) {
	*(volatile uint8_t*) &oSPI->DR = cVal;
}
