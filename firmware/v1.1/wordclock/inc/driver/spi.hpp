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
