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
#include "driver/hardware.hpp"

Hardware::Hardware() {
	/*##### RCC #####*/
	RCC->HSECR = 0xCAFECAFE;
	RCC->HSECR = ((0x18) << 8) | 0x30;
	FLASH->ACR = 1;
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR |= 0b10;
	while ((RCC->CFGR & (0b1000)) != 0b1000);
	RCC->CR &= ~(RCC_CR_HSION);
	//RCC->CFGR |= (0b1000 << 24);
	//RCC->CFGR |= (0b100 << 28); // div 16

	PWR->CR1 |= PWR_CR1_DBP;
	PWR->CR1 |= 0b01ul << 9;
	if (!(RCC->BDCR & RCC_BDCR_LSERDY)) {
		RCC->BDCR |= RCC_BDCR_LSEON;
		while (!(RCC->BDCR & RCC_BDCR_LSERDY));
	}
	RCC->BDCR |= 0b01ul << 8;
	RCC->CCIPR |= 0x03ul << 28;
	PWR->CR4 |= PWR_CR4_C2BOOT;

	/*setting clocks*/
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	RCC->BDCR |= RCC_BDCR_RTCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

	/* GPIOs */
	STM32::configPinAnalog(GPIOA, 0);    //LDR
	STM32::configPinAF(GPIOA, 1, 5);    //SPI CLK
	STM32::configPinOutput(GPIOA, 2);    //..
	STM32::configPinOutput(GPIOA, 3);    //..
	STM32::configPinOutput(GPIOA, 4);    //..
	STM32::configPinOutput(GPIOA, 5);    //CS EEprom
	STM32::configPinAF(GPIOA, 6, 5);    //SPI MISO
	STM32::configPinAF(GPIOA, 7, 5);    //SPI MOSI
	STM32::configPinInput(GPIOA, 8);    //DCF77
	STM32::configPinOutput(GPIOA, 9);    //..
	STM32::configPinOutput(GPIOA, 10);    //..
	STM32::configPin(GPIOA, 11, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, 1);    //SK6812 Timer
	STM32::configPinOutput(GPIOA, 12);    //..
	//STM32::configPin(GPIOA, 13, STM32::Flags::OUTPUT, STM32::Flags::PUSHPULL, STM32::Flags::NOPULL, 0);
	//STM32::configPin(GPIOA, 14, STM32::Flags::OUTPUT, STM32::Flags::PUSHPULL, STM32::Flags::NOPULL, 0);
	STM32::configPinOutput(GPIOA, 15);    //..
	STM32::configPinOutput(GPIOB, 0);    //..
	STM32::configPinOutput(GPIOB, 1);    //..
	STM32::configPinOutput(GPIOB, 2);    //..
	STM32::configPinInput(GPIOB, 3);    //MTO0
	STM32::configPinInput(GPIOB, 4);    //MTO1
	STM32::configPinInput(GPIOB, 5);    //MTO3
	STM32::configPin(GPIOB, 6, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, 7);    //OneWIre
	STM32::configPin(GPIOB, 7, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, 7);    //OneWIre
	STM32::configPinOutput(GPIOB, 8);    //..
	STM32::configPin(GPIOB, 9, STM32::Flags::OUTPUT, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, 0);    //Power LED
	STM32::configPinOutput(GPIOE, 4);    //..

	/*RTC*/
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	//RTC->CR |= RTC_CR_COE;
	//RTC->OR |= (1 << 1);
	RTC->CALR = (uint32_t) (512 - 19) | RTC_CALR_CALP;

	/* Timer für Interrupt */
	TIM2->ARR = 0xffffffff;
	TIM2->CNT = 0xffffffff;
	TIM2->DIER = TIM_DIER_UIE;
	TIM2->PSC = (CLK_TIMER / 1000000ul) - 1;
	TIM2->CR1 = TIM_CR1_CEN;
	STM32::setInterrupt(TIM2_IRQn, 1);
	Timer::setReference((uint32_t*) &TIM2->CNT);

	/* Timer für SK6812 */
	DMAMUX1_Channel0->CCR = 25 << DMAMUX_CxCR_DMAREQ_ID_Pos;
	DMA1_Channel1->CCR = DMA_CCR_MINC | (0b00 << 10) | (0b01 << 8) | DMA_CCR_DIR;
	TIM1->CR1 = 0;
	TIM1->DIER = TIM_DIER_UDE;
	TIM1->CCMR2 = (0b110UL << 12) | TIM_CCMR2_OC4PE;
	TIM1->CCER = TIM_CCER_CC4E;
	TIM1->ARR = (CLK_TIMER / CLK_SK6812) - 1;
	TIM1->CCR4 = 0;
	TIM1->BDTR = TIM_BDTR_MOE;
	TIM1->CR1 = TIM_CR1_CEN;

	/* LDR */
	ADC1->CR &= ~ADC_CR_DEEPPWD;
	ADC1->CR |= ADC_CR_ADVREGEN;
	Timer t(10);
	while (!t.Ready());
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL);
	ADC1->SMPR1 = (0b111ul << 15);
	ADC1->SQR1 = 5 << 6;
	ADC1->CR |= ADC_CR_ADEN;
	while (!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CFGR &= ~ADC_CFGR_JQDIS;
	ADC1->CR |= ADC_CR_ADSTART;

#ifndef DEBUG
	/* Watchdog */
	RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
	WWDG->CFR = 0x7f;
	WWDG->CFR |= 0b111ul << 11;
	WWDG->CR = WWDG_CR_WDGA | 0x7e;
#endif
}
