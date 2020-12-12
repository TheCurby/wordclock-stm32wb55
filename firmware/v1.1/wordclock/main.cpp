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
#include "wordclock/wordclock.hpp"

static Wordclock oWordclock;

extern "C" void HardFault_Handler(void){
	oWordclock.oSettings.incErrorCount();
	oWordclock.oSettings.writeChanges();

	/*struct ErrorCode{
		uint32_t u32Key;
		uint32_t u32UFSR;
	} oErrorCode;

	oErrorCode.u32Key = 0x1234ABCD;
	oErrorCode.u32UFSR = ;

	oWordclock.oEEPROM.Write(M95M01::ut32MemSize - sizeof(uint32_t) - sizeof(ErrorCode), (uint8_t *)&oErrorCode, sizeof(ErrorCode));*/

	while (1){
	}
}

extern "C" void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;

	Timer::incOverflow();
}

extern "C" int main() {
	while (true) {
		oWordclock.loop();
	}

	return 0;
}
