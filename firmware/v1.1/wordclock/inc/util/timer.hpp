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
#pragma once

#include <cstdint>
#include <atomic>
#include "driver/stm32.hpp"

class Timer {
	public:
		Timer();
		Timer(uint64_t u32Ms);
		void StartMs(uint64_t u32Ms);
		void StartUs(uint64_t u32Ms);
		void StartS(uint64_t u32S);
		void Restart();
		void Stop();
		bool Ready();
		bool ReadyEdge();
		uint64_t RestMs();
		uint64_t RestUs();
		uint64_t ElapsedMs();
		uint64_t ElapsedUs();
		static void incOverflow();
		static void setReference(uint32_t * p);

	private:
		uint64_t u64Start;
		uint64_t u64Time;
		bool bRunning;
		static std::atomic<uint32_t> u32OverFlow;
		static uint32_t * u32Timestamp;

		uint64_t getTimestamp();
};
