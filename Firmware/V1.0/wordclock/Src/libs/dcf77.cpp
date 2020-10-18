/*
 * dcf77.c
 *
 *  Created on: Nov 14, 2019
 *      Author: Nils
 */

#include "libs/dcf77.hpp"

DCF77::DCF77() {
	oRTC.u8Day = 0;
	oRTC.u8Hours = 0;
	oRTC.u8Minutes = 0;
	oRTC.u8Month = 0;
	oRTC.u8Seconds = 0;
	oRTC.u8Year = 0;
	oRTC_prev.u8Day = 0;
	oRTC_prev.u8Hours = 0;
	oRTC_prev.u8Minutes = 0;
	oRTC_prev.u8Month = 0;
	oRTC_prev.u8Seconds = 0;
	oRTC_prev.u8Year = 0;

	bSense = false;
	bSecond = false;

	Reset();
}

bool DCF77::receive(bool bVal) {
	bool bResult = false;

	/*
	 * Signalauswertung
	 * */
	if (!bVal && !bSense) {
		bSense = true;

		if (t.ElapsedMs() > 1500 && !t.Ready()) {

			if (u8Pos == 59) {
				if (!bSecond) {
					oRTC_prev = Decode();
					bSecond = true;
				}
				else {
					oRTC = Decode();
					if (Validate()) {
						bResult = true;
						oRTC_prev = oRTC;
					}
					else {
						bSecond = false;
					}
				}
			}
			else {
				bSecond = false;
			}
			Reset();
		}

		t.StartMs(20000);
	}
	else if (bVal && bSense) {
		bSense = false;

		if (u8Pos < 59) {
			uint16_t u16Elp = t.ElapsedMs();

			if (!(u16Elp < 20 || u16Elp > 300 || !tDelay.Ready())) {
				if (u16Elp > 165) {
					au8Data[u8Pos / 8] |= (1 << (u8Pos % 8));
				}
				u8Pos++;
				tDelay.StartMs(600);
			}
		}
	}
	//}

	return bResult;
}

void DCF77::Reset() {
	u8Pos = 0;

	for (uint8_t i = 0; i < 8; i++) {
		au8Data[i] = 0;
	}
}

s_RTC DCF77::Decode() {
	s_RTC oRTC_l;

	oRTC_l.u8Day = bcd2val(getBCD(36, 6));
	oRTC_l.u8Hours = bcd2val(getBCD(29, 6));
	oRTC_l.u8Minutes = bcd2val(getBCD(21, 7));
	oRTC_l.u8Month = bcd2val(getBCD(45, 5));
	oRTC_l.u8Seconds = 0;
	oRTC_l.u8Year = bcd2val(getBCD(50, 8));

	return oRTC_l;
}

bool DCF77::Validate() {
	if (!(oRTC.u8Day >= 1 && oRTC.u8Day <= 31 && oRTC.u8Hours < 24 && oRTC.u8Minutes <= 59 && oRTC.u8Month >= 1 && oRTC.u8Month <= 12 && oRTC.u8Year <= 99)) {
		return false;
	}
	else if (!(oRTC_prev.u8Day >= 1 && oRTC_prev.u8Day <= 31 && oRTC_prev.u8Hours < 24 && oRTC_prev.u8Minutes <= 59 && oRTC_prev.u8Month >= 1 && oRTC_prev.u8Month <= 12
		&& oRTC_prev.u8Year <= 99)) {
		return false;
	}
	else if ((oRTC.u8Minutes + oRTC_prev.u8Minutes) % 2 == 0) {
		return false;
	}
	/*else if ((oRTC.u8Minutes - 1 != oRTC_prev.u8Minutes) || !(oRTC.u8Minutes == 0 && oRTC_prev.u8Minutes == 59)) {
	 return false;
	 }*/
	else if (getBCD(0, 1) || !getBCD(20, 1)) {
		return false;
	}
	else if (oRTC.u8Year != oRTC_prev.u8Year) {
		return false;
	}
	else if (oRTC.u8Month != oRTC_prev.u8Month) {
		return false;
	}
	else if (oRTC.u8Day != oRTC_prev.u8Day) {
		return false;
	}

	return true;
}

uint8_t DCF77::getBCD(uint8_t u8Pos_l, uint8_t u8Length) {
	uint8_t u8Result = 0;

	for (uint8_t i = 0; i < u8Length; i++) {
		if (au8Data[u8Pos_l / 8] & (1 << (u8Pos_l % 8))) {
			u8Result |= (1 << i);
		}
		u8Pos_l++;
	}

	return u8Result;
}

s_RTC DCF77::getRTC() {
	return oRTC;
}

uint8_t DCF77::bcd2val(uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal >> 4) * 10;
	hvar += (ucVal & 0x0F);

	return hvar;
}

/*
 * Konvertiert eine uint8_t Zahl zu einer BCD
 */
uint8_t DCF77::val2bcd(uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal / 10) << 4;
	hvar |= (ucVal % 10);

	return hvar;
}
