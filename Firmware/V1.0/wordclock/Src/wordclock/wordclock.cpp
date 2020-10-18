#include "wordclock/wordclock.hpp"

Wordclock::Wordclock() : Hardware(), oSPI(SPI1, 1), oEEPROM(oSPI, GPIOA, 4), oSettings(oEEPROM), oUSART(
USART1, 9600), oOneWire(oUSART), oDS18X20(oOneWire), oModbus(1, 9600) {
	bOn = true;
	u8SubMenu = 0;
	memset(&oData.u8Void, 0, 70);
	u16ADC = 0;

	tTimeout.StartMs(u16TimeoutTime);
	setMenu(Menu::Uhr);

#ifdef DS18B20
	oDS18X20.setResolution(12);
#endif

	s_RTC oRTCTemp = RTClock::get();
	srand(oRTCTemp.u8Hours * 3600 + oRTCTemp.u8Minutes * 60 + oRTCTemp.u8Seconds);
}

void Wordclock::loop() {
	WWDG->CR |= 0x7f;

	/* LDR */
	if (ADC1->ISR & ADC_ISR_EOS) {
		ADC1->ISR |= ADC_ISR_EOS;

		u16ADC = ADC1->DR;
		ADC1->CR |= ADC_CR_ADSTART;
	}

	bool bPin = STM32::getPin(GPIOA, 3);
	if (oDCF77.receive(bPin) && oSettings.isDcf77()) {
		RTClock::set(oDCF77.getRTC());
	}
	STM32::setPin(GPIOB, 8, bPin);

	if (tTemp.Ready() && (!AnimationMenu.running() || CurrentMenu == Menu::Temp)) {
		oDS18X20.readTemp();
		tTemp.StartMs(500);
	}

	redraw();

	getKeys();
	if (oKeyTest.getLastStatus()) {
		setMenu(Menu::Test1);
	}

	if (oKeyLeft.getLastStatus()) {
		nextMenu();
	}

	if (oKeyMiddle.getLastStatus()) {
		plusminus(true);
	}

	if (oKeyRight.getLastStatus()) {
		plusminus(false);
	}

	if (tTimeout.Ready() && CurrentMenu >= Menu::DCF77) {
		oSettings.writeChanges();
		setMenu(Menu::Uhr);
	}

	executeModbus();
}

void Wordclock::executeModbus() {
	s_RTC oRTCTemp;

	oModbus.setInput(0, oSettings.isDcf77());
	oModbus.setInput(1, oSettings.isNight());
	oModbus.setInput(2, bOn);
	oModbus.setRegister(0, oSettings.getU8Red());
	oModbus.setRegister(1, oSettings.getU8Green());
	oModbus.setRegister(2, oSettings.getU8Blue());
	oModbus.setRegister(3, oSettings.getU8White());
	oModbus.setRegister(4, oSettings.getAnimation());
	oModbus.setRegister(5, (uint16_t) oSettings.getLanguage());
	oModbus.setRegister(6, oSettings.getU8BrightnessMax());
	oModbus.setRegister(7, oSettings.getU8BrightnessMin());
	oRTCTemp = RTClock::get();
	oModbus.setRegister(10, oRTCTemp.u8Hours);
	oModbus.setRegister(11, oRTCTemp.u8Minutes);
	oModbus.setRegister(12, oRTCTemp.u8Seconds);
	oModbus.setRegister(13, oRTCTemp.u8Day);
	oModbus.setRegister(14, oRTCTemp.u8Month);
	oModbus.setRegister(15, oRTCTemp.u8Year);
	oRTCTemp = oSettings.getNightOn();
	oModbus.setRegister(16, oRTCTemp.u8Hours);
	oModbus.setRegister(17, oRTCTemp.u8Minutes);
	oRTCTemp = oSettings.getNightOff();
	oModbus.setRegister(18, oRTCTemp.u8Hours);
	oModbus.setRegister(19, oRTCTemp.u8Minutes);

	if (oModbus.getCoil(0)) {    //Set Color
		oModbus.setCoil(0, false);

		Colors oColors((uint8_t) oModbus.getHolding(0), (uint8_t) oModbus.getHolding(1), (uint8_t) oModbus.getHolding(2), (uint8_t) oModbus.getHolding(3));
		oSettings.setColors(oColors);
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(1)) {    //Set clock
		oModbus.setCoil(1, false);

		oRTCTemp.u8Hours = oModbus.getHolding(10);
		oRTCTemp.u8Minutes = oModbus.getHolding(11);
		oRTCTemp.u8Seconds = oModbus.getHolding(12);
		oRTCTemp.u8Day = oModbus.getHolding(13);
		oRTCTemp.u8Month = oModbus.getHolding(14);
		oRTCTemp.u8Year = oModbus.getHolding(15);

		RTClock::set(oRTCTemp);
	}

	else if (oModbus.getCoil(2)) {    //Set RTC NightOn
		oModbus.setCoil(2, false);

		oRTCTemp.u8Hours = oModbus.getHolding(10);
		oRTCTemp.u8Minutes = oModbus.getHolding(11);
		oRTCTemp.u8Seconds = oModbus.getHolding(12);
		oRTCTemp.u8Day = oModbus.getHolding(13);
		oRTCTemp.u8Month = oModbus.getHolding(14);
		oRTCTemp.u8Year = oModbus.getHolding(15);

		oSettings.setNightOn(oRTCTemp);
		oSettings.writeChanges();
	}

	else if (oModbus.getCoil(3)) {    //Set RTC NightOn
		oModbus.setCoil(3, false);

		oRTCTemp.u8Hours = oModbus.getHolding(10);
		oRTCTemp.u8Minutes = oModbus.getHolding(11);
		oRTCTemp.u8Seconds = oModbus.getHolding(12);
		oRTCTemp.u8Day = oModbus.getHolding(13);
		oRTCTemp.u8Month = oModbus.getHolding(14);
		oRTCTemp.u8Year = oModbus.getHolding(15);

		oSettings.setNightOff(oRTCTemp);
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(4)) {    //Set dcf77
		oModbus.setCoil(4, false);
		oSettings.setDcf77(!oSettings.isDcf77());
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(5)) {    //Set animation
		oModbus.setCoil(5, false);
		oSettings.setAnimation((Animation::AnimationType) oModbus.getHolding(4));
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(6)) {    //Wordclock on/off
		oModbus.setCoil(6, false);
		bOn = !bOn;
		tDisplay.Stop();
	}
	else if (oModbus.getCoil(7)) {    //Wordclock on/off
		oModbus.setCoil(7, false);
		oSettings.setLanguage((Language) oModbus.getHolding(5));
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(8)) {
		oModbus.setCoil(8, false);
		oSettings.setNight(!oSettings.isNight());
		oSettings.writeChanges();
	}
	else if (oModbus.getCoil(16)) {    //show temp
		oModbus.setCoil(16, false);

		setMenu(Menu::Temp);
	}
	else if (oModbus.getCoil(17)) {    //show clock
		oModbus.setCoil(17, false);

		setMenu(Menu::Uhr);
	}
	else if (oModbus.getCoil(18)) {    //show test
		oModbus.setCoil(18, false);
		setMenu(Menu::Test1);
	}
}

void Wordclock::transmit(bool bVal) {
	while (!(DMA1->ISR & DMA_ISR_TCIF1) && (DMA1_Channel1->CCR & DMA_CCR_EN));
	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	TIM1->CR1 &= ~TIM_CR1_CEN;
	TIM1->CNT = 0;
	DMA1->IFCR = DMA_IFCR_CTEIF1;
	DMA1->IFCR = DMA_IFCR_CHTIF1;
	DMA1->IFCR = DMA_IFCR_CTCIF1;
	DMA1->IFCR = DMA_IFCR_CGIF1;
	DMA1_Channel1->CPAR = (uint32_t) &TIM1->CCR4;
	DMA1_Channel1->CMAR = (uint32_t) &oData;
	DMA1_Channel1->CNDTR = sizeof(oData);

	if (bVal) {
		DMA1_Channel1->CCR |= DMA_CCR_EN;
		TIM1->CR1 = TIM_CR1_CEN;
	}

	//while (!((DMA1->ISR & DMA_ISR_TCIF1) && (DMA1_Channel1->CCR & DMA_CCR_EN)));
}

void Wordclock::setLED(uint8_t u8N, Colors& oColors) {
	if (u8N >= LEDS) {
		return;
	}

	oData.LEDs[Wordclock::au8Mapping[u8N]].set(oColors);
}

void Wordclock::setLED(int16_t u8X, int16_t u8Y, Colors& oColors) {
	if ((u8X < 0 || u8Y < 0 || u8X >= WIDTH || u8Y >= HEIGHT) && !(u8Y == HEIGHT && u8X >= 3 && u8X <= 6)) {
		return;
	}

	if (u8Y == HEIGHT) {
		u8X -= 3;
	}

	setLED(u8Y * WIDTH + u8X, oColors);
}

bool Wordclock::night() {
	if (!oSettings.isNight() || !tTimeout.Ready()) {
		return false;
	}

	s_RTC NightOn = oSettings.getNightOn();
	s_RTC NightOff = oSettings.getNightOff();
	s_RTC oRTC = RTClock::get();

	uint16_t u16NightOn = NightOn.u8Hours * 60 + NightOn.u8Minutes;
	uint16_t u16NightOff = NightOff.u8Hours * 60 + NightOff.u8Minutes;
	uint16_t u16Time = oRTC.u8Hours * 60 + oRTC.u8Minutes;

	if (u16NightOn > u16NightOff) {
		if (u16Time >= u16NightOn || u16Time <= u16NightOff) {
			return true;
		}
	}
	else {
		if (u16Time >= u16NightOn && u16Time <= u16NightOff) {
			return true;
		}
	}

	return false;
}
