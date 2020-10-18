//Konstanten nicht auf Stack
//Mehr Syntax checken
//Variablen: Geltungsbereich, Typen
//Optimierungen
//Funktionen
//Reihenfolge in Ausdrücken

#pragma once

#include <string>
#include <string.h>

class Script {
	private:
		static constexpr uint8_t STACKSIZE = 30;
		static constexpr uint8_t CODESIZE = 255;

	public:
		Script(char* sExpression);

		uint16_t u16PC;    //Aktuelle Position im Bytecode
		uint16_t u16ByteCode[CODESIZE];    //Bytecode

		//@formatter:off
		enum ERRORCODE {
			ERROR_NONE, ERROR_VAR, ERROR_EXPR, ERROR_UNKNWONVAR, ERROR_SYNTAX, ERROR_BRACKET, ERROR_ELSIF,
			ERROR_OVERFLOW, ERROR_MEM, ERROR_UNKNOWN
		};

		enum COMMANDS {
			C_NONE,

			C_LDI, C_LDK, C_PULLV, C_PULL, C_PULLS,

			C_ADD, C_SUB, C_MUL, C_DIV, C_MOD,
			C_ADDK, C_SUBK, C_MULK, C_DIVK, C_MODK,
			C_ANDI, C_ORI, C_XORI,

			C_JMP, C_JMPT, C_JMPF,
			C_AND, C_OR, C_EQ, C_NEQ, C_COUT,
			C_NOP,
			C_LED, C_OUTPUT, C_CLEAR, C_SLEEP
		};
		//@formatter:on
	private:
		enum Jumps {
			J_LOOP,
			J_WHILE,
			J_WHILEELSE,
			J_FOR,
			J_IF,
			J_ELSIF,
			J_ELSE,
		};

		enum DataTypes {
			Def,
			ConstNull,
			ConstGrNull,
			Adress,
			Var,
			Expr
		};

		char* pCode;    //Zeiger auf den Code
		char* str;    //Wanderner Zeiger auf den Code
		uint8_t u8SP;
		uint16_t au16Stack[STACKSIZE];
		uint8_t u8IfEndPos;
		uint16_t au16IfEnd[STACKSIZE];
		uint8_t u8BreakPos;
		uint16_t au16Break[STACKSIZE];
		uint8_t u8ContinuePos;
		uint16_t au16Continue[STACKSIZE];
		uint8_t u8ForTempPos;
		char* acForTemp[STACKSIZE];

		void add(uint32_t u32Val);    //F�gt ein Zeichen in den Bytecode hinzu
		void strrpl(char* str_l, char replace, char newChar, char cEnd);    //Ersetzt in einem String ein Zeichen durch ein anderes bis zu einem Endzeichen
		void trim(char** p = nullptr);    //�berspringt alle Tabulatoren und leerzeichen im Code
		void trim_e(uint8_t u8Size);    //Springt u8Size Zeichen im QUellcode weiter und f�hrt dann trim aus. Es wird anschlie�end ein Zeilenende erwartet.
		void nospace(char* p = NULL);    //Wirft einen Fehler, wenn kein Leezeichen an der aktuellen Position ist.
		void noend(char* p = NULL);    //Wirft einen Fehler, wenn kein Zeilenende vorliegt.
		uint8_t loadElement();
		bool isChar(char z);
		bool isNum(char z);
		bool charcmp(char z, char* sComp);
		uint8_t strcnt(char* str_l, char z, char cEnd);
		int8_t varpos(char* name, uint8_t p);
		bool expr();
		int16_t charfind(char* str_l, char z, char cEnd);
		void Compile(char* sExpression);
};

class ScriptException : public std::exception {
	public:
		ScriptException(const Script::ERRORCODE error_l) : error(error_l) {
		}

		virtual const char* what() const throw() {
			return m_msg.c_str();
		}

	private:
		std::string m_msg;
		Script::ERRORCODE error;
};
