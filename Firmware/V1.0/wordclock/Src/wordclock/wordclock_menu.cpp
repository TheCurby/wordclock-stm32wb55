#include "wordclock/wordclock.hpp"

void Wordclock::setMenu(Menu Goto) {
	oKeyLeft.setScrolling(false);
	oKeyMiddle.setScrolling(false);
	oKeyRight.setScrolling(false);

	u8SubMenu = 0;
	CurrentMenu = Goto;
	tDisplay.Stop();

	switch (Goto) {
		case Menu::Uhr:
			if (!AnimationClock.running()) {
				oContainerOld.clear();
			}
			oRTCOld = RTClock::get();
		break;

		case Menu::Hours:
		case Menu::Minutes:
			tBlink.StartMs(u16BlinkTime * 2);
			oKeyMiddle.setScrolling(true, 500, 500);
			oKeyRight.setScrolling(true, 500, 500);
		break;

		case Menu::Colors:
			oKeyRight.setScrolling(true);
		break;

		case Menu::LightUp:
		case Menu::LightDown:
			oKeyMiddle.setScrolling(true, 500, 20);
			oKeyRight.setScrolling(true, 500, 20);
		break;

		case Menu::Night:
			oKeyRight.setScrolling(true, 500, 200);
		break;

		case Menu::Animation:
			AnimationMenu.stop();
			tAnimation.Stop();
		break;

		default:
		break;
	}
}

void Wordclock::nextMenu() {
	Menu Goto;

	switch (CurrentMenu) {
		case Menu::Uhr:
		case Menu::Temp:
		case Menu::Seconds:
			Goto = Menu::DCF77;
		break;
		case Menu::DCF77:
			Goto = oSettings.isDcf77() ? Menu::Colors : Menu::Hours;
		break;
		case Menu::Hours:
			Goto = Menu::Minutes;
		break;
		case Menu::Minutes:
			Goto = Menu::Colors;
		break;
		case Menu::Colors:
			Goto = Menu::LightUp;
		break;
		case Menu::LightUp:
			Goto = Menu::LightDown;
		break;
		case Menu::LightDown:
			Goto = Menu::Animation;
		break;
		case Menu::Animation:
			Goto = Menu::Night;
		break;
		case Menu::Night:
			//oSettings.writeChanges();
			//Goto = Menu::Uhr;
			Goto = Menu::Lang;
		break;
		case Menu::Lang:
			oSettings.writeChanges();
			Goto = Menu::Uhr;
		break;
		case Menu::Test1:
			Goto = Menu::Test2;
		break;
		default:
			Goto = Menu::Uhr;
		break;
	}

	setMenu(Goto);
	tTimeout.StartMs(u16TimeoutTime);
}

void Wordclock::plusminus(bool bVal) {
	uint8_t u8Temp = 0;

	switch (CurrentMenu) {
		case Menu::Uhr:
			if (bVal) {
				setMenu(Menu::Seconds);
			}
			else if (!night()) {
				bOn = !bOn;
			}
		break;

		case Menu::Seconds:
			if (bVal) {
				setMenu(Menu::Temp);
			}
			else if (!night()) {
				bOn = !bOn;
			}
		break;

		case Menu::Temp:
			if (bVal) {
				setMenu(Menu::Uhr);
			}
			else if (!night()) {
				bOn = !bOn;
			}
		break;

		case Menu::DCF77:
			oSettings.setDcf77(!oSettings.isDcf77());
		break;

		case Menu::Hours: {
			s_RTC tRTC = RTClock::get();

			if (bVal) {
				tRTC.u8Hours++;
				if (tRTC.u8Hours >= 24) {
					tRTC.u8Hours = 0;
				}
			}
			else {
				tRTC.u8Hours--;
				if (tRTC.u8Hours >= 24) {
					tRTC.u8Hours = 23;
				}
			}

			RTClock::set(tRTC);
		}
		break;

		case Menu::Minutes: {
			s_RTC tRTC = RTClock::get();

			if (bVal) {
				tRTC.u8Minutes++;
				if (tRTC.u8Minutes >= 59) {
					tRTC.u8Minutes = 0;

					tRTC.u8Hours++;
					if (tRTC.u8Hours > 23) {
						tRTC.u8Hours = 0;
					}
				}
			}
			else {
				tRTC.u8Minutes--;
				if (tRTC.u8Minutes >= 59) {
					tRTC.u8Minutes = 59;

					tRTC.u8Hours--;
					if (tRTC.u8Hours > 23) {
						tRTC.u8Hours = 23;
					}
				}
			}

			tRTC.u8Seconds = 50;
			RTClock::set(tRTC);
		}
		break;

		case Menu::Colors:
			if (!bVal) {

				switch (u8SubMenu) {
					case 0:
						u8Temp = oSettings.getU8Red();
						if (u8Temp < 0xff) {
							oSettings.setU8Red(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Red(0);
						}
					break;

					case 1:
						u8Temp = oSettings.getU8Green();
						if (u8Temp < 0xff) {
							oSettings.setU8Green(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Green(0);
						}
					break;

					case 2:
						u8Temp = oSettings.getU8Blue();
						if (u8Temp < 0xff) {
							oSettings.setU8Blue(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Blue(0);
						}
					break;

#ifndef USING_WS2812
					case 3:
						u8Temp = oSettings.getU8White();
						if (u8Temp < 0xff) {
							oSettings.setU8White(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8White(0);
						}
					break;
#endif
					default:
					case 4:
						oSettings.setRandomColor(!oSettings.isRandomColor());
					break;
				}
			}
			else {
#ifndef USING_WS2812
				u8SubMenu = (u8SubMenu + 1) % 5;
#else
				u8SubMenu = (u8SubMenu + 1) % 4;
#endif
			}
		break;

		case Menu::LightUp:
			u8Temp = oSettings.getU8BrightnessMax();
			oSettings.setU16BrightnessOffsetMin(u16ADC);

			if (bVal && u8Temp < 0xff) {
				oSettings.setU8BrightnessMax(u8Temp + 1);
			}
			else if (!bVal && u8Temp > 0 && u8Temp - 1 >= oSettings.getU8BrightnessMin()) {
				oSettings.setU8BrightnessMax(u8Temp - 1);
			}
		break;

		case Menu::LightDown:
			u8Temp = oSettings.getU8BrightnessMin();
			oSettings.setU16BrightnessOffsetMax(u16ADC);

			if (bVal && u8Temp < 0xff && u8Temp + 1 <= oSettings.getU8BrightnessMax()) {
				oSettings.setU8BrightnessMin(u8Temp + 1);
			}
			else if (!bVal && u8Temp > 0) {
				oSettings.setU8BrightnessMin(u8Temp - 1);
			}
		break;

		case Menu::Animation: {
			uint8_t eSelectedAnimation = oSettings.getAnimation();

			if (bVal) {
				eSelectedAnimation += (Animation::AnimationType) 1;
				if (eSelectedAnimation > Animation::AnimationType::Random) {
					eSelectedAnimation = 0;
				}
			}
			else {
				eSelectedAnimation -= (Animation::AnimationType) 1;
				if (eSelectedAnimation > Animation::AnimationType::Random) {
					eSelectedAnimation = Animation::AnimationType::Random;
				}
			}

			oSettings.setAnimation((Animation::AnimationType) eSelectedAnimation);
			AnimationMenu.stop();
			tAnimation.Stop();
		}
		break;

		case Menu::Night: {
			if (!bVal) {
				switch (u8SubMenu) {
					default:
					case 0:
						oSettings.setNight(!oSettings.isNight());
					break;

					case 1:
					case 2:
						s_RTC rtcNight;

						if (u8SubMenu == 1) {
							rtcNight = oSettings.getNightOn();
						}
						else {
							rtcNight = oSettings.getNightOff();
						}

						rtcNight.u8Minutes -= rtcNight.u8Minutes % 5;
						rtcNight.u8Minutes += 5;
						if (rtcNight.u8Minutes > 59) {
							rtcNight.u8Minutes = 0;

							if (++rtcNight.u8Hours > 23) {
								rtcNight.u8Hours = 0;
							}
						}

						if (oKeyRightDouble.getLastStatus()) {
							rtcNight.u8Hours = 0;
							rtcNight.u8Minutes = 0;
						}

						if (u8SubMenu == 1) {
							oSettings.setNightOn(rtcNight);
						}
						else {
							oSettings.setNightOff(rtcNight);
						}
					break;
				}
			}
			else {
				u8SubMenu = (u8SubMenu + 1) % 3;
			}
		}
		break;

		case Menu::Lang: {
			uint8_t oLang = oSettings.getLanguage();

			if (!bVal) {
				oLang -= 1;
				if (oLang >= Language::COUNT_LANG) {
					oLang = Language::COUNT_LANG - 1;
				}
			}
			else {
				oLang += 1;
				if (oLang >= Language::COUNT_LANG) {
					oLang = 0;
				}
			}

			oSettings.setLanguage((Language) oLang);
		}
		break;

		case Menu::Test2:
			if (bVal) {
				u8SubMenu = (u8SubMenu + 1) % 4;
			}
			else {
				if (u8SubMenu == 0) {
					u8SubMenu = 3;
				}
				else {
					u8SubMenu--;
				}
			}
		break;

		default:
			return;
		break;
	}

	tDisplay.Stop();
	tTimeout.StartMs(u16TimeoutTime);
}

void Wordclock::getKeys() {
	bool bVal;

	bVal = !STM32::getPin(GPIOB, 5);
	oKeyTest.getLongPress(bVal, 5000);
	oKeyLeft.getSingle(bVal);

	bVal = !STM32::getPin(GPIOB, 4);
	oKeyMiddle.getSingle(bVal);

	bVal = !STM32::getPin(GPIOB, 3);
	oKeyRightDouble.getDouble(bVal);
	oKeyRight.getSingle(bVal);
}
