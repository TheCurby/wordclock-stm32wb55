#include "wordclock/settings.hpp"

Settings::Settings(M95M01& oM95M01_l) : oM95M01(oM95M01_l) {
	uint32_t u32Key = 0xa71cab6f;
	uint32_t u32Key_l;
	oM95M01.Read(M95M01::ut32MemSize - sizeof(uint32_t), (uint8_t*) &u32Key_l, sizeof(uint32_t));

	if (u32Key_l != u32Key) {
		oData.oLanguage = Language::English;
		oData.bDCF77 = false;
		oData.bNight = false;
		oData.eAnimation = Animation::None;
		oData.oColors.setWhiteOnly(0x20);
		oData.u16BrightnessOffsetMax = 0;
		oData.u16BrightnessOffsetMin = 0;
		oData.u8BrightnessMax = 0xff;
		oData.u8BrightnessMin = 0;

		writeConfig();
		oM95M01.Write(M95M01::ut32MemSize - sizeof(uint32_t), (uint8_t*) &u32Key, sizeof(uint32_t));
	}
	else {
		loadData(oData);
	}

	oData.oLanguage = Language::Deutsch;
}

void Settings::loadData(s_Data& oData_l) {
	oM95M01.Read(0, (uint8_t*) &oData_l, sizeof(s_Data));

	if (oData.bDCF77 != false && oData.bDCF77 != true) {
		oData.bDCF77 = false;
	}

	if (oData.bNight != false && oData.bNight != true) {
		oData.bNight = false;
	}

	if (oData.bRandomColor != false && oData.bRandomColor != true) {
		oData.bRandomColor = false;
	}

	if (oData.eAnimation > Animation::AnimationType::Random) {
		oData.eAnimation = Animation::AnimationType::None;
	}

	if (oData.oLanguage >= Language::COUNT_LANG) {
		oData.oLanguage = Language::English;
	}

	if (oData.oColors.isClear()) {
		oData.oColors.setWhiteOnly(0xff);
	}

	if (oData.u8BrightnessMax < oData.u8BrightnessMin) {
		oData.u8BrightnessMax = oData.u8BrightnessMin;
	}

	if (oData.rtcNightOn.u8Hours > 23) {
		oData.rtcNightOn.u8Hours = 0;
	}
	if (oData.rtcNightOn.u8Minutes > 59) {
		oData.rtcNightOn.u8Hours = 0;
	}

	if (oData.rtcNightOff.u8Hours > 23) {
		oData.rtcNightOff.u8Hours = 0;
	}
	if (oData.rtcNightOff.u8Minutes > 59) {
		oData.rtcNightOff.u8Hours = 0;
	}
}

void Settings::writeChanges() {
	s_Data oData_l;
	bool bChange = false;
	uint16_t u16Start = 0;
	uint8_t* pOrg = (uint8_t*) &oData_l;
	uint8_t* pNew = (uint8_t*) &oData;

	loadData(oData_l);

	for (uint16_t i = 0; i < sizeof(s_Data); i++) {
		if (pOrg[i] != pNew[i] && !bChange) {
			bChange = true;
			u16Start = i;
		}
		else if (pOrg[i] == pNew[i] && bChange) {
			bChange = false;
			oM95M01.Write(u16Start, (uint8_t*) &pNew[u16Start], i - u16Start);
		}
	}

	if (bChange) {
		oM95M01.Write(u16Start, (uint8_t*) &pNew[u16Start], sizeof(s_Data) - u16Start);
	}
}

void Settings::writeConfig() {
	oM95M01.Write(0, (uint8_t*) &oData, sizeof(s_Data));
}

bool Settings::isDcf77() const {
	return oData.bDCF77;
}

void Settings::setDcf77(bool bVal) {
	oData.bDCF77 = bVal;
}

bool Settings::isNight() const {
	return oData.bNight;
}
void Settings::setNight(bool bVal) {
	oData.bNight = bVal;
}

bool Settings::isRandomColor() const {
	return oData.bRandomColor;
}

void Settings::setRandomColor(bool bVal) {
	oData.bRandomColor = bVal;
}

Animation::AnimationType Settings::getAnimation() const {
	return oData.eAnimation;
}

void Settings::setAnimation(Animation::AnimationType eAnimation) {
	oData.eAnimation = eAnimation;
}

Colors Settings::getColors() const {
	return oData.oColors;
}

void Settings::setColors(Colors& oColors) {
	oData.oColors = oColors;
}

uint8_t Settings::getU8Blue() const {
	return oData.oColors.getBlue();
}

void Settings::setU8Blue(uint8_t u8Val) {
	oData.oColors.setBlue(u8Val);
}

uint8_t Settings::getU8Green() const {
	return oData.oColors.getGreen();
}

void Settings::setU8Green(uint8_t u8Val) {
	oData.oColors.setGreen(u8Val);
}

uint8_t Settings::getU8Red() const {
	return oData.oColors.getRed();
}

void Settings::setU8Red(uint8_t u8Val) {
	oData.oColors.setRed(u8Val);
}

uint8_t Settings::getU8White() const {
	return oData.oColors.getWhite();
}

void Settings::setU8White(uint8_t u8Val) {
	oData.oColors.setWhite(u8Val);
}

uint8_t Settings::getU8BrightnessMin() const {
	return oData.u8BrightnessMin;
}

void Settings::setU8BrightnessMin(uint8_t u8Val) {
	oData.u8BrightnessMin = u8Val;
}

uint8_t Settings::getU8BrightnessMax() const {
	return oData.u8BrightnessMax;
}

void Settings::setU8BrightnessMax(uint8_t u8Val) {
	oData.u8BrightnessMax = u8Val;
}

uint16_t Settings::getU16BrightnessOffsetMax() const {
	return oData.u16BrightnessOffsetMax;
}

void Settings::setU16BrightnessOffsetMax(uint16_t u16Val) {
	oData.u16BrightnessOffsetMax = u16Val;
}

uint16_t Settings::getU16BrightnessOffsetMin() const {
	return oData.u16BrightnessOffsetMin;
}

void Settings::setU16BrightnessOffsetMin(uint16_t u16Val) {
	oData.u16BrightnessOffsetMin = u16Val;
}

Language Settings::getLanguage() const {
	return oData.oLanguage;
}

void Settings::setLanguage(Language oLanguage) {
	oData.oLanguage = oLanguage;
}

s_RTC Settings::getNightOn() const {
	return oData.rtcNightOn;
}

void Settings::setNightOn(s_RTC& oRTC) {
	oData.rtcNightOn = oRTC;
}

s_RTC Settings::getNightOff() const {
	return oData.rtcNightOff;
}

void Settings::setNightOff(s_RTC& oRTC) {
	oData.rtcNightOff = oRTC;
}
