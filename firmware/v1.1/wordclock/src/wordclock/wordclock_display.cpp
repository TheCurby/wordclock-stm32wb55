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

void Wordclock::redraw() {
	static uint8_t u8Seconds = 0;
	s_RTC oRTCTemp = RTClock::get();

	if (u8Seconds == 59 && oRTCTemp.u8Seconds == 0 && CurrentMenu == Menu::Clock) {
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

	for (uint16_t i = 0; i < LEDS; i++) {
		setLED(i, oColors);
	}
}

void Wordclock::drawFrame(Colors& oColors) {
	for (uint8_t i = 0; i < WIDTH; i++) {
		setLED(i, 0, oColors);
	}
	for (uint8_t i = 0; i < WIDTH; i++) {
		setLED(i, HEIGHT - 1, oColors);
	}
	for (uint8_t i = 0; i < (HEIGHT - 2); i++) {
		setLED(WIDTH - 1, 1 + i, oColors);
	}
	for (uint8_t i = 0; i < (HEIGHT - 2); i++) {
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

uint8_t Wordclock::getDimmValue() {
#ifndef ENABLE_LDR
	return 0xff;
#endif

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
		return 0xff;
	}

	Brightness = Min + (uint16_t) (Max - Min) * ADC / OffsetMax;

	return Brightness;
}

void Wordclock::drawScreen(Menu MenuScreen) {
	Colors oColors = oSettings.getColors();

	clrScreen();
	if (MenuScreen == Menu::Clock || MenuScreen == Menu::Temp || MenuScreen == Menu::Seconds) {
		if (night()) {
#if !defined(ENABLE_NIGHT_POWEROFF) || defined(_SIMULATOR_)
			STM32::clrPin(GPIOB, 9);
			oColors.setRedOnly(0x80);
			setLED(110, oColors);
			transmit(true);
#else
			STM32::setPin(GPIOB, 9);
#endif
			return;
		}
		if (!bOn) {
			STM32::clrPin(GPIOB, 9);
			oColors.setRedOnly(0x80);
			setLED(110, oColors);
			transmit(true);
			return;
		}
	}
	STM32::clrPin(GPIOB, 9);

	oContainerTmp.clear();

	switch (MenuScreen) {
		default:
		case Menu::Clock: {
			s_RTC oRTCTemp = RTClock::get();
			Animation::AnimationType eSelectedAnimation = oSettings.getAnimation();

			drawText(oContainerTmp, oRTCTemp, oColors, oSettings.getMode());
			drawHours(oContainerTmp, oRTCTemp, oColors, oSettings.getMode());
			drawMinutes(oContainerTmp, oRTCTemp, oColors, oSettings.getMode());

			if (oRTCTemp.u8Minutes / 5 != oRTCOld.u8Minutes / 5 || abs(RTClock::toTimeStamp(oRTCTemp) - RTClock::toTimeStamp(oRTCOld)) > 5) {
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
				drawContainer(oContainerOld, getDimmValue());
			}
			else {
				tDisplay.StartMs(AnimationClock.run());
				drawContainer(AnimationClock.get(), (uint16_t) AnimationClock.getDimmValue() * getDimmValue() / 0xff);
			}

			oRTCOld = oRTCTemp;
		}
		break;

		case Menu::Seconds: {
			s_RTC oRTCTemp = RTClock::get();
			uint8_t u8DimmValue = getDimmValue();

			if (oRTCOld.u8Seconds / 10 != oRTCTemp.u8Seconds / 10 || oContainerDigit[0].empty()) {
				oContainerDigit[0].clear();
				oContainerDigit[0].drawImage(*aoZiffern[oRTCTemp.u8Seconds / 10], 0, 1, oColors, oSettings.getMode());
			}
			if (oRTCOld.u8Seconds % 10 != oRTCTemp.u8Seconds % 10 || oContainerDigit[1].empty()) {
				oContainerDigit[1].clear();
				oContainerDigit[1].drawImage(*aoZiffern[oRTCTemp.u8Seconds % 10], 6, 1, oColors, oSettings.getMode());
			}
			if (oRTCOld.u8Minutes % 5 != oRTCTemp.u8Minutes % 5 || oContainerOld.empty()) {
				oContainerOld.clear();
				drawDots(oContainerOld, oRTCTemp.u8Minutes % 5, oColors, oSettings.getMode());
			}

			drawContainer(oContainerDigit[0], u8DimmValue);
			drawContainer(oContainerDigit[1], u8DimmValue);
			drawContainer(oContainerOld, u8DimmValue);

			oRTCOld = oRTCTemp;
		}
		break;

		case Menu::Temp: {
			while (!oDS18X20.readTemp());

			uint8_t u8DimmValue = getDimmValue();
			int16_t s16Temp = oDS18X20.getTempRaw();

#ifdef ENABLE_FAHRENHEIT
			s16Temp *= 9;
			s16Temp /= 5;
			s16Temp += (32 << 4);
#endif

			if ((s16Temp >> 4) / 10 != (s16TempOld >> 4) / 10 || oContainerDigit[0].empty()) {
				oContainerDigit[0].clear();
				oContainerDigit[0].drawImage(*aoZiffern[(s16Temp >> 4) / 10], 0, 1, oColors, oSettings.getMode());

#ifdef ENABLE_TEMP_SYMBOL
				uint8_t u8Pos = 0;
				switch (oSettings.getLanguage()) {
					default:
					case Language::English:
						u8Pos = 3;
					break;
					case Language::German:
					case Language::Dutch:
						u8Pos = 6;
					break;
					case Language::Italian:
						u8Pos = 5;
					break;
				}
				oContainerDigit[0].setWord(u8Pos, HEIGHT - 1, 1, oColors, oSettings.getMode());
#endif
			}
			if ((s16Temp >> 4) % 10 != (s16TempOld >> 4) % 10 || oContainerDigit[1].empty()) {
				oContainerDigit[1].clear();
				oContainerDigit[1].drawImage(*aoZiffern[(s16Temp >> 4) % 10], 6, 1, oColors, oSettings.getMode());
			}

			int16_t s16Temp1 = (s16Temp & 0x0f) * 5 / 16;
			int16_t s16Temp2 = (s16TempOld & 0x0f) * 5 / 16;
			if (s16Temp1 != s16Temp2 || oContainerOld.empty()) {
				oContainerOld.clear();
				drawDots(oContainerOld, s16Temp1, oColors, oSettings.getMode());
			}

			drawContainer(oContainerDigit[0], u8DimmValue);
			drawContainer(oContainerDigit[1], u8DimmValue);
			drawContainer(oContainerOld, u8DimmValue);

			s16TempOld = s16Temp;
		}
		break;

		case Menu::Animation: {
			s_RTC oRTCDraw = RTClock::get();

			drawText(oContainerTmp, oRTCDraw, oColors);
			drawAMPM(oContainerTmp, oRTCDraw, oColors);
			drawHours(oContainerTmp, oRTCDraw, oColors);
			drawMinutes(oContainerTmp, oRTCDraw, oColors);

			if (oSettings.getAnimation() != Animation::AnimationType::None && !AnimationMenu.running() && tAnimation.Ready()) {
				AnimationMenu = Animation(oContainerTmp, oContainerTmp, oSettings.getAnimation());
				tAnimation.StartMs(u32AnimationTime);
			}

			if (oSettings.getAnimation() == Animation::AnimationType::Random) {
				oColors.setWhiteOnly(0x80);
				oContainerTmp.clear();
				oContainerTmp.drawImage(question, 2, 0, oColors);
				drawContainer(oContainerTmp);
			}
			else if (oSettings.getAnimation() == Animation::AnimationType::None) {
				oColors.setWhiteOnly(0x80);
				oContainerTmp.clear();
				oContainerTmp.drawImage(error, 0, 1, oColors);
				drawContainer(oContainerTmp);
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
			oColors.setWhiteOnly(0x80);
			oContainerTmp.drawImage(antenna, 0, 0, oColors);
			drawContainer(oContainerTmp);

			if (oSettings.isDcf77()) {
				oColors.setGreenOnly(0x80);
			}
			else {
				oColors.setRedOnly(0x80);
			}
			drawFrame(oColors);
		break;

		case Menu::Hours: {
			s_RTC oRTCTemp = RTClock::get();
			drawText(oContainerTmp, oRTCTemp, oColors);
			drawAMPM(oContainerTmp, oRTCTemp, oColors);
			drawMinutes(oContainerTmp, oRTCTemp, oColors);

			if (tBlink.RestMs() > u16BlinkTime) {
				drawHours(oContainerTmp, oRTCTemp, oColors);
			}
			if (tBlink.Ready()) {
				tBlink.StartMs(u16BlinkTime * 2);
			}

			drawContainer(oContainerTmp);
		}
		break;

		case Menu::Minutes: {
			s_RTC oRTCTemp = RTClock::get();
			drawHours(oContainerTmp, oRTCTemp, oColors);
			drawAMPM(oContainerTmp, oRTCTemp, oColors);
			drawText(oContainerTmp, oRTCTemp, oColors);

			if (tBlink.RestMs() > u16BlinkTime) {
				drawMinutes(oContainerTmp, oRTCTemp, oColors);
			}
			if (tBlink.Ready()) {
				tBlink.StartMs(u16BlinkTime * 2);
			}

			drawContainer(oContainerTmp);
		}
		break;

		case Menu::Colors: {
			uint16_t u16Tmp;

			switch (u8SubMenu) {
				default:
				case 0: {
					s_RTC oRTCDraw = RTClock::get();

					drawText(oContainerTmp, oRTCDraw, oColors, oSettings.getMode());
					drawAMPM(oContainerTmp, oRTCDraw, oColors, oSettings.getMode());
					drawHours(oContainerTmp, oRTCDraw, oColors, oSettings.getMode());
					drawMinutes(oContainerTmp, oRTCDraw, oColors, oSettings.getMode(), false);

					drawContainer(oContainerTmp);
				}
				break;

				case 1:
					drawCenter(oColors);
					u16Tmp = oColors.getRed();
					oColors.setRedOnly(u16Tmp);
					drawFrame(oColors);
					oColors.setRedOnly(0xff);
					setLED(5, 2, oColors);
					oColors.setWhiteOnly(0xff);
					for (uint8_t i = 0; i < u16Tmp * 9 / 0xff; i++) {
						setLED(1 + i, 8, oColors);
					}
				break;

				case 2:
					drawCenter(oColors);
					u16Tmp = oColors.getGreen();
					oColors.setGreenOnly(u16Tmp);
					drawFrame(oColors);
					oColors.setGreenOnly(0xff);
					setLED(5, 2, oColors);
					oColors.setWhiteOnly(0xff);
					for (uint8_t i = 0; i < u16Tmp * 9 / 0xff; i++) {
						setLED(1 + i, 8, oColors);
					}
				break;

				case 3:
					drawCenter(oColors);
					u16Tmp = oColors.getBlue();
					oColors.setBlueOnly(u16Tmp);
					drawFrame(oColors);
					oColors.setBlueOnly(0xff);
					setLED(5, 2, oColors);
					oColors.setWhiteOnly(0xff);
					for (uint8_t i = 0; i < u16Tmp * 9 / 0xff; i++) {
						setLED(1 + i, 8, oColors);
					}
				break;

#ifndef USING_WS2812
				case 4:
					drawCenter(oColors);
					u16Tmp = oColors.getWhite();
					oColors.setWhiteOnly(u16Tmp);
					drawFrame(oColors);
					oColors.setWhiteOnly(0xff);
					setLED(5, 2, oColors);
					oColors.setWhiteOnly(0xff);
					for (uint8_t i = 0; i < u16Tmp * 9 / 0xff; i++) {
						setLED(1 + i, 8, oColors);
					}
				break;
#endif
			}

			oColors.setWhiteOnly(0x80);
#ifndef USING_WS2812
			uint8_t u8Length = (u8SubMenu + 4) % 5;
#else
			uint8_t u8Length = (u8SubMenu + 3) % 4;
#endif
			if (u8Length < 4) {
				setLED(WIDTH * HEIGHT + u8Length, oColors);
			}
			else {
				setLED(WIDTH * HEIGHT, oColors);
				setLED(WIDTH * HEIGHT + 3, oColors);
			}
		}
		break;

		case Menu::Light: {
			uint8_t u8Brightness;
			uint8_t u8Limit;
			const Image* pImage;

			switch (u8SubMenu) {
				default:
				case 0:
					u8Brightness = oSettings.getU8BrightnessMax();
					pImage = &Up;
				break;

				case 1:
					u8Brightness = oSettings.getU8BrightnessMin();
					pImage = &Down;
				break;
			}

			u8Limit = (uint16_t) WIDTH * u8Brightness / 0xff;

			if (u8Brightness == 0) {
				oColors.setWhiteOnly(0x80);
				oContainerTmp.drawImage(error, 0, 1, oColors);
			}
			else {
				oColors.setWhiteOnly(u8Brightness);
				oContainerTmp.drawImage(*pImage, 2, 0, oColors);
				oContainerTmp.setWord(0, HEIGHT - 1, u8Limit, oColors, DisplayMode::Normal);
			}
			drawContainer(oContainerTmp);

			oColors.setWhiteOnly(0x80);
			setLED(WIDTH * HEIGHT + u8SubMenu, oColors);
		}
		break;

		case Menu::Night:
			oColors.setWhiteOnly(0x80);
			setLED(WIDTH * HEIGHT + u8SubMenu, oColors);

			switch (u8SubMenu) {
				default:
				case 2:
					oColors.setWhiteOnly(0x80);

					if (oSettings.isNight()) {
						oContainerTmp.drawImage(ok, 0, 1, oColors);
						oColors.setGreenOnly(0x80);
					}
					else {
						oContainerTmp.drawImage(error, 0, 1, oColors);
						oColors.setRedOnly(0x80);
					}
					drawFrame(oColors);

					drawContainer(oContainerTmp);
				break;

				case 0:
				case 1: {
					s_RTC oRTCTemp;
					if (u8SubMenu == 0) {
						oRTCTemp = oSettings.getNightOn();
						oColors.setGreenOnly(0x80);
					}
					else {
						oRTCTemp = oSettings.getNightOff();
						oColors.setRedOnly(0x80);
					}
					drawHours(oContainerTmp, oRTCTemp, oColors);
					drawMinutes(oContainerTmp, oRTCTemp, oColors, DisplayMode::Normal, false);
					drawAMPM(oContainerTmp, oRTCTemp, oColors);
					drawText(oContainerTmp, oRTCTemp, oColors, DisplayMode::Normal, false);
					drawContainer(oContainerTmp);
				}
				break;
			}

		break;

		case Menu::Lang:
			switch (oSettings.getLanguage()) {
				default:
				case Language::English:
					oContainerTmp.drawImage(*aoBuchstaben['E' - 'A'], 0, 1, oColors);
					oContainerTmp.drawImage(*aoBuchstaben['N' - 'A'], 6, 1, oColors);
				break;

				case Language::German:
					oContainerTmp.drawImage(*aoBuchstaben['D' - 'A'], 0, 1, oColors);
					oContainerTmp.drawImage(*aoBuchstaben['E' - 'A'], 6, 1, oColors);
				break;

				case Language::Dutch:
					oContainerTmp.drawImage(*aoBuchstaben['N' - 'A'], 0, 1, oColors);
					oContainerTmp.drawImage(*aoBuchstaben['L' - 'A'], 6, 1, oColors);
				break;

				case Language::Francais:
					oContainerTmp.drawImage(*aoBuchstaben['F' - 'A'], 0, 1, oColors);
					oContainerTmp.drawImage(*aoBuchstaben['R' - 'A'], 6, 1, oColors);
				break;

				case Language::Italian:
					oContainerTmp.drawImage(*aoBuchstaben['I' - 'A'], 0, 1, oColors);
					oContainerTmp.drawImage(*aoBuchstaben['T' - 'A'], 6, 1, oColors);
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

			for (uint16_t i = 0; i < LEDS; i++) {
				setLED(i, oColors);
			}
		break;
	}

	transmit(true);
}

void Wordclock::drawDots(Container& oContainer, uint8_t u8Size, Colors& oColors, DisplayMode eMode) {
	if (u8Size > 0) {
		oContainer.setWord(-1, -1, 1, oColors, eMode);
	}
	if (u8Size > 1) {
		oContainer.setWord(WIDTH, -1, 1, oColors, eMode);
	}
	if (u8Size > 2) {
		oContainer.setWord(WIDTH, HEIGHT, 1, oColors, eMode);
	}
	if (u8Size > 3) {
		oContainer.setWord(-1, HEIGHT, 1, oColors, eMode);
	}
}
