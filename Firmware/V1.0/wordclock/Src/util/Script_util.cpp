#include "util/Script.hpp"

void Script::trim(char** p) {
	if (p == nullptr) {
		p = &str;
	}

	while (**p == ' ' || **p == '\t') {
		*p += 1;

		if (**p == '\0') {
			break;
		}
	}
}

void Script::trim_e(uint8_t u8Size) {
	str += u8Size;

	trim();

	if (!(*str == '\n' || *str == '\0')) {
		throw ScriptException(ERROR_SYNTAX);
	}
}

void Script::add(uint32_t u32Val) {
	if (u16PC >= CODESIZE) {
		throw ScriptException(ERROR_MEM);
	}

	u16ByteCode[u16PC] = u32Val;
	u16PC++;
}

void Script::nospace(char* p) {
	if (p == nullptr) {
		p = str;
	}

	if (!(*p == ' ' || *p == '\t')) {
		throw ScriptException(ERROR_SYNTAX);
	}
}

void Script::noend(char* p) {
	if (p == nullptr) {
		p = str;
	}

	if (!(*p == '\n' || *p == '\0')) {
		throw ScriptException(ERROR_SYNTAX);
	}
}

/*
 * Diese Funktion ersetzt in einem ganzen String ein zeichen
 */
void Script::strrpl(char* str_l, char replace, char newChar, char cEnd) {
	while (*str_l != '\0' && *str_l != cEnd) {
		if (*str_l == replace) {
			*str_l = newChar;
		}
		str_l++;
	}
}

/*
 * Diese Funktion z�hlt bis Zeilenende oder Stringende ein Zeichen
 */
uint8_t Script::strcnt(char* str_l, char z, char cEnd) {
	uint8_t u8Result = 0;

	while (*str_l != '\0' && *str_l != cEnd) {
		if (*str_l++ == z) {
			u8Result++;
		}
	}

	return u8Result;
}

/*
 * Diese Funktion sucht im ganzen String ein Zeichen einer zeichenmenge
 */
int16_t Script::charfind(char* str_l, char z, char cEnd) {
	uint16_t c = 0;

	while (*str_l != '\0' && *str_l != cEnd) {

		if (*str_l == z) {
			return c;
		}

		c++;
		str_l++;
	}

	return -1;
}

/*
 * Diese Funktion pr�ft ob ein Zeichen einer Menge anderer Zeichen entspricht
 */
bool Script::charcmp(char z, char* sComp) {
	uint8_t l = strlen(sComp);

	for (uint8_t i = 0; i < l; i++) {
		if (z == sComp[i]) {
			return true;
		}
	}

	return false;
}

bool Script::isChar(char z) {
	if ((z >= 'a' && z <= 'z') || (z >= 'A' && z <= 'Z')) {
		return true;
	}

	return false;
}

bool Script::isNum(char z) {
	if ((z >= '0' && z <= '9')) {
		return true;
	}

	return false;
}
