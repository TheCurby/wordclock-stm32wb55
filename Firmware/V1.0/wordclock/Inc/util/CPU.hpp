#pragma once

#include "util/Script.hpp"
#include "wordclock/Colors.hpp"

class CPU {
	protected:
		static constexpr uint16_t STACKSIZE = 200;
		static constexpr uint16_t VARSIZE = 20;

		uint32_t stack[STACKSIZE];
		uint32_t vars[VARSIZE];
		uint8_t stackpos = 0;
		uint8_t pc = 0;

	public:
		void run(Script& scr);
		virtual void set(uint8_t u8Val, Colors& oColors) = 0;
		virtual void sleep(uint16_t u16Val) = 0;
		virtual void clear() = 0;
		virtual void output() = 0;
};
