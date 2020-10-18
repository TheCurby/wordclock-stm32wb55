#pragma once

#include <cstdint>
#include <atomic>
#include "system/stm32.hpp"

class Timer {
	public:
		Timer();
		Timer(uint64_t u32Ms);
		void StartMs(uint64_t u32Ms);    //Timer mit mS Laufzeit starten
		void StartUs(uint64_t u32Ms);    //Timer mit mS Laufzeit starten
		void StartS(uint64_t u32S);    //Timer mit mS Laufzeit starten
		void Restart();
		void Stop();    //Timer stoppen
		bool Ready();    //Pr�fen ob Timer ready
		bool ReadyEdge();    //Pr�fen ob Timer ready
		uint64_t RestMs();    //Restzeit vom Timer ermittlen
		uint64_t RestUs();    //Restzeit vom Timer ermittlen
		uint64_t ElapsedMs();    //Restzeit vom Timer ermittlen
		uint64_t ElapsedUs();    //Restzeit vom Timer ermittlen
		static void incOverflow();

	private:
		uint64_t u64Start;    //Startzeit
		uint64_t u64Time;    //Laufzeit
		bool bRunning;    //L�uft er?
		static std::atomic_uint32_t u32OverFlow;

		inline uint64_t getTimestamp();
};
