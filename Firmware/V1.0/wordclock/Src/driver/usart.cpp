/*
 * uart.c
 *
 *  Created on: 14.11.2019
 *      Author: Nils
 */

#include "driver/usart.hpp"
#include "system/clocks.hpp"

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
