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
#pragma once

#include "wordclock/wordclock_typedef.hpp"

/*Define this to compile for DS18B20. Undefine will compile for DS18S20.*/
#define USING_DS18B20

/*Compile for SK6812 or WS2812B*/
//#define USING_WS2812

/*Define this to enable temperature menu*/
#define ENABLE_TEMP

/*Define this to use fahrenheit instead of celsius*/
//#define ENABLE_FAHRENHEIT

/*Define this to show celsius/fahrenheit symbol*/
#define ENABLE_TEMP_SYMBOL

/*Define this to enable DCF77*/
#define ENABLE_DCF77

/*Define this to enable the ldr correction*/
#define ENABLE_LDR

/*Define this to enable night mode*/
#define ENABLE_NIGHTMODE

/*Define this to switch power off during night mode*/
#define ENABLE_NIGHT_POWEROFF

/*Define this to enable the seconds menu*/
#define ENABLE_SECONDS

/*Define this to enable animations*/
#define ENABLE_ANIMATIONS
#define ENABLE_ANIMATION_FADE
#define ENABLE_ANIMATION_PLOP
#define ENABLE_ANIMATION_MOVE
#define ENABLE_ANIMATION_DROP
#define ENABLE_ANIMATION_MATRIX
#define ENABLE_ANIMATION_TELETYPE
#define ENABLE_ANIMATION_SLIDER

/*Define  will enable the language menu. Otherwise select a default language*/
//#define ENABLE_LANGUAGE
constexpr Language DefaultLanguage = Language::Dutch;
