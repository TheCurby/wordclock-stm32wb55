/*
 * rtc.c
 *
 *  Created on: Nov 14, 2019
 *      Author: Nils
 */

#include "driver/rtclock.hpp"

void RTClock::disableWP() {
	PWR->CR1 |= PWR_CR1_DBP;
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
}

void RTClock::set(const s_RTC oRTC) {
	uint32_t u32Tmp;

	disableWP();
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));

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

/*
 * Konvertiert eine BCD Zahl zu einer uint8_t
 */
uint8_t RTClock::bcd2val(const uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal >> 4) * 10;
	hvar += (ucVal & 0x0F);

	return hvar;
}

/*
 * Konvertiert eine uint8_t Zahl zu einer BCD
 */
uint8_t RTClock::val2bcd(const uint8_t ucVal) {
	uint8_t hvar;

	hvar = (ucVal / 10) << 4;
	hvar += (ucVal % 10);

	return hvar;
}
