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
#include "driver/stm32.hpp"

extern bool bLeftButton;
extern bool bRightButton;
extern bool bMiddleButton;
extern bool bLED;
extern uint8_t u8Light;

void STM32::configPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, Flags u8Mode, Flags u8Drive, Flags u8Pullup, uint8_t AF) {

}

void STM32::configPinOutput(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {

}

void STM32::configPinInput(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {

}

void STM32::configPinAnalog(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {

}

void STM32::configPinAF(GPIO_TypeDef* oGPIO, uint8_t u8Pin, uint8_t u8AF) {

}

void STM32::setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin, bool bVal) {
    if(oGPIO == GPIOB && u8Pin == 8){
        bLED = bVal;
    }
}

void STM32::setPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {

}

void STM32::clrPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {

}

bool STM32::getPin(GPIO_TypeDef* oGPIO, uint8_t u8Pin) {
    if(oGPIO == GPIOB && u8Pin == 5){
        return bLeftButton;
    }
    if(oGPIO == GPIOB && u8Pin == 4){
        return bMiddleButton;
    }
    if(oGPIO == GPIOB && u8Pin == 3){
        return bRightButton;
    }

    return false;
}

void STM32::setInterrupt(uint8_t u8Flag, uint8_t u8Priority) {

}

uint16_t STM32::getADC() {
    return 255-u8Light;
}
