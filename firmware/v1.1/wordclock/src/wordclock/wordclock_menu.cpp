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

void Wordclock::setMenu(Menu Goto) {
	oKeyLeft.setScrolling(false);
	oKeyMiddle.setScrolling(false);
	oKeyRight.setScrolling(false);

	bDir = false;
	u8SubMenu = 0;
	CurrentMenu = Goto;
	tDisplay.Stop();

	switch (Goto) {
		case Menu::Clock:
			AnimationClock.stop();
			oContainerOld.clear();
			oRTCOld = RTClock::get();
		break;

		case Menu::Seconds:
		case Menu::Temp:
			oContainerDigit[0].clear();
			oContainerDigit[1].clear();
			oContainerOld.clear();
			s16TempOld = 0;
			oRTCOld = RTClock::get();
		break;

		case Menu::Hours:
		case Menu::Minutes:
			tBlink.StartMs(u16BlinkTime * 2);
			oKeyMiddle.setScrolling(true, 500, 500);
			oKeyRight.setScrolling(true, 500, 500);
		break;

		case Menu::Colors:
			u8SubMenu = 1;
			oKeyRight.setScrolling(true);
		break;

		case Menu::Light:
			oKeyRight.setScrolling(true, 500, 20);
		break;

		case Menu::Night:
			oKeyRight.setScrolling(true, 500, 120);
		break;

		case Menu::Animation:
			AnimationMenu.stop();
			tAnimation.Stop();
		break;

		default:
		break;
	}
}

const Menu Wordclock::MenuMapping[] = {
	Menu::Clock,
#ifdef ENABLE_DCF77
	Menu::DCF77,
#endif
	Menu::Hours,
	Menu::Minutes,
	Menu::Colors,
#ifdef ENABLE_LDR
	Menu::Light,
#endif
#ifdef ENABLE_ANIMATIONS
	Menu::Animation,
#endif
#ifdef ENABLE_NIGHTMODE
	Menu::Night,
#endif
#ifdef ENABLE_LANGUAGE
	Menu::Lang,
#endif
};

void Wordclock::nextMenu() {
	Menu Goto = MenuMapping[1];
	constexpr uint8_t u8Length = sizeof(MenuMapping) / sizeof(Menu);

	if(CurrentMenu == Menu::Test1){
		Goto = Menu::Test2;
	}
	else if(CurrentMenu == Menu::Test2){
		Goto = Menu::Clock;
	}
	else{
		for (uint8_t i = 0; i < u8Length; i++) {
			if (MenuMapping[i] == CurrentMenu) {
				if (++i >= u8Length) {
					i = 0;
					oSettings.writeChanges();
				}
				Goto = MenuMapping[i];

				break;
			}
		}
	}

	setMenu(Goto);
	tTimeout.StartMs(u16TimeoutTime);
}

void Wordclock::plusminus(bool bVal) {
	uint8_t u8Temp = 0;

	switch (CurrentMenu) {
		case Menu::Clock:
			if (bVal) {
#ifdef ENABLE_SECONDS
				setMenu(Menu::Seconds);
#elif defined(ENABLE_TEMP)
				setMenu(Menu::Temp);
#else
				bOn = !bOn;
#endif
			}
			else if (!night()) {
				bOn = !bOn;
			}
		break;

		case Menu::Seconds:
			if (bVal) {
#ifdef ENABLE_TEMP
				setMenu(Menu::Temp);
#else
				setMenu(Menu::Clock);
#endif
			}
			else if (!night()) {
				bOn = !bOn;
			}
		break;

		case Menu::Temp:
			if (bVal) {
				setMenu(Menu::Clock);
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
				if (tRTC.u8Minutes > 59) {
					tRTC.u8Minutes = 0;

					tRTC.u8Hours++;
					if (tRTC.u8Hours > 23) {
						tRTC.u8Hours = 0;
					}
				}
			}
			else {
				tRTC.u8Minutes--;
				if (tRTC.u8Minutes > 59) {
					tRTC.u8Minutes = 59;

					tRTC.u8Hours--;
					if (tRTC.u8Hours > 23) {
						tRTC.u8Hours = 23;
					}
				}
			}

			tRTC.u8Seconds = 0;
			RTClock::set(tRTC);
		}
		break;

		case Menu::Colors:
			oKeyRight.setScrolling(u8SubMenu != 0 ? true : false);

			if (!bVal) {
				switch (u8SubMenu) {
					default:
					case 0:{
						uint8_t eMode = (uint8_t) oSettings.getMode();
						eMode += 1;

						if (eMode >= DisplayMode::COUNT_MODE) {
							eMode = DisplayMode::Normal;
						}

						oSettings.setMode((DisplayMode) eMode);
					}
					break;

					case 1:
						u8Temp = oSettings.getU8Red();
						if (u8Temp < 0xff) {
							oSettings.setU8Red(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Red(0);
						}
					break;

					case 2:
						u8Temp = oSettings.getU8Green();
						if (u8Temp < 0xff) {
							oSettings.setU8Green(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Green(0);
						}
					break;

					case 3:
						u8Temp = oSettings.getU8Blue();
						if (u8Temp < 0xff) {
							oSettings.setU8Blue(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8Blue(0);
						}
					break;

#ifndef USING_WS2812
					case 4:
						u8Temp = oSettings.getU8White();
						if (u8Temp < 0xff) {
							oSettings.setU8White(u8Temp + 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8White(0);
						}
					break;
#endif
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

		case Menu::Light:
			if (!bVal) {
				switch (u8SubMenu) {
					default:
					case 0:
						u8Temp = oSettings.getU8BrightnessMax();
						oSettings.setU16BrightnessOffsetMin(u16ADC);

						if (u8Temp > 0) {
							oSettings.setU8BrightnessMax(u8Temp - 1);
						}

						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8BrightnessMax(0xff);
						}
					break;

					case 1:
						u8Temp = oSettings.getU8BrightnessMin();
						oSettings.setU16BrightnessOffsetMax(u16ADC);

						if (u8Temp > 0) {
							oSettings.setU8BrightnessMin(u8Temp - 1);
						}
						if (oKeyRightDouble.getLastStatus()) {
							oSettings.setU8BrightnessMin(0xff);
						}
					break;
				}
			}
			else {
				u8SubMenu = (u8SubMenu + 1) % 2;
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
					case 2:
						oSettings.setNight(!oSettings.isNight());
					break;

					case 0:
					case 1:
						s_RTC rtcNight;

						if (oKeyRightDouble.getLastStatus()) {
							bDir = bDir ? false : true;
						}

						if (u8SubMenu == 0) {
							rtcNight = oSettings.getNightOn();
						}
						else {
							rtcNight = oSettings.getNightOff();
						}

						rtcNight.u8Minutes -= rtcNight.u8Minutes % 5;
						if(!bDir){
							rtcNight.u8Minutes += 5;
							if (rtcNight.u8Minutes > 59) {
								rtcNight.u8Minutes = 0;

								if (++rtcNight.u8Hours > 23) {
									rtcNight.u8Hours = 0;
								}
							}
						}
						else{
							rtcNight.u8Minutes -= 5;
							if (rtcNight.u8Minutes > 59) {
								rtcNight.u8Minutes = 55;

								if (--rtcNight.u8Hours > 23) {
									rtcNight.u8Hours = 23;
								}
							}
						}

						if (u8SubMenu == 0) {
							oSettings.setNightOn(rtcNight);
						}
						else {
							oSettings.setNightOff(rtcNight);
						}
					break;
				}
			}
			else {
				bDir = false;
				u8SubMenu = (u8SubMenu + 1) % 3;

				if(u8SubMenu == 2){
					oKeyRight.setScrolling(false, 500, 120);
				}
				else{
					oKeyRight.setScrolling(true, 500, 120);
				}
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

	bVal = STM32::getPin(GPIOB, 5) ? false : true;
	oKeyLeft.getSingle(bVal);
	oKeyTest.getLongPress(oKeyLeft.getValue(), 5000);

	bVal = STM32::getPin(GPIOB, 4) ? false : true;
	oKeyMiddle.getSingle(bVal);

	bVal = STM32::getPin(GPIOB, 3) ? false : true;
	oKeyRight.getSingle(bVal);
	oKeyRightDouble.getDouble(oKeyRight.getValue());
}
