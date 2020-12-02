////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
// Copyright (C) 2020 Nils Handelmann
// E-Mail: nils.handelmann@gmail.com
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

extern s_RTC oRTC_l;

void RTClock::disableWP() {

}

void RTClock::set(const s_RTC oRTC) {
    oRTC_l = oRTC;
}

s_RTC RTClock::get() {
    return oRTC_l;
}

uint8_t RTClock::bcd2val(const uint8_t ucVal) {
    return 0;
}

uint8_t RTClock::val2bcd(const uint8_t ucVal) {
    return 0;
}
