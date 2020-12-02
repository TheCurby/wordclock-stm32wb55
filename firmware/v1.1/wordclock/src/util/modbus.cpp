////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
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
#include "util/modbus.hpp"

Modbus::Modbus(uint8_t u8SlaveID, uint32_t u32Baud) {
	uint8_t i;

	for (i = 0; i < MB_N_COIL; i++) {
		au8Coils[i] = 0;
	}

	for (i = 0; i < MB_N_INPUT; i++) {
		au8Inputs[i] = 0;
	}

	for (i = 0; i < MB_N_HOLDING; i++) {
		au16Holding[i] = 0;
	}

	for (i = 0; i < MB_N_REGISTER; i++) {
		au16Register[i] = 0;
	}

	u8Retries = MB_RETRIES;
	u8FC12Size = 0;
	u8CRCBuffer = 0;
	u8RXPos = 0;
	u8TXPos = 0;
	u8SlavedID = u8SlaveID;
	bReceiving = false;
	u8Delay = 28000 / u32Baud + 1;
	if (u8Delay == 0) {
		u8Delay = 1;
	}
	tTimeout.Stop();
}

void Modbus::clearBuffer() {
	u8RXPos = 0;
}

void Modbus::setCoil(uint16_t u16Addr, bool bVal) {
	if (u16Addr >= MB_N_COIL) {
		return;
	}

	au8Coils[u16Addr] = bVal;
}

void Modbus::setInput(uint16_t u16Addr, bool bVal) {
	if (u16Addr >= MB_N_INPUT) {
		return;
	}

	au8Inputs[u16Addr] = bVal;
}

void Modbus::setRegister(uint16_t u16Addr, uint16_t u16Val) {
	if (u16Addr >= MB_N_REGISTER) {
		return;
	}

	au16Register[u16Addr] = u16Val;
}

void Modbus::setHolding(uint16_t u16Addr, uint16_t u16Val) {
	if (u16Addr >= MB_N_HOLDING) {
		return;
	}

	au16Holding[u16Addr] = u16Val;
}

bool Modbus::getCoil(uint16_t u16Addr) {
	if (u16Addr >= MB_N_COIL) {
		return false;
	}

	return au8Coils[u16Addr];
}

bool Modbus::getInput(uint16_t u16Addr) {
	if (u16Addr >= MB_N_INPUT) {
		return false;
	}

	return au8Inputs[u16Addr];
}

uint16_t Modbus::getRegister(uint16_t u16Addr) {
	if (u16Addr >= MB_N_REGISTER) {
		return 0;
	}

	return au16Register[u16Addr];
}

uint16_t Modbus::getHolding(uint16_t u16Addr) {
	if (u16Addr >= MB_N_HOLDING) {
		return 0;
	}

	return au16Holding[u16Addr];
}

uint8_t Modbus::hex2val(uint8_t* ucH) {
	uint8_t i;
	uint8_t ucResult = 0;

	for (i = 0; i < 2; i++) {
		ucResult <<= 4;

		if (*ucH >= 'a')
			ucResult += *ucH - ('a' - 10);
		else if (*ucH >= 'A')
			ucResult += *ucH - ('A' - 10);
		else
			ucResult += *ucH - ('0');

		ucH++;
	}

	return ucResult;
}

Modbus::s_hex Modbus::val2hex(int8_t u8Val) {
	s_hex oResult;
	uint8_t u8temp;

	u8temp = u8Val & 0x0F;
	if (u8temp <= 9)
		oResult.u8L = u8temp + '0';
	else
		oResult.u8L = u8temp + 'A' - 10;

	u8temp = (u8Val & 0xF0) >> 4;
	if (u8temp <= 9)
		oResult.u8H = u8temp + '0';
	else
		oResult.u8H = u8temp + 'A' - 10;

	return oResult;
}

uint8_t Modbus::calcn(uint8_t n) {
	return n / 8 + (n % 8 != 0 ? 1 : 0);
}
