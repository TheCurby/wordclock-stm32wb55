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
#include "util/modbus.hpp"

ModbusRTU::ModbusRTU(uint8_t u8SlaveID, uint32_t u32Baud) : Modbus(u8SlaveID, u32Baud) {

}

uint8_t ModbusRTU::getData(uint8_t* pu8Data, uint8_t u8Size) {
	uint8_t i;
	uint8_t* au8RXBuffer_t = &au8RXBuffer[0];
	uint8_t u8Val;
	uint16_t u16Data1;
	uint16_t u16Data2;

	if (!bReceiving && u8Size > 0) {
		clearBuffer();
		bReceiving = true;

		while (u8Size-- > 0) {
			au8RXBuffer_t[u8RXPos++] = *pu8Data++;

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
			au8RXBuffer_t[u8RXPos++] = *pu8Data++;

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

		uint16_t u16CRC = au8RXBuffer_t[u8RXPos - 1];
		u16CRC <<= 8;
		u16CRC |= au8RXBuffer_t[u8RXPos - 2];
		if (CRC16(au8RXBuffer_t, u8RXPos - 2) != u16CRC) {
			clearBuffer();
			return RESP_CRCERROR;
		}

		if (au8RXBuffer_t[0] != u8SlavedID) {
			clearBuffer();
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
				}
				else if (11 + calcn(u16Data2) > MB_TX_BUFFER_SIZE) {
					setException(EX_SlaveDeviceFailure);
				}
				else {
					u8TXPos = 0;
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
				}
			break;

			case FC_ReadInput:
				if (u16Data1 + u16Data2 > MB_N_INPUT) {
					setException(EX_IllegalDataAdress);
				}
				else if (11 + calcn(u16Data2) > MB_TX_BUFFER_SIZE) {
					setException(EX_SlaveDeviceFailure);
				}
				else {
					u8TXPos = 0;
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
				}
			break;

			case FC_ReadRegister:
				if (u16Data1 + u16Data2 > MB_N_REGISTER) {
					setException(EX_IllegalDataAdress);
				}
				else if (11 + u16Data2 * 2 > MB_TX_BUFFER_SIZE) {
					setException(EX_SlaveDeviceFailure);
				}
				else {
					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_ReadRegister);
					add(u16Data2 * 2);

					for (i = 0; i < u16Data2; i++) {
						uint16_t u16Val = getRegister(u16Data1 + i);
						add(u16Val >> 8);
						add(u16Val & 0xff);
					}

					addCRC();
				}
			break;

			case FC_ReadHolding:
				if (u16Data1 + u16Data2 > MB_N_HOLDING) {
					setException(EX_IllegalDataAdress);
				}
				else if (11 + u16Data2 * 2 > MB_TX_BUFFER_SIZE) {
					setException(EX_SlaveDeviceFailure);
				}
				else {
					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_ReadHolding);
					add(u16Data2 * 2);

					for (i = 0; i < u16Data2; i++) {
						uint16_t u16Val = getHolding(u16Data1 + i);
						add(u16Val >> 8);
						add(u16Val & 0xff);
					}

					addCRC();
				}
			break;

			case FC_SetCoil:
				if (u16Data1 >= MB_N_COIL) {
					setException(EX_IllegalDataAdress);
				}
				else {
					if (u16Data2 == 0xFF00) {
						setCoil(u16Data1, true);
					}
					else {
						setCoil(u16Data1, false);
					}

					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_SetCoil);
					add(u16Data1 >> 8);
					add(u16Data1 & 0xff);
					add(u16Data2 >> 8);
					add(u16Data2 & 0xff);
					addCRC();
				}
			break;

			case FC_SetRegister:
				if (u16Data1 >= MB_N_HOLDING) {
					setException(EX_IllegalDataAdress);
				}
				else {
					setHolding(u16Data1, u16Data2);

					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_SetRegister);
					add(u16Data1 >> 8);
					add(u16Data1 & 0xff);
					add(u16Data2 >> 8);
					add(u16Data2 & 0xff);
					addCRC();
				}
			break;

			case FC_SetCoils:
				if (u16Data1 + u16Data2 > MB_N_COIL) {
					setException(EX_IllegalDataAdress);
				}
				else {
					for (i = 0; i < u16Data2; i++) {
						setCoil(u16Data1 + i, au8RXBuffer_t[7 + i / 8] & (1 << (i % 8)));
					}

					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_SetCoils);
					add(u16Data1 >> 8);
					add(u16Data1 & 0xff);
					add(u16Data2 >> 8);
					add(u16Data2 & 0xff);
					addCRC();
				}
			break;

			case FC_SetRegisters:
				if (u16Data1 + u16Data2 > MB_N_HOLDING) {
					setException(EX_IllegalDataAdress);
				}
				else {
					for (uint16_t j = 0; j < u16Data2; j++) {
						setHolding(u16Data1 + j, ((uint16_t) au8RXBuffer_t[7 + j * 2]) | au8RXBuffer_t[8 + j * 2]);
					}

					u8TXPos = 0;
					add(u8SlavedID);
					add(FC_SetRegisters);
					add(u16Data1 >> 8);
					add(u16Data1 & 0xff);
					add(u16Data2 >> 8);
					add(u16Data2 & 0xff);
					addCRC();
				}
			break;

			default:
				setException(EX_IllegalFunction);
			break;
		}

		clearBuffer();
		return RESP_DATA;
	}

	return RESP_NODATA;
}

void ModbusRTU::setException(uint8_t u8Val) {
	u8TXPos = 0;
	add(u8SlavedID);
	add(au8RXBuffer[1] | 0x80);
	add(u8Val);
	addCRC();
}

void ModbusRTU::add(uint8_t u8Val) {
	if (u8TXPos < MB_TX_BUFFER_SIZE) {
		auTXBuffer[u8TXPos++] = u8Val;
	}
}

void ModbusRTU::addCRC() {
	uint16_t u16CRC = CRC16(auTXBuffer, u8TXPos);
	add(u16CRC & 0xff);
	add(u16CRC >> 8);
}

uint16_t ModbusRTU::CRC16(uint8_t* pu8Data, uint8_t u8Size) {
	uint8_t i;
	uint16_t u16CRC = 0xFFFF;

	while (u8Size-- > 0) {
		u16CRC ^= *pu8Data++;

		for (i = 0; i < 8; i++) {
			if ((u16CRC & 0x0001) != 0) {
				u16CRC >>= 1;
				u16CRC ^= 0xA001;
			}
			else
				u16CRC >>= 1;
		}
	}

	return u16CRC;
}
