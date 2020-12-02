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

#include "system/stm32wbxx_it.hpp"

extern "C" void NMI_Handler(void){
}

extern "C" void HardFault_Handler(void){
  while (1){
  }
}

extern "C" void MemManage_Handler(void){
  while (1){
  }
}

extern "C" void BusFault_Handler(void){
  while (1){
  }
}

extern "C" void UsageFault_Handler(void){
  while (1){
  }
}

extern "C" void SVC_Handler(void){
}

extern "C" void DebugMon_Handler(void){
}

extern "C" void PendSV_Handler(void){
}

extern "C" void SysTick_Handler(void){
}
