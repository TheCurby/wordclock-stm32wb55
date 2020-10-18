#include "wordclock/wordclock.hpp"

void Wordclock::redraw() {
	static uint8_t u8Seconds = 0;
	s_RTC oRTCTemp = RTClock::get();

	if (oDS18X20.change() && CurrentMenu == Menu::Temp) {
		tDisplay.Stop();
	}
	else if (u8Seconds == 59 && oRTCTemp.u8Seconds == 0 && CurrentMenu == Menu::Uhr) {
		tDisplay.Stop();
	}
	else if (u8Seconds != oRTCTemp.u8Seconds && CurrentMenu == Menu::Seconds) {
		tDisplay.Stop();
	}
	u8Seconds = oRTCTemp.u8Seconds;

	if (tDisplay.Ready()) {
		switch (CurrentMenu) {
			case Menu::Hours:
			case Menu::Minutes:
				tDisplay.StartMs(u16BlinkTime / 4);
			break;

			case Menu::Test1:
				tDisplay.StartMs(u16TestTime);
			break;

			case Menu::Animation:
				tDisplay.StartMs(u32AnimationTime);
			break;

			default:
				tDisplay.StartMs(u16RedrawTime);
			break;
		}

		drawScreen(CurrentMenu);
	}
}

void Wordclock::clrScreen() {
	Colors oColors;
	oColors.clear();

	for (uint16_t i = 0; i < LEDS; i++) {
		setLED(i, oColors);
	}
}

void Wordclock::drawImage(const Image& oZ, uint8_t u8OffsetX, uint8_t u8OffsetY, Colors& oColors_l, bool bRandomColor) {
	Colors oColors = oColors_l;

	for (uint8_t i = 0; i < oZ.u8Size; i++) {
		if (bRandomColor) {
			oColors = oColors_l;
			oColors.setRandom();
		}
		setLED(oZ.u8Points[i].u8X + u8OffsetX, oZ.u8Points[i].u8Y + u8OffsetY, oColors);
	}
}

void Wordclock::drawFrame(Colors& oColors_l, bool bRandomColor) {
	Colors oColors;

	for (uint8_t i = 0; i < WIDTH; i++) {
		oColors = oColors_l;
		if (bRandomColor) {
			oColors.setRandom();
		}
		setLED(i, 0, oColors);
	}
	for (uint8_t i = 0; i < WIDTH; i++) {
		oColors = oColors_l;
		if (bRandomColor) {
			oColors.setRandom();
		}
		setLED(i, HEIGHT - 1, oColors);
	}
	for (uint8_t i = 0; i < (HEIGHT - 2); i++) {
		oColors = oColors_l;
		if (bRandomColor) {
			oColors.setRandom();
		}
		setLED(WIDTH - 1, 1 + i, oColors);
	}
	for (uint8_t i = 0; i < (HEIGHT - 2); i++) {
		oColors = oColors_l;
		if (bRandomColor) {
			oColors.setRandom();
		}
		setLED(0, 1 + i, oColors);
	}
}

void Wordclock::drawCenter(Colors& oColors) {
	for (uint8_t i = 0; i < 4; i++) {
		setLED(3, 3 + i, oColors);
		setLED(4, 3 + i, oColors);
		setLED(5, 3 + i, oColors);
		setLED(6, 3 + i, oColors);
		setLED(7, 3 + i, oColors);
	}
}

void Wordclock::drawContainer(Container& oContainer_l, uint16_t u16Brightness) {
	for (uint8_t i = 0; i < oContainer_l.size(); i++) {
		Container::s_Point oPoint = oContainer_l.get(i);

		if (u16Brightness < 0xff) {
			oPoint.oColors.dimm(u16Brightness);
		}

		setLED(oPoint.u8X, oPoint.u8Y, oPoint.oColors);
	}
}

Colors Wordclock::getColors() {
	Colors oColors = oSettings.getColors();

	uint16_t Brightness;
	uint8_t Max = oSettings.getU8BrightnessMax();
	uint8_t Min = oSettings.getU8BrightnessMin();
	uint16_t OffsetMax = oSettings.getU16BrightnessOffsetMax();
	uint16_t OffsetMin = oSettings.getU16BrightnessOffsetMin();
	uint16_t ADC = u16ADC;

	if (ADC < OffsetMin) {
		ADC = OffsetMin;
	}
	else if (ADC > OffsetMax) {
		ADC = OffsetMax;
	}

	OffsetMax -= OffsetMin;
	ADC -= OffsetMin;
	ADC = OffsetMax - ADC;

	if (OffsetMax == 0) {
		oColors = oSettings.getColors();
		return oColors;
	}

	Brightness = Min + (uint16_t) (Max - Min) * ADC / OffsetMax;

	oColors.dimm(Brightness);

	if (oColors.isClear()) {
		//oColors.setWhiteOnly(0xff);
	}

	return oColors;
}

void Wordclock::drawScreen(Menu MenuScreen) {
	Colors oColors = oSettings.getColors();

	clrScreen();
	if (MenuScreen == Menu::Uhr || MenuScreen == Menu::Temp || MenuScreen == Menu::Seconds) {
		if (night() || !bOn) {
			oColors.setRedOnly(0x10);
			setLED(110, oColors);
			transmit(true);
			return;
		}
	}

	oContainerTmp.clear();

	switch (MenuScreen) {
		default:
		case Menu::Uhr: {
			s_RTC oRTCTemp = RTClock::get();
			oColors = getColors();
			Animation::AnimationType eSelectedAnimation = oSettings.getAnimation();

			drawText(oRTCTemp.u8Minutes, oColors, oSettings.isRandomColor());
			drawHours(oRTCTemp, oColors, oSettings.isRandomColor());
			drawMinutes(oRTCTemp, oColors, oSettings.isRandomColor());

			if (oRTCTemp.u8Minutes / 5 != oRTCOld.u8Minutes / 5) {
				if (eSelectedAnimation != Animation::AnimationType::None && !AnimationClock.running()) {
					if (eSelectedAnimation == Animation::Random) {
						eSelectedAnimation = (Animation::AnimationType) (std::rand() % Animation::AnimationType::Random);
					}

					AnimationClock = Animation(oContainerOld, oContainerTmp, eSelectedAnimation);
				}
				oContainerOld = oContainerTmp;
			}

			if (!AnimationClock.running() || eSelectedAnimation == Animation::AnimationType::None) {
				oContainerOld.merge(oContainerTmp);
				drawContainer(oContainerOld);
			}
			else {
				tDisplay.StartMs(AnimationClock.run());
				drawContainer(AnimationClock.get(), AnimationClock.getDimmValue());
			}

			oRTCOld = oRTCTemp;
		}
		break;

		case Menu::Seconds: {
			s_RTC oRTCTemp = RTClock::get();
			oColors = getColors();
			Colors oColors_l = oColors;

			drawImage(*aoZiffern[oRTCTemp.u8Seconds / 10], 0, 1, oColors, oSettings.isRandomColor());
			drawImage(*aoZiffern[oRTCTemp.u8Seconds % 10], 6, 1, oColors, oSettings.isRandomColor());

			for (uint8_t i = 0; i < oRTCTemp.u8Minutes % 5; i++) {
				if (oSettings.isRandomColor()) {
					oColors_l = oColors;
					oColors_l.setRandom();
				}
				setLED(WIDTH * HEIGHT + i, oColors_l);
			}
		}
		break;

		case Menu::Temp: {
			int16_t s16Temp = oDS18X20.getTemp();
			oColors = getColors();
			Colors oColors_l = oColors;

			drawImage(*aoZiffern[s16Temp / 10], 0, 1, oColors, oSettings.isRandomColor());
			drawImage(*aoZiffern[s16Temp % 10], 6, 1, oColors, oSettings.isRandomColor());

			s16Temp = oDS18X20.getTempRaw() & 0x0f;
			s16Temp *= 5;
			s16Temp /= 16;
			for (uint8_t i = 0; i < s16Temp; i++) {
				if (oSettings.isRandomColor()) {
					oColors_l = oColors;
					oColors_l.setRandom();
				}
				setLED(WIDTH * HEIGHT + i, oColors_l);
			}

			setLED(105, oColors);    //°C
		}
		break;

		case Menu::Animation: {
			s_RTC oRTCDraw = RTClock::get();

			drawText(oRTCDraw.u8Minutes, oColors);
			drawAMPM(oRTCDraw.u8Hours, oColors);
			drawHours(oRTCDraw, oColors);
			drawMinutes(oRTCDraw, oColors);

			if (oSettings.getAnimation() != Animation::AnimationType::None && !AnimationMenu.running() && tAnimation.Ready()) {
				AnimationMenu = Animation(oContainerTmp, oContainerTmp, oSettings.getAnimation());
				tAnimation.StartMs(u32AnimationTime);
			}

			if (oSettings.getAnimation() == Animation::AnimationType::Random) {
				oColors.setWhiteOnly(0x80);
				drawImage(Fragezeichen, 2, 0, oColors);
			}
			else if (oSettings.getAnimation() == Animation::AnimationType::None) {
				oColors.setWhiteOnly(0x80);
				drawImage(error, 0, 1, oColors);
			}
			else if (AnimationMenu.running()) {
				tDisplay.StartMs(AnimationMenu.run());
				drawContainer(AnimationMenu.get(), AnimationMenu.getDimmValue());
			}
			else {
				drawContainer(oContainerTmp);
			}
		}
		break;

		case Menu::DCF77:
			if (oSettings.isDcf77()) {
				oColors.setWhiteOnly(0x80);
				drawImage(ok, 0, 1, oColors);
				oColors.setGreenOnly(0x80);
				drawFrame(oColors);
			}
			else {
				oColors.setWhiteOnly(0x80);
				drawImage(error, 0, 1, oColors);
				oColors.setRedOnly(0x80);
				drawFrame(oColors);
			}
		break;

		case Menu::Hours: {
			s_RTC oRTCTemp = RTClock::get();
			drawText(oRTCTemp.u8Minutes, oColors);
			drawAMPM(oRTCTemp.u8Hours, oColors);
			drawMinutes(oRTCTemp, oColors);

			if (tBlink.RestMs() > u16BlinkTime) {
				drawHours(oRTCTemp, oColors);
			}
			if (tBlink.Ready()) {
				tBlink.StartMs(u16BlinkTime * 2);
			}

			drawContainer(oContainerTmp);
		}
		break;

		case Menu::Minutes: {
			s_RTC oRTCTemp = RTClock::get();
			drawHours(oRTCTemp, oColors);
			drawAMPM(oRTCTemp.u8Hours, oColors);
			drawText(oRTCTemp.u8Minutes, oColors);

			if (tBlink.RestMs() > u16BlinkTime) {
				drawMinutes(oRTCTemp, oColors);
			}
			if (tBlink.Ready()) {
				tBlink.StartMs(u16BlinkTime * 2);
			}

			drawContainer(oContainerTmp);
		}
		break;

		case Menu::Colors: {

			switch (u8SubMenu) {
				case 0:
					drawCenter(oColors);
					oColors.setRedOnly(oColors.getRed());
					drawFrame(oColors);
					oColors.setRedOnly(0xff);
					setLED(5, 2, oColors);
				break;

				case 1:
					drawCenter(oColors);
					oColors.setGreenOnly(oColors.getGreen());
					drawFrame(oColors);
					oColors.setGreenOnly(0xff);
					setLED(5, 2, oColors);
				break;

				case 2:
					drawCenter(oColors);
					oColors.setBlueOnly(oColors.getBlue());
					drawFrame(oColors);
					oColors.setBlueOnly(0xff);
					setLED(5, 2, oColors);
				break;

#ifndef USING_WS2812
				case 3:drawCenter(oColors);
					oColors.setWhiteOnly(oColors.getWhite());
					drawFrame(oColors);
					oColors.setWhiteOnly(0xff);
					setLED(5, 2, oColors);
				break;
#endif
				default:
				case 4:
					drawFrame(oColors, true);
					if (oSettings.isRandomColor()) {
						oColors.setWhiteOnly(0x80);
						drawImage(ok, 0, 1, oColors);
					}
					else {
						oColors.setWhiteOnly(0x80);
						drawImage(error, 0, 1, oColors);
					}
				break;
			}

		}
		break;

		case Menu::LightDown: {
			uint8_t u8Brightness = oSettings.getU8BrightnessMin();
			if (u8Brightness == 0) u8Brightness = 1;
			oColors.setWhiteOnly(u8Brightness);

			drawImage(Down, 2, 0, oColors);

			uint8_t u8Limit = (uint16_t) WIDTH * u8Brightness / 0xff;
			if (u8Limit == 0) u8Limit = 1;
			for (uint8_t i = 0; i < u8Limit; i++) {
				setLED(i, HEIGHT - 1, oColors);
			}
		}
		break;

		case Menu::LightUp: {
			uint8_t u8Brightness = oSettings.getU8BrightnessMax();
			if (u8Brightness == 0) u8Brightness = 1;
			oColors.setWhiteOnly(u8Brightness);

			drawImage(Up, 2, 0, oColors);

			uint8_t u8Limit = (uint16_t) WIDTH * u8Brightness / 0xff;
			if (u8Limit == 0) u8Limit = 1;
			for (uint8_t i = 0; i < u8Limit; i++) {
				setLED(i, HEIGHT - 1, oColors);
			}
		}
		break;

		case Menu::Night:
			switch (u8SubMenu) {
				default:
				case 0:
					if (oSettings.isNight()) {
						oColors.setWhiteOnly(0x80);
						drawImage(N, 0, 0, oColors);
						oColors.setGreenOnly(0x80);
						drawFrame(oColors);
					}
					else {
						oColors.setWhiteOnly(0x80);
						drawImage(N, 0, 0, oColors);
						oColors.setRedOnly(0x80);
						drawFrame(oColors);
					}
				break;

				case 1:
				case 2: {
					s_RTC oRTCTemp;
					if (u8SubMenu == 1) {
						oRTCTemp = oSettings.getNightOn();
						oColors.setGreenOnly(0x80);
					}
					else {
						oRTCTemp = oSettings.getNightOff();
						oColors.setRedOnly(0x80);
					}
					drawHours(oRTCTemp, oColors);
					drawMinutes(oRTCTemp, oColors);
					drawAMPM(oRTCTemp.u8Hours, oColors);
					drawText(oRTCTemp.u8Minutes, oColors, false);
					drawContainer(oContainerTmp);
				}
				break;
			}

		break;

		case Menu::Lang:
			switch (oSettings.getLanguage()) {
				default:
				case Language::English:
					drawImage(*aoBuchstaben['E' - 'A'], 0, 1, oColors);
					drawImage(*aoBuchstaben['N' - 'A'], 6, 1, oColors);
				break;

				case Language::Deutsch:
					drawImage(*aoBuchstaben['D' - 'A'], 0, 1, oColors);
					drawImage(*aoBuchstaben['E' - 'A'], 6, 1, oColors);
				break;

				case Language::Nederlands:
					drawImage(*aoBuchstaben['N' - 'A'], 0, 1, oColors);
					drawImage(*aoBuchstaben['L' - 'A'], 6, 1, oColors);
				break;

				case Language::Francais:
					drawImage(*aoBuchstaben['F' - 'A'], 0, 1, oColors);
					drawImage(*aoBuchstaben['R' - 'A'], 6, 1, oColors);
				break;
			}

			drawContainer(oContainerTmp);
		break;

		case Menu::Test1:
			oColors.setWhiteOnly(0xff);
			setLED(u8SubMenu, oColors);

			oColors.setRedOnly(0xff);
			setLED(u8SubMenu - 1, oColors);

			oColors.setGreenOnly(0xff);
			setLED(u8SubMenu - 2, oColors);

			oColors.setBlueOnly(0xff);
			setLED(u8SubMenu - 3, oColors);

			if (++u8SubMenu >= LEDS + 3) {
				u8SubMenu = 0;
			}
		break;

		case Menu::Test2:
			switch (u8SubMenu) {
				default:
				case 0:
					oColors.setRedOnly(0xff);
				break;
				case 1:
					oColors.setGreenOnly(0xff);
				break;
				case 2:
					oColors.setBlueOnly(0xff);
				break;
				case 3:
					oColors.setWhiteOnly(0xff);
				break;
			}

			for (uint8_t i = 0; i < LEDS; i++) {
				setLED(i, oColors);
			}
		break;
	}

	transmit(true);
}
