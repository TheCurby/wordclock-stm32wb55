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
#include "driver/stm32.hpp"

uint16_t STM32::u16ADC = 0;

void STM32::configPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, Flags u8Mode, Flags u8Drive, Flags u8Pullup, uint8_t AF) {
	oGPIO->MODER &= ~(0b11ul << (2 * u8Pin));    //Reset Mode
	oGPIO->MODER |= u8Mode << (2 * u8Pin);    //Set Mode
	oGPIO->OTYPER &= ~(1 << u8Pin);    //Clr Opendrain
	oGPIO->OTYPER |= u8Drive << u8Pin;    //set opendrain
	oGPIO->PUPDR &= ~(0b11 << (2 * u8Pin));    //Clr pullup
	oGPIO->PUPDR |= u8Pullup << (2 * u8Pin);    //set pullup
	oGPIO->AFR[u8Pin / 8] &= ~(0b1111 << (4 * (u8Pin % 8)));    // Clr afunc
	oGPIO->AFR[u8Pin / 8] |= AF << (4 * (u8Pin % 8));
}

void STM32::configPinOutput(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	configPin(oGPIO, u8Pin, OUTPUT, PUSHPULL, NOPULL, (Flags) 0);
}

void STM32::configPinInput(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	configPin(oGPIO, u8Pin, INPUT, PUSHPULL, NOPULL, (Flags) 0);
}

void STM32::configPinAnalog(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	configPin(oGPIO, u8Pin, ANALOG, PUSHPULL, NOPULL, (Flags) 0);
}

void STM32::configPinAF(GPIO_TypeDef* oGPIO, uint8_t u8Pin, uint8_t u8AF) {
	configPin(oGPIO, u8Pin, ALTERNATE, PUSHPULL, NOPULL, u8AF);
}

void STM32::setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, bool bVal) {
	if (bVal) {
		oGPIO->BSRR = 1 << u8Pin;
	}
	else {
		oGPIO->BSRR = 1 << (16 + u8Pin);
	}
}

void STM32::setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	oGPIO->BSRR = 1 << u8Pin;
}

void STM32::clrPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	oGPIO->BSRR = 1 << (16 + u8Pin);
}

bool STM32::getPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
	return oGPIO->IDR & (1 << u8Pin) ? true : false;
}

void STM32::setInterrupt(uint8_t u8Flag, uint8_t u8Priority) {
	NVIC->ICPR[u8Flag / 32] |= 1 << (u8Flag % 32);
	NVIC->ISER[u8Flag / 32] |= 1 << (u8Flag % 32);
	NVIC->IP[u8Flag / 4] |= (u8Priority << 4) << (8 * (u8Flag % 4));
}

uint16_t STM32::getADC() {
	if (ADC1->ISR & ADC_ISR_EOS) {
		ADC1->ISR |= ADC_ISR_EOS;

		u16ADC = ADC1->DR;
		ADC1->CR |= ADC_CR_ADSTART;
	}

	return u16ADC;
}
