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

uint8_t ModbusRTU::Master(uint8_t* pu8Dst, uint8_t* pu8Src, uint8_t u8Size) {
	uint8_t i;

	if (tTimeout.Ready()) {
		tTimeout.StartMs(MB_TIMEOUT);
		clearBuffer();

		if (u8Retries > 0) {
			u8Retries--;
			return RESP_TIMEOUT;
		}
		else {
			return RESP_ERROR;
		}
	}

	if (!bReceiving && u8Size > 0) {
		clearBuffer();
		bReceiving = true;

		while (u8Size-- > 0) {
			au8RXBuffer[u8RXPos++] = *pu8Src++;

			if (u8RXPos >= MB_RX_BUFFER_SIZE) {
				clearBuffer();
				return RESP_Overflow;
			}
		}

		tRTU.StartMs(u8Delay);

		return RESP_NODATA;
	}
	else if (bReceiving && !tRTU.Ready() && u8Size > 0) {
		while (u8Size-- > 0) {
			au8RXBuffer[u8RXPos++] = *pu8Src++;

			if (u8RXPos >= MB_RX_BUFFER_SIZE) {
				clearBuffer();
				return RESP_Overflow;
			}
		}

		tRTU.StartMs(u8Delay);

		return RESP_NODATA;
	}
	else if (bReceiving && tRTU.Ready()) {
		bReceiving = false;

		uint16_t u16CRC = au8RXBuffer[u8RXPos - 1];
		u16CRC <<= 8;
		u16CRC |= au8RXBuffer[u8RXPos - 2];
		if (CRC16(au8RXBuffer, u8RXPos - 2) != u16CRC) {
			clearBuffer();
			return RESP_CRCERROR;
		}

		if (au8RXBuffer[0] != auTXBuffer[0]) {
			clearBuffer();
			return RESP_ERROR;
		}

		if (au8RXBuffer[1] >= 0x80) {
			return RESP_ERROR;
		}

		if (au8RXBuffer[1] != auTXBuffer[1]) {
			clearBuffer();
			return RESP_ERROR;
		}

		switch (au8RXBuffer[1]) {
			case FC_ReadCoil:
			case FC_ReadInput:
				for (i = 0; i < u8FC12Size; i++) {
					if (au8RXBuffer[3 + i / 8] & (1 << (i % 8))) {
						*pu8Dst++ = true;
					}
					else {
						*pu8Dst++ = false;
					}
				}

				clearBuffer();
				return RESP_SUCCESS;
			break;

			case FC_ReadHolding:
			case FC_ReadRegister:
				for (i = 0; i < au8RXBuffer[2] / 2; i++) {
					uint16_t u16Val = au8RXBuffer[3 + 2 * i];
					u16Val <<= 8;
					u16Val |= au8RXBuffer[4 + 2 * i];
					*pu8Dst = u16Val;
					pu8Dst += 2;
				}

				clearBuffer();
				return RESP_SUCCESS;
			break;

			case FC_SetCoil:
			case FC_SetRegister:
			case FC_SetCoils:
			case FC_SetRegisters:
				clearBuffer();
				return RESP_SUCCESS;
			break;

			default:
				clearBuffer();
				return RESP_ERROR;
			break;
		}
	}

	return RESP_NODATA;
}

bool ModbusRTU::fc1(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_ReadCoil);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	u8FC12Size = u16Size;

	if (u8TXPos == 8) {
		return true;
	}

	return false;
}

bool ModbusRTU::fc2(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_ReadInput);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	u8FC12Size = u16Size;

	if (u8TXPos == 8) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc3(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_ReadRegister);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	if (u8TXPos == 8) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc4(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_ReadHolding);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	if (u8TXPos == 8) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc5(uint8_t u8SLaveId, uint16_t u16Addr, bool bVal) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_SetCoil);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	if (bVal) {
		add(0xff);
	}
	else {
		add(0);
	}
	add(0);
	addCRC();

	if (u8TXPos == 8) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc6(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Val) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_SetRegister);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Val >> 8);
	add(u16Val & 0xff);
	addCRC();

	if (u8TXPos == 8) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc15(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, bool* pabData) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_SetCoils);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	add(calcn(u16Size));

	uint8_t i;
	uint8_t u8Val = 0;
	for (i = 0; i < u16Size; i++) {
		if (pabData[i]) {
			u8Val |= (1 << (i % 8));
		}

		if ((i + 1) % 8 == 0) {
			add(u8Val);
			u8Val = 0;
		}
	}
	if (i % 8 != 0) {
		add(u8Val);
		u8Val = 0;
	}
	addCRC();

	if (u8TXPos == 9 + (calcn(u16Size))) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusRTU::fc16(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, uint16_t* pu16Data) {
	u8TXPos = 0;
	add(u8SLaveId);
	add(FC_SetRegisters);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	add(u16Size * 2);
	for (uint8_t i = 0; i < u16Size; i++) {
		add(*pu16Data >> 8);
		add(*pu16Data++ & 0xff);
	}
	addCRC();

	if (u8TXPos == 9 + u16Size * 2) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}
