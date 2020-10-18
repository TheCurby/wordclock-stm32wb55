#pragma once

#include <cstdint>
#include <atomic>
#include "system/stm32.hpp"

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

	private:
		uint64_t u64Start;
		uint64_t u64Time;
		bool bRunning;
		static std::atomic_uint32_t u32OverFlow;

		inline uint64_t getTimestamp();
};
