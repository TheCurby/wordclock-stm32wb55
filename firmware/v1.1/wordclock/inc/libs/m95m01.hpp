////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
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

#include <cstdint>
#include "driver/spi.hpp"
#include "driver/stm32.hpp"
#include "util/timer.hpp"

class M95M01 {
	public:
		static constexpr uint16_t u16Timeout = 100;
		static constexpr uint32_t ut32MemSize = 131072;
		static constexpr uint16_t u16PageSize = 256;

		M95M01(SPI& oSPI_l, GPIO_TypeDef* oGPIO_l, uint16_t u16Pin_l);
		void Read(uint32_t usAddr, uint8_t* pucDest, uint16_t usSize);
		void Write(uint32_t usAddr, uint8_t* pucSrc, uint16_t usSize);

	protected:
		void setCS(bool bVal);
		void Wren(bool bVal);
		bool Busy();
		bool WaitForEEPROM();

		SPI& oSPI;
		GPIO_TypeDef* oGPIO;
		uint16_t u16Pin;
};
