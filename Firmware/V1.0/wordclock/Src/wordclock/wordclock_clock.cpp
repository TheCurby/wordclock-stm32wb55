#include "wordclock/wordclock.hpp"

void Wordclock::drawHours(const s_RTC& oRTC, Colors& oColors, bool bRandomColor) {
	uint8_t u8Hours = oRTC.u8Hours;

	u8Hours %= 12;

	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 12;
			}

			switch (u8Hours) {
				default:
				case 0:
					oContainerTmp.setWord(5, 8, 6, oColors, bRandomColor);
				break;
				case 1:
					oContainerTmp.setWord(0, 5, 3, oColors, bRandomColor);
				break;
				case 2:
					oContainerTmp.setWord(8, 6, 3, oColors, bRandomColor);
				break;
				case 3:
					oContainerTmp.setWord(6, 5, 5, oColors, bRandomColor);
				break;
				case 4:
					oContainerTmp.setWord(0, 6, 4, oColors, bRandomColor);
				break;
				case 5:
					oContainerTmp.setWord(4, 6, 4, oColors, bRandomColor);
				break;
				case 6:
					oContainerTmp.setWord(3, 5, 3, oColors, bRandomColor);
				break;
				case 7:
					oContainerTmp.setWord(0, 8, 5, oColors, bRandomColor);
				break;
				case 8:
					oContainerTmp.setWord(0, 7, 5, oColors, bRandomColor);
				break;
				case 9:
					oContainerTmp.setWord(7, 4, 4, oColors, bRandomColor);
				break;
				case 10:
					oContainerTmp.setWord(0, 10, 3, oColors, bRandomColor);
				break;
				case 11:
					oContainerTmp.setWord(6, 7, 5, oColors, bRandomColor);
				break;
			}
		break;

		case Language::Deutsch:
			if (oRTC.u8Minutes >= 25) {
				u8Hours++;
				u8Hours %= 12;
			}

			switch (u8Hours) {
				default:
				case 0:
					oContainerTmp.setWord(5, 4, 5, oColors, bRandomColor);
				break;
				case 1:
					if (oRTC.u8Minutes < 5) {
						oContainerTmp.setWord(2, 5, 3, oColors, bRandomColor);
					}
					else {
						oContainerTmp.setWord(2, 5, 4, oColors, bRandomColor);
					}
				break;
				case 2:
					oContainerTmp.setWord(0, 5, 4, oColors, bRandomColor);
				break;
				case 3:
					oContainerTmp.setWord(1, 6, 4, oColors, bRandomColor);
				break;
				case 4:
					oContainerTmp.setWord(7, 7, 4, oColors, bRandomColor);
				break;
				case 5:
					oContainerTmp.setWord(7, 6, 4, oColors, bRandomColor);
				break;
				case 6:
					oContainerTmp.setWord(1, 9, 5, oColors, bRandomColor);
				break;
				case 7:
					oContainerTmp.setWord(5, 5, 6, oColors, bRandomColor);
				break;
				case 8:
					oContainerTmp.setWord(1, 8, 4, oColors, bRandomColor);
				break;
				case 9:
					oContainerTmp.setWord(3, 7, 4, oColors, bRandomColor);
				break;
				case 10:
					oContainerTmp.setWord(5, 8, 4, oColors, bRandomColor);
				break;
				case 11:
					oContainerTmp.setWord(0, 7, 3, oColors, bRandomColor);
				break;
			}
		break;

		case Language::Nederlands:

			if (oRTC.u8Minutes >= 20) {
				u8Hours++;
				u8Hours %= 12;
			}

			switch (u8Hours) {
				default:
				case 0:
					oContainerTmp.setWord(0, 9, 6, oColors, bRandomColor);
				break;
				case 1:
					oContainerTmp.setWord(7, 4, 3, oColors, bRandomColor);
				break;
				case 2:
					oContainerTmp.setWord(0, 5, 4, oColors, bRandomColor);
				break;
				case 3:
					oContainerTmp.setWord(7, 5, 4, oColors, bRandomColor);
				break;
				case 4:
					oContainerTmp.setWord(0, 6, 4, oColors, bRandomColor);
				break;
				case 5:
					oContainerTmp.setWord(4, 6, 4, oColors, bRandomColor);
				break;
				case 6:
					oContainerTmp.setWord(8, 6, 3, oColors, bRandomColor);
				break;
				case 7:
					oContainerTmp.setWord(0, 7, 5, oColors, bRandomColor);
				break;
				case 8:
					oContainerTmp.setWord(0, 8, 4, oColors, bRandomColor);
				break;
				case 9:
					oContainerTmp.setWord(6, 7, 5, oColors, bRandomColor);
				break;
				case 10:
					oContainerTmp.setWord(4, 8, 4, oColors, bRandomColor);
				break;
				case 11:
					oContainerTmp.setWord(8, 8, 3, oColors, bRandomColor);
				break;
			}
		break;
	}
}

void Wordclock::drawMinutes(const s_RTC& oRTC, Colors& oColors, bool bRandomColor, bool bDots) {
	uint8_t u8Minutes = oRTC.u8Minutes;

	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (u8Minutes < 5) {
				//Nothing
			}
			else if (u8Minutes < 10) {
				oContainerTmp.setWord(6, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 15) {
				oContainerTmp.setWord(5, 3, 3, oColors, bRandomColor);    //Zehn
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 20) {
				oContainerTmp.setWord(2, 1, 7, oColors, bRandomColor);    //viertel
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 25) {
				oContainerTmp.setWord(0, 2, 6, oColors, bRandomColor);    //zwanzig
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 30) {
				oContainerTmp.setWord(0, 2, 10, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 35) {
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //halb
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 40) {
				oContainerTmp.setWord(0, 2, 10, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(9, 3, 2, oColors, bRandomColor);    //to
			}
			else if (u8Minutes < 45) {
				oContainerTmp.setWord(0, 2, 6, oColors, bRandomColor);    //zwanzig
				oContainerTmp.setWord(9, 3, 2, oColors, bRandomColor);    //to
			}
			else if (u8Minutes < 50) {
				oContainerTmp.setWord(2, 1, 7, oColors, bRandomColor);    //viertel
				oContainerTmp.setWord(9, 3, 2, oColors, bRandomColor);    //to
			}
			else if (u8Minutes < 55) {
				oContainerTmp.setWord(5, 3, 3, oColors, bRandomColor);    //Zehn
				oContainerTmp.setWord(9, 3, 2, oColors, bRandomColor);    //to
			}
			else if (u8Minutes < 60) {
				oContainerTmp.setWord(6, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(9, 3, 2, oColors, bRandomColor);    //to
			}
		break;

		case Language::Deutsch:
			if (u8Minutes < 5) {
				//Nothing
			}
			else if (u8Minutes < 10) {
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(2, 3, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 15) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //Zehn
				oContainerTmp.setWord(2, 3, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 20) {
				oContainerTmp.setWord(4, 2, 7, oColors, bRandomColor);    //viertel
				oContainerTmp.setWord(2, 3, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 25) {
				oContainerTmp.setWord(4, 1, 7, oColors, bRandomColor);    //zwanzig
				oContainerTmp.setWord(2, 3, 4, oColors, bRandomColor);    //nach
			}
			else if (u8Minutes < 30) {
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(6, 3, 3, oColors, bRandomColor);    //vor
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //halb
			}
			else if (u8Minutes < 35) {
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //halb
			}
			else if (u8Minutes < 40) {
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(2, 3, 4, oColors, bRandomColor);    //nach
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //halb
			}
			else if (u8Minutes < 45) {
				oContainerTmp.setWord(4, 1, 7, oColors, bRandomColor);    //zwanzig
				oContainerTmp.setWord(6, 3, 3, oColors, bRandomColor);    //vor
			}
			else if (u8Minutes < 50) {
				oContainerTmp.setWord(4, 2, 7, oColors, bRandomColor);    //viertel
				oContainerTmp.setWord(6, 3, 3, oColors, bRandomColor);    //vor
			}
			else if (u8Minutes < 55) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //Zehn
				oContainerTmp.setWord(6, 3, 3, oColors, bRandomColor);    //vor
			}
			else if (u8Minutes < 60) {
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //fünf
				oContainerTmp.setWord(6, 3, 3, oColors, bRandomColor);    //vor
			}
		break;

		case Language::Nederlands:
			if (u8Minutes < 5) {
				//Nothing
			}
			else if (u8Minutes < 10) {
				oContainerTmp.setWord(7, 0, 4, oColors, bRandomColor);    //fijf
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //over
			}
			else if (u8Minutes < 15) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //tien
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //over
			}
			else if (u8Minutes < 20) {
				oContainerTmp.setWord(6, 2, 5, oColors, bRandomColor);    //kwaart
				oContainerTmp.setWord(7, 3, 4, oColors, bRandomColor);    //over2
			}
			else if (u8Minutes < 25) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //tien
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //voor
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //half
			}
			else if (u8Minutes < 30) {
				oContainerTmp.setWord(7, 0, 4, oColors, bRandomColor);    //fijf
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //voor
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //half
			}
			else if (u8Minutes < 35) {
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //half
			}
			else if (u8Minutes < 40) {
				oContainerTmp.setWord(7, 0, 4, oColors, bRandomColor);    //fijf
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //over
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //half
			}
			else if (u8Minutes < 45) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //tien
				oContainerTmp.setWord(0, 2, 4, oColors, bRandomColor);    //over
				oContainerTmp.setWord(0, 4, 4, oColors, bRandomColor);    //half
			}
			else if (u8Minutes < 50) {
				oContainerTmp.setWord(6, 2, 5, oColors, bRandomColor);    //kwaart
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //voor
			}
			else if (u8Minutes < 55) {
				oContainerTmp.setWord(0, 1, 4, oColors, bRandomColor);    //tien
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //voor
			}
			else if (u8Minutes < 60) {
				oContainerTmp.setWord(7, 0, 4, oColors, bRandomColor);    //fijf
				oContainerTmp.setWord(0, 3, 4, oColors, bRandomColor);    //voor
			}
		break;
	}

	if (bDots) {
		oContainerTmp.setWord(3, 10, u8Minutes % 5, oColors, bRandomColor);
	}
}

void Wordclock::drawText(uint8_t u8Minutes, Colors& oColors, bool bRandomColor, bool bItIs) {
	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (bItIs) {
				oContainerTmp.setWord(0, 2, oColors, bRandomColor);    //ES
				oContainerTmp.setWord(3, 2, oColors, bRandomColor);    //IST
			}

			if (u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainerTmp.setWord(5, 9, 6, oColors, bRandomColor);    //UHR
			}
		break;

		case Language::Deutsch:
			if (bItIs) {
				oContainerTmp.setWord(0, 2, oColors, bRandomColor);    //ES
				oContainerTmp.setWord(3, 3, oColors, bRandomColor);    //IST
			}

			if (u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainerTmp.setWord(107, 3, oColors, bRandomColor);    //UHR
			}
		break;

		case Language::Nederlands:
			if (bItIs) {
				oContainerTmp.setWord(0, 3, oColors, bRandomColor);    //ES
				oContainerTmp.setWord(4, 2, oColors, bRandomColor);    //IST
			}

			if (u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainerTmp.setWord(107, 3, oColors, bRandomColor);    //UHR
			}
		break;
	}
}

void Wordclock::drawAMPM(uint8_t u8Hours, Colors& oColors, bool bRandomColor) {
	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
		case Language::Deutsch:
			if (u8Hours < 12) {
				oContainerTmp.setWord(7, 2, oColors, bRandomColor);    //AM
			}
			else {
				oContainerTmp.setWord(9, 2, oColors, bRandomColor);    //PM
			}
		break;

		case Language::Nederlands:
			if (u8Hours < 12) {
				oContainerTmp.setWord(7, 1, 2, oColors, bRandomColor);    //AM
			}
			else {
				oContainerTmp.setWord(9, 1, 2, oColors, bRandomColor);    //PM
			}
		break;
	}
}
