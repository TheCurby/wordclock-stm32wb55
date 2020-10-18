#pragma once

#include <cstdint>
#include "util/timer.hpp"

class Modbus {
	protected:
		enum config {
			MB_N_COIL = 50,
			MB_N_REGISTER = 50,
			MB_N_INPUT = 50,
			MB_N_HOLDING = 50,
			MB_TX_BUFFER_SIZE = 200,
			MB_RX_BUFFER_SIZE = 200,
			MB_TIMEOUT = 200,
			MB_RETRIES = 3
		};

		typedef struct {
				uint8_t u8H;
				uint8_t u8L;
		} s_hex;

		enum FC {
			FC_ReadCoil = 1,
			FC_ReadInput,
			FC_ReadHolding,
			FC_ReadRegister,
			FC_SetCoil,
			FC_SetRegister,
			FC_SetCoils = 15,
			FC_SetRegisters
		};

		enum Resp {
			RESP_NODATA,
			RESP_DATA,
			RESP_Overflow,
			RESP_CRCERROR,
			RESP_SUCCESS,
			RESP_ERROR,
			RESP_TIMEOUT
		};

		enum Exceptions {
			EX_IllegalFunction = 1,
			EX_IllegalDataAdress,
			EX_SlaveDeviceFailure = 4
		};

	public:
		Modbus(uint8_t u8SlaveID, uint32_t u32Baud);
		void setCoil(uint16_t u16Addr, bool bVal);
		void setInput(uint16_t u16Addr, bool bVal);
		void setRegister(uint16_t u16Addr, uint16_t u16Val);
		void setHolding(uint16_t u16Addr, uint16_t u16Val);

		bool getCoil(uint16_t u16Addr);
		bool getInput(uint16_t u16Addr);
		uint16_t getRegister(uint16_t u16Addr);
		uint16_t getHolding(uint16_t u16Addr);

	protected:
		bool bReceiving;
		uint8_t u8Delay;
		uint8_t u8Retries;
		uint8_t u8FC12Size;
		uint8_t u8CRCBuffer;

		Timer tTimeout;

		uint8_t au8RXBuffer[MB_RX_BUFFER_SIZE];    //Eingangsbuffer
		uint8_t u8RXPos;    //Aktuelle Position Eingangsbuffer

		uint8_t auTXBuffer[MB_TX_BUFFER_SIZE];    //Ausgangsbuffer
		uint8_t u8TXPos;    //Aktuelle Position Ausgangsbuffer

		uint8_t u8SlavedID;    //Die SlaveID

		bool au8Coils[MB_N_COIL];    //Coil Bits
		bool au8Inputs[MB_N_INPUT];    //Input Bits
		uint16_t au16Register[MB_N_REGISTER];    //Register
		uint16_t au16Holding[MB_N_HOLDING];    //Holding

		uint8_t calcn(uint8_t n);
		void clearBuffer();
		uint8_t hex2val(uint8_t* ucH);
		s_hex val2hex(int8_t u8Val);
};

class ModbusAscii : public Modbus {
	public:
		ModbusAscii(uint8_t u8SlaveID, uint32_t u32Baud);
		uint8_t getData(uint8_t* pu8Data, uint8_t u8Size);
		uint8_t Master(uint8_t* pu8Dst, uint8_t* pu8Src, uint8_t u8Size);
		bool fc1(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc2(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc3(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc4(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc5(uint8_t u8SLaveId, uint16_t u16Addr, bool bVal);
		bool fc6(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Val);
		bool fc15(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, bool* pabData);
		bool fc16(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, uint16_t* pu16Data);

	private:
		void setException(uint8_t u8Val);
		void add(uint8_t u8Val);
		void addCRC();
		uint8_t CRCASCII(uint8_t* pu8Data, uint8_t u8Size);

};

class ModbusRTU : public Modbus {
	public:
		ModbusRTU(uint8_t u8SlaveID, uint32_t u32Baud);
		uint8_t getData(uint8_t* pu8Data, uint8_t u8Size);
		uint8_t Master(uint8_t* pu8Dst, uint8_t* pu8Src, uint8_t u8Size);
		bool fc1(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc2(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc3(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc4(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size);
		bool fc5(uint8_t u8SLaveId, uint16_t u16Addr, bool bVal);
		bool fc6(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Val);
		bool fc15(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, bool* pabData);
		bool fc16(uint8_t u8SLaveId, uint16_t u16Addr, uint16_t u16Size, uint16_t* pu16Data);
		bool receiveMaster();

	private:
		void setException(uint8_t u8Val);
		void add(uint8_t u8Val);
		void addCRC();
		uint16_t CRC16(uint8_t* pu8Data, uint8_t u8Size);

		Timer tRTU;
};
