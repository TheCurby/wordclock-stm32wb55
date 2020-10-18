#include "util/CPU.hpp"
#include "system/stm32.hpp"

void CPU::run(Script& scr) {

	while (1) {
		uint32_t ins = scr.u16ByteCode[pc];

		switch (ins) {
			default: {
				break;
			}
			break;

			case Script::COMMANDS::C_NONE:
			break;

			case Script::COMMANDS::C_LDI:
				stack[stackpos++] = vars[scr.u16ByteCode[pc + 1]];
				pc += 2;
			break;

			case Script::COMMANDS::C_LDK:
				stack[stackpos++] = scr.u16ByteCode[pc + 1];
				pc += 2;
			break;

			case Script::COMMANDS::C_PULLV:
				vars[scr.u16ByteCode[pc + 1]] = stack[--stackpos];
				pc += 2;
			break;

			case Script::COMMANDS::C_PULLS: {
				uint32_t u32Val = stack[--stackpos];
				vars[stack[stackpos - 1]] = u32Val;
				pc++;
			}
			break;

			case Script::COMMANDS::C_PULL:
				stackpos--;
				pc++;
			break;

			case Script::COMMANDS::C_ADD:
				stack[stackpos - 2] += stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_SUB:
				stack[stackpos - 2] -= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_MUL:
				stack[stackpos - 2] *= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_DIV:
				stack[stackpos - 2] /= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_MOD:
				stack[stackpos - 2] %= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_ANDI:
				stack[stackpos - 2] &= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_ORI:
				stack[stackpos - 2] |= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_XORI:
				stack[stackpos - 2] ^= stack[stackpos - 1];
				stackpos--;
				pc += 1;
			break;

			case Script::COMMANDS::C_JMP:
				pc = scr.u16ByteCode[pc + 1];
			break;

			case Script::COMMANDS::C_JMPT:
				if (stack[stackpos - 1] != 0) {
					pc = scr.u16ByteCode[pc + 1];
				}
				else {
					pc += 2;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_JMPF:
				if (stack[stackpos - 1] == 0) {
					pc = scr.u16ByteCode[pc + 1];
				}
				else {
					pc += 2;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_AND:
				if (stack[stackpos - 1] != 1 || stack[stackpos - 2] != 1) {
					pc = scr.u16ByteCode[pc + 1];
					stack[stackpos - 2] = 0;
				}
				else {
					stack[stackpos - 1] = 1;
					pc += 2;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_OR:
				if (stack[stackpos - 1] == 0 && stack[stackpos - 2] == 0) {
					pc = scr.u16ByteCode[pc + 1];
					stack[stackpos - 2] = 0;
				}
				else {
					pc += 2;
					stack[stackpos - 2] = 1;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_EQ:
				if (stack[stackpos - 1] != stack[stackpos - 2]) {
					pc = scr.u16ByteCode[pc + 1];
					stack[stackpos - 2] = 0;
				}
				else {
					pc += 2;
					stack[stackpos - 2] = 1;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_NEQ:
				if (stack[stackpos - 1] == stack[stackpos - 2]) {
					pc = scr.u16ByteCode[pc + 1];
					stack[stackpos - 2] = 0;
				}
				else {
					pc += 2;
					stack[stackpos - 2] = 1;
				}
				stackpos--;
			break;

			case Script::COMMANDS::C_LED: {
				uint8_t w = stack[--stackpos];
				uint8_t b = stack[--stackpos];
				uint8_t g = stack[--stackpos];
				uint8_t r = stack[--stackpos];
				Colors oColors;
				oColors.set(r, g, b, w);
				set(stack[--stackpos], oColors);
				pc++;
			}
			break;

			case Script::COMMANDS::C_CLEAR:
				clear();
				pc++;
			break;

			case Script::COMMANDS::C_SLEEP:
				sleep(stack[--stackpos]);
				pc++;
			break;

			case Script::COMMANDS::C_OUTPUT:
				output();
				pc++;
			break;

			case Script::COMMANDS::C_COUT:
			break;
		}
	}
}
