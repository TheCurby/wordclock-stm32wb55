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

#define setOneWord(x, y, size) {oContainer.setWord(x, y, size, oColors, eMode);}
#define setTwoWords(x0, y0, size0, x1, y1, size1) {oContainer.setWord(x0, y0, size0, oColors, eMode); oContainer.setWord(x1, y1, size1, oColors, eMode);}
#define setThreeWords(x0, y0, size0, x1, y1, size1, x2, y2, size2) {oContainer.setWord(x0, y0, size0, oColors, eMode); oContainer.setWord(x1, y1, size1, oColors, eMode);oContainer.setWord(x2, y2, size2, oColors, eMode);}

void Wordclock::drawHours(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode) {
	uint8_t u8Hours = oRTC.u8Hours;

	u8Hours %= 12;

	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 12;
			}

			//@formatter:off
			switch (u8Hours) {
				default:
				case 0: setOneWord(5, 8, 6); break;
				case 1: setOneWord(0, 5, 3); break;
				case 2: setOneWord(8, 6, 3); break;
				case 3: setOneWord(6, 5, 5); break;
				case 4: setOneWord(0, 6, 4); break;
				case 5: setOneWord(4, 6, 4); break;
				case 6: setOneWord(3, 5, 3); break;
				case 7: setOneWord(0, 8, 5); break;
				case 8: setOneWord(0, 7, 5); break;
				case 9: setOneWord(7, 4, 4); break;
				case 10: setOneWord(0, 9, 3); break;
				case 11: setOneWord(5, 7, 6); break;
			}
			//@formatter:on
		break;

		case Language::German:
			if (oRTC.u8Minutes >= 25) {
				u8Hours++;
				u8Hours %= 12;
			}

			//@formatter:off
			switch (u8Hours) {
				default:
				case 0: setOneWord(5, 4, 5); break;
				case 1:
					if (oRTC.u8Minutes < 5) {
						setOneWord(2, 5, 3);
					}
					else {
						setOneWord(2, 5, 4);
					}
				break;
				case 2: setOneWord(0, 5, 4); break;
				case 3: setOneWord(1, 6, 4); break;
				case 4: setOneWord(7, 7, 4); break;
				case 5: setOneWord(7, 6, 4); break;
				case 6: setOneWord(1, 9, 5); break;
				case 7: setOneWord(5, 5, 6); break;
				case 8: setOneWord(1, 8, 4); break;
				case 9: setOneWord(3, 7, 4); break;
				case 10: setOneWord(5, 8, 4); break;
				case 11: setOneWord(0, 7, 3); break;
			}
			//@formatter:on
		break;

		case Language::Dutch:
			if (oRTC.u8Minutes >= 20) {
				u8Hours++;
				u8Hours %= 12;
			}

			//@formatter:off
			switch (u8Hours) {
				default:
				case 0: setOneWord(0, 9, 6); break;
				case 1: setOneWord(7, 4, 3); break;
				case 2: setOneWord(0, 5, 4); break;
				case 3: setOneWord(7, 5, 4); break;
				case 4: setOneWord(0, 6, 4); break;
				case 5: setOneWord(4, 6, 4); break;
				case 6: setOneWord(8, 6, 3); break;
				case 7: setOneWord(0, 7, 5); break;
				case 8: setOneWord(0, 8, 4); break;
				case 9: setOneWord(6, 7, 5); break;
				case 10: setOneWord(4, 8, 4); break;
				case 11: setOneWord(8, 8, 3); break;
			}
			//@formatter:on
		break;

		case Language::Italian:
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 12;
			}

			//@formatter:off
			switch (u8Hours) {
				default:
				case 0: setOneWord(0, 4, 6); break;
				case 1: setOneWord(2, 1, 4); break;
				case 2: setOneWord(7, 1, 3); break;
				case 3: setOneWord(0, 2, 3); break;
				case 4: setOneWord(0, 5, 7); break;
				case 5: setOneWord(0, 6, 6); break;
				case 6: setOneWord(8, 5, 3); break;
				case 7: setOneWord(6, 4, 5); break;
				case 8: setOneWord(3, 2, 4); break;
				case 9: setOneWord(7, 2, 4); break;
				case 10: setOneWord(0, 3, 5); break;
				case 11: setOneWord(5, 3, 6); break;
			}
			//@formatter:on
		break;
	}
}

void Wordclock::drawMinutes(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode, bool bDots) {
	uint8_t u8Minutes = oRTC.u8Minutes;

	//@formatter:off
	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (u8Minutes < 5) {/*nothing*/}
			else if (u8Minutes < 10) setTwoWords(6,2,4, 0,4,4)
			else if (u8Minutes < 15) setTwoWords(5,3,3, 0,4,4)
			else if (u8Minutes < 20) setTwoWords(2,1,7, 0,4,4)
			else if (u8Minutes < 25) setTwoWords(0,2,6, 0,4,4)
			else if (u8Minutes < 30) setTwoWords(0,2,10, 0,4,4)
			else if (u8Minutes < 35) setTwoWords(0,3,4, 0,4,4)
			else if (u8Minutes < 40) setTwoWords(0,2,10, 9,3,2)
			else if (u8Minutes < 45) setTwoWords(0,2,6, 9,3,2)
			else if (u8Minutes < 50) setTwoWords(2,1,7, 9,3,2)
			else if (u8Minutes < 55) setTwoWords(5,3,3, 9,3,2)
			else if (u8Minutes < 60) setTwoWords(6,2,4, 9,3,2)
		break;

		case Language::German:
			if (u8Minutes < 5) {/*nothing*/}
			else if (u8Minutes < 10) setTwoWords(0,2,4, 2,3,4)
			else if (u8Minutes < 15) setTwoWords(0,1,4, 2,3,4)
			else if (u8Minutes < 20) setTwoWords(4,2,7, 2,3,4)
			else if (u8Minutes < 25) setTwoWords(4,1,7, 2,3,4)
			else if (u8Minutes < 30) setThreeWords(0,2,4, 6,3,3, 0,4,4)
			else if (u8Minutes < 35) setOneWord(0,4,4)
			else if (u8Minutes < 40) setThreeWords(0,2,4, 2,3,4, 0,4,4)
			else if (u8Minutes < 45) setTwoWords(4,1,7, 6,3,3)
			else if (u8Minutes < 50) setTwoWords(4,2,7, 6,3,3)
			else if (u8Minutes < 55) setTwoWords(0,1,4, 6,3,3)
			else if (u8Minutes < 60) setTwoWords(0,2,4, 6,3,3)
		break;

		case Language::Dutch:
			if (u8Minutes < 5) {/*nothing*/}
			else if (u8Minutes < 10) setTwoWords(7,0,4, 0,2,4)
			else if (u8Minutes < 15) setTwoWords(0,1,4, 0,2,4)
			else if (u8Minutes < 20) setTwoWords(6,2,5, 7,3,4)
			else if (u8Minutes < 25) setThreeWords(0,1,4, 0,3,4, 0,4,4)
			else if (u8Minutes < 30) setThreeWords(7,0,4, 0,3,4, 0,4,4)
			else if (u8Minutes < 35) setOneWord(0,4,4)
			else if (u8Minutes < 40) setThreeWords(7,0,4, 0,2,4, 0,4,4)
			else if (u8Minutes < 45) setThreeWords(0,1,4, 0,2,4, 0,4,4)
			else if (u8Minutes < 50) setTwoWords(6,2,5, 0,3,4)
			else if (u8Minutes < 55) setTwoWords(0,1,4, 0,3,4)
			else if (u8Minutes < 60) setTwoWords(7,0,4, 0,3,4)
		break;

		case Language::Italian:
			if (u8Minutes < 5) {/*nothing*/}
			else if (u8Minutes < 10) setTwoWords(0,7,1, 5,8,6)
			else if (u8Minutes < 15) setTwoWords(0,7,1, 0,9,5)
			else if (u8Minutes < 20) setThreeWords(0,7,1, 2,7,2, 5,7,6)
			else if (u8Minutes < 25) setTwoWords(0,7,1, 0,8,5)
			else if (u8Minutes < 30) setTwoWords(0,7,1, 0,8,11)
			else if (u8Minutes < 35) setTwoWords(0,7,1, 6,9,5)
			else if (u8Minutes < 40) setTwoWords(7,6,4, 0,8,11)
			else if (u8Minutes < 45) setTwoWords(7,6,4, 0,8,5)
			else if (u8Minutes < 50) setThreeWords(7,6,4, 2,7,2, 5,7,6)
			else if (u8Minutes < 55) setTwoWords(7,6,4, 0,9,5)
			else if (u8Minutes < 60) setTwoWords(7,6,4, 5,8,6)
		break;
	}

	//@formatter:on
	if (bDots) {
		drawDots(oContainer, u8Minutes % 5, oColors, eMode);
	}
}

void Wordclock::drawText(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode, bool bItIs) {
	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (bItIs) {
				oContainer.setWord(0, 0, 2, oColors, eMode);
				oContainer.setWord(3, 0, 2, oColors, eMode);
			}

			if (oRTC.u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainer.setWord(5, 9, 6, oColors, eMode);
			}
		break;

		case Language::German:
			if (bItIs) {
				oContainer.setWord(0, 0, 2, oColors, eMode);
				oContainer.setWord(3, 0, 3, oColors, eMode);
			}

			if (oRTC.u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainer.setWord(8, 9, 3, oColors, eMode);
			}
		break;

		case Language::Dutch:
			if (bItIs) {
				oContainer.setWord(0, 0, 3, oColors, eMode);
				oContainer.setWord(4, 0, 2, oColors, eMode);
			}

			if (oRTC.u8Minutes < 5 && CurrentMenu != Menu::Temp) {
				oContainer.setWord(8, 9, 3, oColors, eMode);
			}
		break;

		case Language::Italian:{
			uint8_t u8Hours = oRTC.u8Hours;
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 12;
			}

			if (bItIs) {
				if(u8Hours % 12 == 1){
					oContainer.setWord(0, 1, 1, oColors, eMode);
				}
				else{
					oContainer.setWord(0, 0, 4, oColors, eMode);
					oContainer.setWord(5, 0, 2, oColors, eMode);
				}
			}
		}
		break;
	}
}

void Wordclock::drawAMPM(Container& oContainer, const s_RTC& oRTC, Colors& oColors, DisplayMode eMode) {
	uint8_t u8Hours = oRTC.u8Hours;

	switch (oSettings.getLanguage()) {
		default:
		case Language::English:
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 24;
			}

			if (u8Hours < 12) {
				oContainer.setWord(7, 0, 2, oColors, eMode);
			}
			else {
				oContainer.setWord(9, 0, 2, oColors, eMode);
			}
		break;
		case Language::German:
			if (oRTC.u8Minutes >= 25) {
				u8Hours++;
				u8Hours %= 24;
			}

			if (u8Hours < 12) {
				oContainer.setWord(7, 0, 2, oColors, eMode);
			}
			else {
				oContainer.setWord(9, 0, 2, oColors, eMode);
			}
		break;
		case Language::Italian:
			if (oRTC.u8Minutes >= 35) {
				u8Hours++;
				u8Hours %= 24;
			}

			if (u8Hours < 12) {
				oContainer.setWord(7, 0, 2, oColors, eMode);
			}
			else {
				oContainer.setWord(9, 0, 2, oColors, eMode);
			}
		break;

		case Language::Dutch:
			if (oRTC.u8Minutes >= 20) {
				u8Hours++;
				u8Hours %= 24;
			}

			if (u8Hours < 12) {
				oContainer.setWord(7, 1, 2, oColors, eMode);
			}
			else {
				oContainer.setWord(9, 1, 2, oColors, eMode);
			}
		break;
	}
}
