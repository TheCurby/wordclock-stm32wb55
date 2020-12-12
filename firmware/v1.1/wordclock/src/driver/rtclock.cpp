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
#include "driver/rtclock.hpp"

void RTClock::disableWP() {
	PWR->CR1 |= PWR_CR1_DBP;
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}

void RTClock::set(const s_RTC& oRTC) {
	uint32_t u32Tmp;

	disableWP();
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));
	RTC->PRER = 0x7f00ff;

	u32Tmp = 0;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Hours / 10) & 0x03) << 20;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Hours % 10) & 0x0f) << 16;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Minutes / 10) & 0x07) << 12;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Minutes % 10) & 0x0f) << 8;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Seconds / 10) & 0x07) << 4;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Seconds % 10) & 0x0f) << 0;
	RTC->TR = u32Tmp;

	u32Tmp = 0;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Year / 10) & 0x0f) << 20;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Year % 10) & 0x0f) << 16;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Month / 10) & 0x01) << 12;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Month % 10) & 0x0f) << 8;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Day / 10) & 0x03) << 4;
	u32Tmp |= (uint32_t) (val2bcd(oRTC.u8Day % 10) & 0x0f) << 0;
	RTC->DR = u32Tmp;

	RTC->ISR &= ~RTC_ISR_INIT;
	RTC->WPR = 0xff;
}

s_RTC RTClock::get() {
	uint32_t u32Tmp;
	s_RTC oRTC;

	u32Tmp = RTC->TR;
	oRTC.u8Hours = bcd2val((u32Tmp >> 20) & 0x03) * 10 + bcd2val((u32Tmp >> 16) & 0x0f);
	oRTC.u8Minutes = bcd2val((u32Tmp >> 12) & 0x07) * 10 + bcd2val((u32Tmp >> 8) & 0x0f);
	oRTC.u8Seconds = bcd2val((u32Tmp >> 4) & 0x07) * 10 + bcd2val((u32Tmp >> 0) & 0x0f);

	u32Tmp = RTC->DR;
	oRTC.u8Year = bcd2val((u32Tmp >> 20) & 0x0f) * 10 + bcd2val((u32Tmp >> 16) & 0x0f);
	oRTC.u8Month = bcd2val((u32Tmp >> 12) & 0x01) * 10 + bcd2val((u32Tmp >> 8) & 0x0f);
	oRTC.u8Day = bcd2val((u32Tmp >> 4) & 0x03) * 10 + bcd2val((u32Tmp >> 0) & 0x0f);

	return oRTC;
}

int32_t RTClock::toTimeStamp(const s_RTC& oRTC) {
	return oRTC.u8Hours * 60 + oRTC.u8Minutes;
}

uint8_t RTClock::bcd2val(const uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal >> 4) * 10;
	hvar += (ucVal & 0x0F);

	return hvar;
}

uint8_t RTClock::val2bcd(const uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal / 10) << 4;
	hvar += (ucVal % 10);

	return hvar;
}
