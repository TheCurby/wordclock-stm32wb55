#pragma once

#include "libs/m95m01.hpp"
#include "util/clock.hpp"
#include "wordclock/Animation.hpp"
#include "wordclock/Colors.hpp"
#include "wordclock/wordclock_typedef.hpp"

class Settings {
	private:
		struct s_Data {
				bool bDCF77;    //DCF77 wird ausgewertet
				bool bNight;
				bool bRandomColor;
				uint8_t u8BrightnessMin;
				uint8_t u8BrightnessMax;
				uint16_t u16BrightnessOffsetMin;
				uint16_t u16BrightnessOffsetMax;
				Colors oColors;
				Animation::AnimationType eAnimation;
				Language oLanguage;
				s_RTC rtcNightOff;
				s_RTC rtcNightOn;
		} oData;
		M95M01& oM95M01;

		void loadData(s_Data& oData_l);

	public:
		Settings(M95M01& oM95M01_l);
		void writeChanges();
		void writeConfig();

		bool isDcf77() const;
		void setDcf77(bool bVal);

		bool isNight() const;
		void setNight(bool bVal);

		bool isRandomColor() const;
		void setRandomColor(bool bVal);

		Animation::AnimationType getAnimation() const;
		void setAnimation(Animation::AnimationType eAnimation);

		Colors getColors() const;
		void setColors(Colors& oColors);

		uint8_t getU8Blue() const;
		void setU8Blue(uint8_t u8Val);

		uint8_t getU8Green() const;
		void setU8Green(uint8_t u8Val);

		uint8_t getU8Red() const;
		void setU8Red(uint8_t u8Val);

		uint8_t getU8White() const;
		void setU8White(uint8_t u8Val);

		uint8_t getU8BrightnessMin() const;
		void setU8BrightnessMin(uint8_t u8Val);

		uint8_t getU8BrightnessMax() const;
		void setU8BrightnessMax(uint8_t u8Val);

		uint16_t getU16BrightnessOffsetMax() const;
		void setU16BrightnessOffsetMax(uint16_t u8Val);

		uint16_t getU16BrightnessOffsetMin() const;
		void setU16BrightnessOffsetMin(uint16_t u8Val);

		Language getLanguage() const;
		void setLanguage(Language oLanguage);

		s_RTC getNightOn() const;
		void setNightOn(s_RTC& oRTC);

		s_RTC getNightOff() const;
		void setNightOff(s_RTC& oRTC);
};
