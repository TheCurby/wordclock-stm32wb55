#include "util/modbus.hpp"

uint8_t ModbusAscii::Master(uint8_t* pu8Dst, uint8_t* pu8Src, uint8_t u8Size) {
	uint8_t i;
	uint8_t u8Char;
	uint8_t u8Length;
	uint8_t au8RXBuffer_l[MB_RX_BUFFER_SIZE];

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

	if (u8Size == 0) {
		return RESP_NODATA;
	}

	while (u8Size-- > 0) {
		u8Char = *pu8Src++;

		if (u8Char == ':') {
			clearBuffer();
		}
		else if (u8Char == '\r') {
		}
		else if (u8Char == '\n') {
			u8Length = u8RXPos / 2;

			if (u8RXPos % 2 != 0) {
				clearBuffer();
				return RESP_ERROR;
			}

			for (i = 0; i < u8Length; i++) {
				au8RXBuffer_l[i] = hex2val(&au8RXBuffer_l[i * 2]);
			}

			if (CRCASCII(au8RXBuffer_l, u8Length - 1) != au8RXBuffer_l[u8Length - 1]) {
				return RESP_CRCERROR;
			}

			if (au8RXBuffer_l[0] != hex2val(&auTXBuffer[1])) {
				return RESP_ERROR;
			}

			if (au8RXBuffer_l[1] >= 0x80) {
				return RESP_ERROR;
			}

			if (au8RXBuffer_l[1] != hex2val(&auTXBuffer[3])) {
				clearBuffer();
				return RESP_ERROR;
			}

			switch (au8RXBuffer_l[1]) {
				case FC_ReadCoil:
				case FC_ReadInput:
					for (i = 0; i < u8FC12Size; i++) {
						if (au8RXBuffer_l[3 + i / 8] & (1 << (i % 8))) {
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
					for (i = 0; i < au8RXBuffer_l[2] / 2; i++) {
						uint16_t u16Val = au8RXBuffer_l[3 + 2 * i];
						u16Val <<= 8;
						u16Val |= au8RXBuffer_l[4 + 2 * i];
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
		else {
			if (u8RXPos >= MB_RX_BUFFER_SIZE) {
				clearBuffer();
				return RESP_Overflow;
			}
			au8RXBuffer_l[u8RXPos++] = u8Char;
		}
	}

	return RESP_NODATA;
}

bool ModbusAscii::fc1(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SLaveId);
	add(FC_ReadCoil);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	u8FC12Size = u16Size;

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc2(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SLaveId);
	add(FC_ReadInput);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	u8FC12Size = u16Size;

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc3(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SLaveId);
	add(FC_ReadHolding);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc4(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SLaveId);
	add(FC_ReadRegister);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Size >> 8);
	add(u16Size & 0xff);
	addCRC();

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc5(uint8_t u8SLaveId, uint16_t u16Addr, bool bVal) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
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

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc6(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Val) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SLaveId);
	add(FC_SetRegister);
	add(u16Addr >> 8);
	add(u16Addr & 0xff);
	add(u16Val >> 8);
	add(u16Val & 0xff);
	addCRC();

	if (u8TXPos == 17) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc15(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, bool* pabData) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
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

	if (u8TXPos == 19 + (calcn(u16Size)) * 2) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}

bool ModbusAscii::fc16(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, uint16_t* pu16Data) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
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

	if (u8TXPos == 19 + u16Size * 4) {
		tTimeout.StartMs(MB_TIMEOUT);
		u8Retries = MB_RETRIES;
		return true;
	}

	return false;
}
