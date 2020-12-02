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
#include "libs/ds18x20.hpp"
#include <QDateTime>

DS18X20::DS18X20(OneWire& oOneWire_l) : oOneWire(oOneWire_l) {
    srand(QDateTime::currentMSecsSinceEpoch() );
s16Temp = 250 + rand() % 120;
}

void DS18X20::getPower() {

}

bool DS18X20::convert() {


	return true;
}

bool DS18X20::startConvert() {
    return true;
}

bool DS18X20::read() {


	return true;
}

bool DS18X20::write() {

	return true;
}

#ifdef USING_DS18B20
bool DS18X20::setResolution(uint8_t u8Resolution) {
    uint8_t u8Val;

    if (!read()) {
        return false;
    }

    switch (u8Resolution) {
        default:
        case 9:
            u8Val = 0 << 5;
        break;
        case 10:
            u8Val = 1 << 5;
        break;
        case 11:
            u8Val = 2 << 5;
        break;
        case 12:
            u8Val = 3 << 5;
        break;
    }

    scratchpad.u8Config = u8Val;

    return write();
}
#endif

bool DS18X20::readTemp() {


	return true;
}

bool DS18X20::readTempParallel() {

    return true;
}

int16_t DS18X20::getTempRaw()  {
s16Temp++;
#ifdef USING_DS18B20
	return s16Temp & ((uint16_t) 0xffff << (3 - ((scratchpad.u8Config >> 5) & 0x03)));
#else
	return s16Temp;
#endif
}

int16_t DS18X20::getTemp()  {
#ifdef USING_DS18B20
	return s16Temp >> 4;
#else
	return s16Temp / 2;
#endif
}

float DS18X20::getTempFloat()  {
	float fResult = 0;


	return fResult;
}

bool DS18X20::change() {
	if (s16Temp != s16TempOld && (s16Temp > s16TempOld + 1 || s16Temp < s16TempOld - 1)) {
		s16TempOld = s16Temp;
		return true;
	}
	else {
		return false;
	}
}
