////////////////////////////////////////////////////////////////////////////////
// Wordclock V1.0
// Copyright (C) 2020 Nils Handelmann
// E-Mail: nils.handelmann@gmail.com
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
#include "libs/m95m01.hpp"
#include <QFile>
using namespace std;

M95M01::M95M01(SPI& oSPI_l, GPIO_TypeDef* oGPIO_l, uint16_t u16Pin_l) : oSPI(oSPI_l) {

}

void M95M01::Wren(bool bVal) {

}

void M95M01::Read(uint32_t usAddr, uint8_t* pucDest, uint16_t usSize) {
    /*QFile in("config");
    in.open(QIODevice::ReadOnly);
    in.read((char *)pucDest, usSize);
    in.close();*/
}

void M95M01::Write(uint32_t usAddr, uint8_t* pucSrc, uint16_t usSize) {
    /*QFile out("config");
    out.open(QIODevice::WriteOnly);
    out.write((const char *)pucSrc, usSize);
    out.close();*/
}

bool M95M01::Busy() {
	return false;
}

bool M95M01::WaitForEEPROM() {


	return true;
}

void M95M01::setCS(bool bVal) {

}
