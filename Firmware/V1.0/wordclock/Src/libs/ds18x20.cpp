#include "libs/ds18x20.hpp"

DS18X20::DS18X20(OneWire& oOneWire_l) : oOneWire(oOneWire_l) {
	bParasitePower = false;
	s16Temp = 0;
	s16TempOld = 0;
	u8Status = 0;

	oROMCode.u8Family = 0;
	for (uint8_t i = 0; i < 6; i++) {
		oROMCode.au8Code[i] = 0;
	}
	oROMCode.u8CRC = 0;

	scratchpad.au8Temp[0] = 0;
	scratchpad.au8Temp[1] = 0;
	scratchpad.u8CRC = 0;
	scratchpad.u8TH = 0;
	scratchpad.u8TL = 0;
#ifdef DS18B20
	scratchpad.u8Config = 0;
	scratchpad.au8Void[0] = 0;
	scratchpad.au8Void[1] = 0;
#else
	scratchpad.u8CountPer = 0;
	scratchpad.u8CountRemain = 0;
	scratchpad.au8Void[0] = 0;
	scratchpad.au8Void[1] = 0;
	scratchpad.au8Void[2] = 0;
#endif
}

void DS18X20::getPower() {
	if (!oOneWire.skipROM()) {
		return;
	}

	oOneWire.setByte(0xb4);

	if (!oOneWire.getBool()) {
		bParasitePower = true;
	}
	else {
		bParasitePower = false;
	}
}

bool DS18X20::convert() {
	if (!oOneWire.skipROM()) {
		return false;
	}

	oOneWire.setByte(0x44);

	tTimeout.StartMs(u16Timeout);
	while (oOneWire.getBool()) {
		if (tTimeout.Ready()) {
			return false;
		}
	}

	return true;
}

bool DS18X20::startConvert() {
	if (!oOneWire.skipROM()) {
		return false;
	}

	tTimeout.StartMs(u16Timeout);
	oOneWire.setByte(0x44);
	return true;
}

bool DS18X20::read() {
	if (!oOneWire.skipROM()) {
		return false;
	}

	oOneWire.setByte(0xbe);

	scratchpad.au8Temp[0] = oOneWire.getByte();
	scratchpad.au8Temp[1] = oOneWire.getByte();
	scratchpad.u8TH = oOneWire.getByte();
	scratchpad.u8TL = oOneWire.getByte();
#ifdef DS18B20
	scratchpad.u8Config = oOneWire.getByte();
	scratchpad.au8Void[0] = oOneWire.getByte();
	scratchpad.au8Void[1] = oOneWire.getByte();
	scratchpad.au8Void[2] = oOneWire.getByte();
#else
	scratchpad.au8Void[0] = oOneWire.getByte();
	scratchpad.au8Void[1] = oOneWire.getByte();
	scratchpad.u8CountRemain = oOneWire.getByte();
	scratchpad.u8CountPer = oOneWire.getByte();
#endif
	scratchpad.u8CRC = oOneWire.getByte();

	uint8_t u8CRC = oOneWire.calcCRC(&scratchpad.au8Temp[0], 8);
	if (scratchpad.u8CRC != u8CRC) {
		return false;
	}

	return true;
}

bool DS18X20::write() {
	if (!oOneWire.skipROM()) {
		return false;
	}

	oOneWire.setByte(0x4e);
	oOneWire.setByte(scratchpad.u8TH);
	oOneWire.setByte(scratchpad.u8TL);
#ifdef DS18B20
	oOneWire.setByte(scratchpad.u8Config);
#endif

	return true;
}

#ifdef DS18B20
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
	if (!convert()) {
		return false;
	}

	tTimeout.StartMs(u16Timeout);

	if (read()) {
		s16Temp = 0;
		s16Temp |= scratchpad.au8Temp[1];
		s16Temp <<= 8;
		s16Temp |= scratchpad.au8Temp[0];
	}
	else {
		return false;
	}

	return true;
}

bool DS18X20::readTempParallel() {
	bool bResult = false;

	switch (u8Status) {
		default:
		case 0:
			if (startConvert()) {
				u8Status = 1;
			}
		break;

		case 1:
			if (!oOneWire.getBool()) {
				u8Status = 2;
			}
			else if (tTimeout.Ready()) {
				u8Status = 0;
			}
		break;

		case 2:
			if (read()) {
				s16Temp = 0;
				s16Temp |= scratchpad.au8Temp[1];
				s16Temp <<= 8;
				s16Temp |= scratchpad.au8Temp[0];

				bResult = true;
			}

			u8Status = 0;
		break;
	}

	return bResult;
}

int16_t DS18X20::getTempRaw() const {
#ifdef DS18B20
	return s16Temp & ((uint16_t) 0xffff << (3 - ((scratchpad.u8Config >> 5) & 0x03)));
#else
	return s16Temp;
#endif
}

int16_t DS18X20::getTemp() const {
#ifdef DS18B20
	return s16Temp >> 4;
#else
	return s16Temp / 2;
#endif
}

float DS18X20::getTempFloat() const {
	float fResult = 0;

#ifdef DS18B20
	fResult = s16Temp & ((uint16_t) 0xffff << (3 - ((scratchpad.u8Config >> 5) & 0x03)));
	fResult /= 16;
#else
	fResult = s16Temp;
	fResult -= 0.25;
	fResult += (float) (scratchpad.u8CountPer - scratchpad.u8CountRemain)
			/ scratchpad.u8CountPer;
	fResult /= 2;
#endif

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
