#pragma once

#include "util/timer.hpp"
#include "driver/oneWire.hpp"

#define DS18B20

#ifdef DS18B20
#define DS_FAMILY 0x28
#else
#define DS_FAMILY 0x10
#endif

class DS18X20 {
	public:
		DS18X20(OneWire& oOneWire_l);
		bool change();
		void getPower();
		bool readTemp();
		bool readTempParallel();
		int16_t getTempRaw() const;
		int16_t getTemp() const;
		float getTempFloat() const;
#ifdef DS18B20
		bool setResolution(uint8_t u8Resolution);
#endif

	protected:
		bool bParasitePower;
		uint8_t u8Status;
		int16_t s16Temp;
		int16_t s16TempOld;
		Timer tTimeout;
		OneWire::s_ROMCode oROMCode;
		OneWire& oOneWire;

		struct {
				uint8_t au8Temp[2];
				uint8_t u8TH;
				uint8_t u8TL;
#ifndef DS18B20
				uint8_t au8Void[2];
				uint8_t u8CountRemain;
				uint8_t u8CountPer;
#else
				uint8_t u8Config;
				uint8_t au8Void[3];
#endif
				uint8_t u8CRC;
		} scratchpad;

		bool convert();
		bool startConvert();
		bool read();
		bool write();

		static constexpr uint16_t u16Timeout = 5000;
};
