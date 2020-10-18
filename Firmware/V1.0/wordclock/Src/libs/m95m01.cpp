#include "libs/m95m01.hpp"

M95M01::M95M01(SPI& oSPI_l, GPIO_TypeDef* oGPIO_l, uint16_t u16Pin_l) : oSPI(oSPI_l) {
	oGPIO = oGPIO_l;
	u16Pin = u16Pin_l;
}

/*
 * Schreibschutz des EEPROMs �ndern
 * */
void M95M01::Wren(bool bVal) {
	setCS(false);
	oSPI.send(bVal ? 0x06 : 0x04);
	setCS(true);
}

/*
 * Aus dem EEPROM wird ab einer bestimmten Adresse,
 * eine bestimmte Anzahl an Bytes gelesen,
 * und an eine Speicheradresse kopiert.
 * */
void M95M01::Read(uint32_t usAddr, uint8_t* pucDest, uint16_t usSize) {
	if (!WaitForEEPROM()) {
		return;
	}

	setCS(false);
	oSPI.send(0x03);
	oSPI.send((uint8_t) ((usAddr >> 16) & 0xff));
	oSPI.send((uint8_t) ((usAddr >> 8) & 0xff));
	oSPI.send((uint8_t) (usAddr & 0xff));
	while (usSize-- > 0) {
		*pucDest++ = oSPI.send(0);
	}
	setCS(true);
}

/*
 * In das EEPROM wird an eine bestimmten Adresse,
 * eine bestimmte Anzahl an Bytes geschrieben,
 * die aus einer Speicheradresse kopiert werden.
 * */
void M95M01::Write(uint32_t usAddr, uint8_t* pucSrc, uint16_t usSize) {
	if (!WaitForEEPROM()) {
		return;
	}

	Wren(true);
	setCS(false);
	oSPI.send(0x02);
	oSPI.send((uint8_t) ((usAddr >> 16) & 0xff));
	oSPI.send((uint8_t) ((usAddr >> 8) & 0xff));
	oSPI.send((uint8_t) (usAddr & 0xff));

	while (usSize-- > 0) {
		oSPI.send(*pucSrc++);

		/*
		 * Das Ende Page ist erreicht.
		 * Bisher gesendete Bytes abspeichern.
		 * ANschlie�end �bertragung neu initialisieren
		 * */
		if (++usAddr % u16PageSize == 0) {
			setCS(true);
			if (!WaitForEEPROM()) {
				return;
			}
			Wren(true);
			setCS(false);
			oSPI.send(0x02);
			oSPI.send((uint8_t) ((usAddr >> 16) & 0xff));
			oSPI.send((uint8_t) ((usAddr >> 8) & 0xff));
			oSPI.send((uint8_t) (usAddr & 0xff));
		}
	}
	setCS(true);
}

/*
 * Abfragen, ob das EEPROM bereit ist
 * */
bool M95M01::Busy() {
	uint8_t ucStatus;

	setCS(false);
	oSPI.send(0x05);
	ucStatus = oSPI.send(0);
	setCS(true);

	if (ucStatus & 1) {
		return true;
	}

	return false;
}

/*
 * Auf einen Schreibvorgang des EEPROMs warten
 * Warteschleife enth�lt Timeout um Deadlock zu verhindern.
 * */
bool M95M01::WaitForEEPROM() {
	Timer tTimeout;

	tTimeout.StartMs(u16Timeout);
	while (Busy()) {
		if (tTimeout.Ready()) {
			return false;
		}
	}

	return true;
}

void M95M01::setCS(bool bVal) {
	STM32::setPin(oGPIO, u16Pin, bVal);
}
