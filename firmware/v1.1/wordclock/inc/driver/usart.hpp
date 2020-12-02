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
