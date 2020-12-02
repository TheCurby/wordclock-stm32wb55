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

ModbusAscii::ModbusAscii(uint8_t u8SlaveID, uint32_t u32Baud) : Modbus(u8SlaveID, u32Baud) {

}

uint8_t ModbusAscii::getData(uint8_t* pu8Data, uint8_t u8Size) {
	uint8_t i;
	uint8_t u8Length;
	uint8_t u8Char;
	uint16_t u16Data1;
	uint16_t u16Data2;
	uint8_t au8RXBuffer_t[MB_RX_BUFFER_SIZE];
	uint8_t u8Val;

	if (u8Size == 0) {
		return RESP_NODATA;
	}

	while (u8Size-- > 0) {
		u8Char = *pu8Data++;

		switch (u8Char) {
			case '\r':
			break;

			case ':':
				clearBuffer();
			break;

			case '\n':
				u8Length = u8RXPos / 2;

				if (u8RXPos % 2 != 0) {
					return RESP_NODATA;
				}

				for (i = 0; i < u8Length; i++) {
					au8RXBuffer_t[i] = hex2val(&au8RXBuffer_t[i * 2]);
				}

				if (CRCASCII(au8RXBuffer_t, u8Length - 1) != au8RXBuffer_t[u8Length - 1]) {
					return RESP_CRCERROR;
				}

				if (au8RXBuffer_t[0] != u8SlavedID) {
					return RESP_NODATA;
				}

				u16Data1 = au8RXBuffer_t[2];
				u16Data1 <<= 8;
				u16Data1 |= au8RXBuffer_t[3];

				u16Data2 = au8RXBuffer_t[4];
				u16Data2 <<= 8;
				u16Data2 |= au8RXBuffer_t[5];

				switch (au8RXBuffer_t[1]) {
					case FC_ReadCoil:
						if (u16Data1 + u16Data2 > MB_N_COIL) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}
						else if (11 + calcn(u16Data2) > MB_TX_BUFFER_SIZE) {
							setException(EX_SlaveDeviceFailure);
							return RESP_DATA;
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_ReadCoil);
						add(calcn(u16Data2));

						u8Val = 0;
						for (i = 0; i < u16Data2; i++) {
							if (getCoil(u16Data1 + i)) {
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

						return RESP_DATA;
					break;

					case FC_ReadInput:
						if (u16Data1 + u16Data2 > MB_N_INPUT) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}
						else if (11 + calcn(u16Data2) > MB_TX_BUFFER_SIZE) {
							setException(EX_SlaveDeviceFailure);
							return RESP_DATA;
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_ReadInput);
						add(calcn(u16Data2));

						u8Val = 0;
						for (i = 0; i < u16Data2; i++) {
							if (getInput(u16Data1 + i)) {
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

						return RESP_DATA;
					break;

					case FC_ReadRegister:
						if (u16Data1 + u16Data2 > MB_N_REGISTER) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}
						else if (11 + u16Data2 * 2 > MB_TX_BUFFER_SIZE) {
							setException(EX_SlaveDeviceFailure);
							return RESP_DATA;
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_ReadRegister);
						add(u16Data2 * 2);

						for (i = 0; i < u16Data2; i++) {
							uint16_t u16Val = getRegister(u16Data1 + i);
							add(u16Val >> 8);
							add(u16Val & 0xff);
						}
						addCRC();

						return RESP_DATA;
					break;

					case FC_ReadHolding:
						if (u16Data1 + u16Data2 > MB_N_HOLDING) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}
						else if (11 + u16Data2 * 2 > MB_TX_BUFFER_SIZE) {
							setException(EX_SlaveDeviceFailure);
							return RESP_DATA;
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_ReadHolding);
						add(u16Data2 * 2);

						for (i = 0; i < u16Data2; i++) {
							uint16_t u16Val = getHolding(u16Data1 + i);
							add(u16Val >> 8);
							add(u16Val & 0xff);
						}
						addCRC();

						return RESP_DATA;
					break;

					case FC_SetCoil:
						if (u16Data1 >= MB_N_COIL) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}

						if (u16Data2 == 0xFF00) {
							setCoil(u16Data1, true);
						}
						else {
							setCoil(u16Data1, false);
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_SetCoil);
						add(u16Data1 >> 8);
						add(u16Data1 & 0xff);
						add(u16Data2 >> 8);
						add(u16Data2 & 0xff);
						addCRC();

						return RESP_DATA;
					break;

					case FC_SetRegister:
						if (u16Data1 >= MB_N_HOLDING) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}

						setHolding(u16Data1, u16Data2);

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_SetRegister);
						add(u16Data1 >> 8);
						add(u16Data1 & 0xff);
						add(u16Data2 >> 8);
						add(u16Data2 & 0xff);
						addCRC();

						return RESP_DATA;
					break;

					case FC_SetCoils:
						if (u16Data1 + u16Data2 > MB_N_COIL) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}

						for (i = 0; i < u16Data2; i++) {
							setCoil(u16Data1 + i, au8RXBuffer_t[7 + i / 8] & (1 << (i % 8)));
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_SetCoils);
						add(u16Data1 >> 8);
						add(u16Data1 & 0xff);
						add(u16Data2 >> 8);
						add(u16Data2 & 0xff);
						addCRC();

						return RESP_DATA;
					break;

					case FC_SetRegisters:
						if (u16Data1 + u16Data2 > MB_N_HOLDING) {
							setException(EX_IllegalDataAdress);
							return RESP_DATA;
						}

						for (uint16_t j = 0; j < u16Data2; j++) {
							setHolding(u16Data1 + j, ((uint16_t) au8RXBuffer_t[7 + j * 2]) | au8RXBuffer_t[8 + j * 2]);
						}

						u8CRCBuffer = 0;
						u8TXPos = 0;
						auTXBuffer[u8TXPos++] = ':';
						add(u8SlavedID);
						add(FC_SetRegisters);
						add(u16Data1 >> 8);
						add(u16Data1 & 0xff);
						add(u16Data2 >> 8);
						add(u16Data2 & 0xff);
						addCRC();

						return RESP_DATA;
					break;

					default:
						setException(EX_IllegalFunction);
						return RESP_DATA;
					break;
				}
			break;

			default:
				if (u8RXPos >= MB_RX_BUFFER_SIZE) {
					clearBuffer();
					return RESP_Overflow;
				}
				au8RXBuffer_t[u8RXPos++] = u8Char;
			break;
		}
	}

	return RESP_NODATA;
}

void ModbusAscii::setException(uint8_t u8Val) {
	u8CRCBuffer = 0;
	u8TXPos = 0;
	auTXBuffer[u8TXPos++] = ':';
	add(u8SlavedID);
	add(hex2val(&au8RXBuffer[2]) | 0x80);
	add(u8Val);
	addCRC();
}

void ModbusAscii::add(uint8_t u8Val) {
	s_hex oHex;

	u8CRCBuffer += u8Val;
	oHex = val2hex(u8Val);
	if (u8TXPos < MB_TX_BUFFER_SIZE) {
		auTXBuffer[u8TXPos++] = oHex.u8H;
	}
	if (u8TXPos < MB_TX_BUFFER_SIZE) {
		auTXBuffer[u8TXPos++] = oHex.u8L;
	}
}

void ModbusAscii::addCRC() {
	add((u8CRCBuffer ^ 0xff) + 1);
	if (u8TXPos < MB_TX_BUFFER_SIZE) {
		auTXBuffer[u8TXPos++] = 0x0D;
	}
	if (u8TXPos < MB_TX_BUFFER_SIZE) {
		auTXBuffer[u8TXPos++] = 0x0A;
	}
}

uint8_t ModbusAscii::CRCASCII(uint8_t* pu8Data, uint8_t u8Size) {
	uint8_t u8Result = 0;

	while (u8Size-- > 0) {
		u8Result += *pu8Data++;
	}
	u8Result ^= 0xFF;
	u8Result++;

	return u8Result;
}
