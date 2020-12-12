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
#include "wordclock/wordclock.hpp"

Wordclock::Wordclock() : oHardware(), oSPI(SPI1, 1), oEEPROM(oSPI, GPIOA, 5), oSettings(oEEPROM), oUSART(
USART1, 9600), oOneWire(oUSART), oDS18X20(oOneWire) {
	bOn = true;
	bNewData = false;
	s16TempOld = 0;
	u8SubMenu = 0;
	memset(&oData.u8Void, 0, 70);
	u16ADC = 0;

	tTimeout.StartMs(u16TimeoutTime);
	setMenu(Menu::Clock);
	STM32::setPin(GPIOA, 15);
	STM32::clrPin(GPIOB, 9);

#ifdef USING_DS18B20
	oDS18X20.setResolution(12);
#endif
	oDS18X20.readTemp();

	s_RTC oRTCTemp = RTClock::get();
	srand(oRTCTemp.u8Hours * 3600 + oRTCTemp.u8Minutes * 60 + oRTCTemp.u8Seconds);
}

Wordclock::~Wordclock() {
}

bool Wordclock::loop() {
#ifndef _SIMULATOR_
	WWDG->CR = WWDG_CR_WDGA | 0x7e;
#endif

	/* LDR */
	u16ADC = STM32::getADC();

#ifdef ENABLE_DCF77
	if (oDCF77.receive(STM32::getPin(GPIOA, 8)) && oSettings.isDcf77()) {
		RTClock::set(oDCF77.getRTC());
		if (CurrentMenu == Menu::Clock) {
			tDisplay.Stop();
		}
	}
#endif

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

	if (tTimeout.Ready() && (uint8_t)CurrentMenu < (uint8_t)Menu::MENU_END) {
		oSettings.writeChanges();
		setMenu(Menu::Clock);
	}

	bool bTmp = bNewData;
	bNewData = false;
	return bTmp;
}

void Wordclock::transmit(bool bVal) {
#ifndef _SIMULATOR_
	Timer tTimeout_l(500);
	while (!(DMA1->ISR & DMA_ISR_TCIF1) && (DMA1_Channel1->CCR & DMA_CCR_EN)){
		if(tTimeout_l.Ready()){
			return;
		}
	}
	oDataSend = oData;

	DMA1_Channel1->CCR &= ~DMA_CCR_EN;
	TIM1->CR1 &= ~TIM_CR1_CEN;
	TIM1->CNT = 0;
	DMA1->IFCR = DMA_IFCR_CTEIF1;
	DMA1->IFCR = DMA_IFCR_CHTIF1;
	DMA1->IFCR = DMA_IFCR_CTCIF1;
	DMA1->IFCR = DMA_IFCR_CGIF1;
	DMA1_Channel1->CPAR = (uint32_t) &TIM1->CCR4;
	DMA1_Channel1->CMAR = (uint32_t) &oDataSend;
	DMA1_Channel1->CNDTR = sizeof(Data);

	if (bVal) {
		DMA1_Channel1->CCR |= DMA_CCR_EN;
		TIM1->CR1 |= TIM_CR1_CEN;
	}
#endif

	bNewData = true;
}

void Wordclock::setLED(uint16_t u8N, Colors& oColors) {
	if (u8N >= LEDS) {
		return;
	}

#ifndef _SIMULATOR_
	oData.LEDs[Wordclock::au8Mapping[u8N]].set(oColors);
#else
	oData.LEDs[u8N].set(oColors);
#endif
}

void Wordclock::setLED(int16_t u8X, int16_t u8Y, Colors& oColors) {
	if (u8X == -1 && u8Y == -1) {
		setLED(WIDTH * HEIGHT + 0, oColors);
	}
	else if (u8X == WIDTH && u8Y == -1) {
		setLED(WIDTH * HEIGHT + 1, oColors);
	}
	else if (u8X == WIDTH && u8Y == HEIGHT) {
		setLED(WIDTH * HEIGHT + 2, oColors);
	}
	else if (u8X == -1 && u8Y == HEIGHT) {
		setLED(WIDTH * HEIGHT + 3, oColors);
	}
	else {
		if (u8X < 0 || u8Y < 0 || u8X >= WIDTH || u8Y >= HEIGHT) {
			return;
		}

		setLED(u8Y * WIDTH + u8X, oColors);
	}
}

bool Wordclock::night() {
#ifndef ENABLE_NIGHTMODE
	return false;
#endif

	if (!oSettings.isNight() || !tTimeout.Ready()) {
		return false;
	}

	uint16_t u16NightOn = RTClock::toTimeStamp(oSettings.getNightOn());
	uint16_t u16NightOff = RTClock::toTimeStamp(oSettings.getNightOff());
	uint16_t u16Time = RTClock::toTimeStamp(RTClock::get());

	if (u16NightOn > u16NightOff) {
		if (u16Time >= u16NightOn || u16Time < u16NightOff) {
			return true;
		}
	}
	else {
		if (u16Time >= u16NightOn && u16Time < u16NightOff) {
			return true;
		}
	}

	return false;
}
