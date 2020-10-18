#include "wordclock/Wordclock.hpp"

Hardware::Hardware() {
	/*##### RCC #####*/
	FLASH->ACR = 1;
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));
	RCC->CFGR |= 0b10;
	while ((RCC->CFGR & (0b1000)) != 0b1000);
	RCC->CR &= ~(RCC_CR_HSION);

	PWR->CR1 |= PWR_CR1_DBP;
	PWR->CR1 |= 0b01ul << 9;
	if (!(RCC->BDCR & RCC_BDCR_LSERDY)) {
		RCC->BDCR |= RCC_BDCR_LSEON;
		while (!(RCC->BDCR & RCC_BDCR_LSERDY));
	}
	RCC->BDCR |= 0b01ul << 8;
	RCC->CCIPR |= 0x03ul << 28;
	PWR->CR4 |= PWR_CR4_C2BOOT;

	/* GPIOs */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	STM32::configPinOutput(GPIOA, 0);    //..
	STM32::configPinOutput(GPIOA, 1);    //..
	STM32::configPinAnalog(GPIOA, 2);    //LDR
	STM32::configPinInput(GPIOA, 3);    //DCF77
	STM32::configPinOutput(GPIOA, 4);    //CS EEprom
	STM32::configPinAF(GPIOA, 5, (STM32::Flags) 5);    //SPI CLK
	STM32::configPinAF(GPIOA, 6, (STM32::Flags) 5);    //SPI MISO
	STM32::configPinAF(GPIOA, 7, (STM32::Flags) 5);    //SPI MOSI
	STM32::configPinOutput(GPIOA, 8);    //..
	STM32::configPinOutput(GPIOA, 9);    //..
	STM32::configPinOutput(GPIOA, 10);    //..
	STM32::configPin(GPIOA, 11, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, (STM32::Flags) 1);    //SK6812 Timer
	STM32::configPinOutput(GPIOA, 12);    //..
	//STM32::configPin(GPIOA, 13, STM32::Flags::OUTPUT, STM32::Flags::PUSHPULL, STM32::Flags::NOPULL, 0);
	//STM32::configPin(GPIOA, 14, STM32::Flags::OUTPUT, STM32::Flags::PUSHPULL, STM32::Flags::NOPULL, 0);
	STM32::configPinOutput(GPIOA, 15);    //..
	STM32::configPinOutput(GPIOB, 0);    //..
	STM32::configPinOutput(GPIOB, 1);    //..
	STM32::configPinOutput(GPIOB, 2);    //..
	STM32::configPin(GPIOB, 3, STM32::Flags::INPUT, STM32::Flags::OPENDRAIN, STM32::Flags::PULLUP, (STM32::Flags) 0);    //MTO0
	STM32::configPin(GPIOB, 4, STM32::Flags::INPUT, STM32::Flags::OPENDRAIN, STM32::Flags::PULLUP, (STM32::Flags) 0);    //MTO1
	STM32::configPin(GPIOB, 5, STM32::Flags::INPUT, STM32::Flags::OPENDRAIN, STM32::Flags::PULLUP, (STM32::Flags) 0);    //MTO3
	STM32::configPin(GPIOB, 6, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, (STM32::Flags) 7);    //OneWIre
	STM32::configPin(GPIOB, 7, STM32::Flags::ALTERNATE, STM32::Flags::OPENDRAIN, STM32::Flags::NOPULL, (STM32::Flags) 7);    //OneWIre
	STM32::configPinOutput(GPIOB, 8);    //..
	STM32::configPinOutput(GPIOB, 9);    //..
	STM32::configPinOutput(GPIOE, 4);    //..

	/* RTC */
	RCC->BDCR |= RCC_BDCR_RTCEN;

	/* SPI */
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	/* Timer für Interrupt */
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->ARR = 0xffffffff;
	TIM2->CNT = 0xffffffff;
	TIM2->DIER = TIM_DIER_UIE;
	TIM2->PSC = (CLK_TIMER / 1000000ul) - 1;
	TIM2->CR1 = TIM_CR1_CEN;
	STM32::setInterrupt(TIM2_IRQn, 1);

	/* Timer für SK6812 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMAMUX1EN;
	DMAMUX1_Channel0->CCR = 25;
	DMA1_Channel1->CCR =
	DMA_CCR_MINC | (0b00 << 10) | (0b01 << 8) | DMA_CCR_DIR;
	TIM1->CR1 = 0;
	TIM1->DIER = TIM_DIER_UDE;
	TIM1->CCMR2 = (0b110UL << 12) | TIM_CCMR2_OC4PE;
	TIM1->CCER = TIM_CCER_CC4E;
	TIM1->ARR = (CLK_TIMER / CLK_SK6812) - 1;
	TIM1->CCR4 = 0;
	TIM1->BDTR = TIM_BDTR_MOE;
	TIM1->CR1 = TIM_CR1_CEN;

	/* USART für OneWire */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* LDR */
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	ADC1->CR &= ~ADC_CR_DEEPPWD;
	ADC1->CR |= ADC_CR_ADVREGEN;
	Timer t(10);
	while (!t.Ready());
	ADC1->CR |= ADC_CR_ADCAL;
	while (ADC1->CR & ADC_CR_ADCAL);
	ADC1->SMPR1 = (0b111ul << 21);
	ADC1->SQR1 = 7 << 6;
	ADC1->CR |= ADC_CR_ADEN;
	while (!(ADC1->ISR & ADC_ISR_ADRDY));
	ADC1->CFGR &= ~ADC_CFGR_JQDIS;
	ADC1->CR |= ADC_CR_ADSTART;

#ifndef DEBUG
	/* Watchdog */
	RCC->APB1ENR1 |= RCC_APB1ENR1_WWDGEN;
	WWDG->CFR = 0x7f;
	WWDG->CFR |= 0b111ul << 11;
	WWDG->CR = WWDG_CR_WDGA | 0x7f;
#endif
}
