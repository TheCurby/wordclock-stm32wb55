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
#include "driver/usart.hpp"
#include "driver/clocks.hpp"

USART::USART(USART_TypeDef* oUSART_l, uint32_t u32Baudrate) {
	oUSART = oUSART_l;
	init8N1(u32Baudrate);
}

void USART::init8N1(uint32_t u32Baudrate) {
	oUSART->CR1 = USART_CR1_TE | USART_CR1_RE;
	oUSART->CR1 |= USART_CR1_UE;

	setBaudrate(u32Baudrate);
}

void USART::setBaudrate(uint32_t u32Baudrate) {
	oUSART->CR1 &= ~USART_CR1_UE;
	oUSART->BRR = CLK_USART / u32Baudrate;
	oUSART->CR1 |= USART_CR1_UE;
}

void USART::send(const uint8_t u8Val) {
	while (!(oUSART->ISR & USART_ISR_TXE));
	oUSART->TDR = u8Val;
}

uint8_t USART::get() {
	while (!(oUSART->ISR & USART_ISR_RXNE));
	return oUSART->RDR;
}
