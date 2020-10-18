#pragma once

#include <cstdint>
#include "driver/spi.hpp"
#include "system/stm32.hpp"
#include "util/timer.hpp"

class M95M01 {
	public:
		static constexpr uint16_t u16Timeout = 100;
		static constexpr uint32_t ut32MemSize = 131072UL;
		static constexpr uint16_t u16PageSize = 256;

		M95M01(SPI& oSPI_l, GPIO_TypeDef* oGPIO_l, uint16_t u16Pin_l);
		void Read(uint32_t usAddr, uint8_t* pucDest, uint16_t usSize);    //Daten vom EEprom lesen
		void Write(uint32_t usAddr, uint8_t* pucSrc, uint16_t usSize);    //Daten vom EEPROM speichern

	protected:
		void setCS(bool bVal);
		void Wren(bool bVal);    //Funktion erlaubt/verbietet Schreibzugriffe aufs EEPROM
		bool Busy();    //Pr�ft, ob das EEPROM besch�ftigt ist
		bool WaitForEEPROM();    //Die Funktion wartet darauf, dass das EEPROM nicht mehr busy ist

		SPI& oSPI;
		GPIO_TypeDef* oGPIO;
		uint16_t u16Pin;
};
