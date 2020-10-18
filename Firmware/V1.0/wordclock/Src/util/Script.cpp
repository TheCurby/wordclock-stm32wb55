#include "util/Script.hpp"

#define OVERFLOW_STACK(a) if(u8SP + a > STACKSIZE) {throw ScriptException(ERROR_OVERFLOW);}
#define OVERFLOW_IF(a) if(u8IfEndPos + a > STACKSIZE) {throw ScriptException(ERROR_OVERFLOW);}
#define OVERFLOW_BREAK(a) if(u8BreakPos + a > STACKSIZE) {throw ScriptException(ERROR_OVERFLOW);}
#define OVERFLOW_CONTINUE(a) if(u8ContinuePos + a > STACKSIZE) {throw ScriptException(ERROR_OVERFLOW);}

Script::Script(char* sExpression) {
	Compile(sExpression);
}

void Script::Compile(char* sExpression) {
	pCode = sExpression;
	str = sExpression;
	uint16_t* pu16Dst = u16ByteCode;

	u16PC = 0;
	u8SP = 0;
	u8IfEndPos = 0;
	u8BreakPos = 0;
	u8ContinuePos = 0;
	u8ForTempPos = 0;

	/*
	 */
	strrpl(str, '\r', ' ', '\0');

	/*
	 * Alle Kommentare ersetzen
	 * */
	char* sFound;
	for (sFound = strstr(str, "#"); sFound != NULL; sFound = strstr(str, "#")) {
		do {
			*sFound++ = ' ';
		} while (!(*sFound == '\0' || *sFound == '\n'));
	}

	do {
		trim();

		if (*str == '\n') {
			str++;
			//Nothing
		}
		else if (*str == '\0') {
			break;
		}
		else if (strncmp(str, "if", 2) == 0) {
			OVERFLOW_STACK(3);

			str += 2;
			nospace();
			trim();

			au16Stack[u8SP++] = u16PC;    //Startadresse vom IF im Stack speichern
			expr();
			add(C_JMPF);    //Wenn bedingung NICHT erf�llt ist, zum elseif / else / end springen

			au16Stack[u8SP++] = u16PC;    //Sprungadresse merken; wird sp�ter bef�llt
			au16Stack[u8SP++] = J_IF;
			add(0);

			trim();
			noend();
		}
		else if (strncmp(str, "elsif", 5) == 0) {
			if (au16Stack[u8SP - 1] == J_IF || au16Stack[u8SP - 1] == J_ELSIF) {
				OVERFLOW_IF(1);
				add(C_JMP);    //An das Ende der IF Klausel springen
				au16IfEnd[u8IfEndPos++] = u16PC;    //Adresse wird sp�ter geschrieben
				add(0);

				--u8SP;    //J_IF entfernen
				pu16Dst[au16Stack[--u8SP]] = u16PC;    //Sprungadresse vom IF setzen

				str += 5;
				nospace();
				trim();

				expr();
				add(C_JMPF);

				au16Stack[u8SP++] = u16PC;
				au16Stack[u8SP++] = J_ELSIF;
				add(0);

				trim();
				noend();
			}
			else {
				throw ScriptException(ERROR_ELSIF);
			}
		}
		else if (strncmp(str, "else", 4) == 0) {
			if (au16Stack[u8SP - 1] == J_IF || au16Stack[u8SP - 1] == J_ELSIF) {
				OVERFLOW_IF(1);
				add(C_JMP);
				au16IfEnd[u8IfEndPos++] = u16PC;
				add(0);

				--u8SP;
				pu16Dst[au16Stack[--u8SP]] = u16PC;

				au16Stack[u8SP++] = J_ELSE;

				str += 4;
				trim();
				noend();
			}
			else if (au16Stack[u8SP - 1] == J_WHILE) {
				u8SP--;
				pu16Dst[au16Stack[--u8SP]] = u16PC + 2;
				add(C_JMP);
				add(au16Stack[u8SP - 1]);

				au16Stack[u8SP++] = u16PC;
				au16Stack[u8SP++] = J_WHILEELSE;

				str += 4;
				trim();
				noend();
			}
			else {
				throw ScriptException(ERROR_ELSIF);
			}
		}
		else if (strncmp(str, "for", 3) == 0) {
			OVERFLOW_STACK(3);

			str += 3;
			nospace();
			trim();
			if (expr()) {
				add(C_PULL);
			}

			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();

			au16Stack[u8SP++] = u16PC;
			if (expr()) {
				add(C_JMPF);
				au16Stack[u8SP++] = u16PC;
				au16Stack[u8SP++] = J_FOR;
				add(0);
			}
			else {
				throw ScriptException(ERROR_SYNTAX);
			}

			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();

			acForTemp[u8ForTempPos++] = str;
			while (*str != '\0' && *str != '\n') {
				str++;
			}
		}
		else if (strncmp(str, "while", 5) == 0) {
			OVERFLOW_STACK(3);

			str += 5;
			nospace();
			trim();

			au16Stack[u8SP++] = u16PC;
			expr();
			add(C_JMPF);
			au16Stack[u8SP++] = u16PC;
			au16Stack[u8SP++] = J_WHILE;
			add(0);

			trim();
			noend();
		}
		else if (strncmp(str, "end", 3) == 0) {
			uint8_t u8CMP = au16Stack[--u8SP];

			switch (u8CMP) {
				case J_WHILEELSE: {
					uint8_t u8Tmp2 = au16Stack[--u8SP];
					uint8_t u8Tmp = au16Stack[--u8SP];

					for (uint8_t j = u8BreakPos; j > 0; j--) {
						if (au16Break[j - 1] >= u8Tmp && au16Break[j - 1] <= u8Tmp2) {
							pu16Dst[au16Break[j - 1]] = u16PC;
							u8BreakPos--;
						}
						else {
							break;
						}
					}

					for (uint8_t j = u8ContinuePos; j > 0; j--) {
						if (au16Continue[j - 1] >= u8Tmp && au16Continue[j - 1] <= u8Tmp2) {
							pu16Dst[au16Continue[j - 1]] = u8Tmp;
							u8ContinuePos--;
						}
						else {
							break;
						}
					}
				}
				break;

				case J_FOR:
				case J_LOOP:
				case J_WHILE: {
					if (u8CMP == J_FOR) {
						char* tmp = str;
						str = acForTemp[--u8ForTempPos];
						if (expr()) {
							add(C_PULL);
						}
						str = tmp;
					}

					if (u8CMP == J_WHILE || u8CMP == J_FOR) {
						uint16_t u16addr = au16Stack[--u8SP];
						pu16Dst[u16addr] = u16PC + 2;
					}
					uint8_t u8Tmp = au16Stack[--u8SP];
					add(C_JMP);
					add(u8Tmp);

					for (uint8_t j = u8BreakPos; j > 0; j--) {
						if (au16Break[j - 1] >= u8Tmp) {
							pu16Dst[au16Break[j - 1]] = u16PC;
							u8BreakPos--;
						}
						else {
							break;
						}
					}

					for (uint8_t j = u8ContinuePos; j > 0; j--) {
						if (au16Continue[j - 1] >= u8Tmp) {
							pu16Dst[au16Continue[j - 1]] = u8Tmp;
							u8ContinuePos--;
						}
						else {
							break;
						}
					}
				}
				break;

				case J_ELSIF:
				case J_IF:
				case J_ELSE: {
					if (u8CMP != J_ELSE) {
						pu16Dst[au16Stack[--u8SP]] = u16PC;
					}
					uint8_t u8Tmp = au16Stack[--u8SP];

					for (uint8_t j = u8IfEndPos; j > 0; j--) {
						if (au16IfEnd[j - 1] >= u8Tmp) {
							pu16Dst[au16IfEnd[j - 1]] = u16PC;
							u8IfEndPos--;
						}
						else {
							break;
						}
					}
				}
				break;

				default:
					throw ScriptException(ERRORCODE::ERROR_UNKNOWN);
				break;
			}

			trim_e(3);
		}
		else if (strncmp(str, "loop", 4) == 0) {
			OVERFLOW_STACK(2);
			au16Stack[u8SP++] = u16PC;
			au16Stack[u8SP++] = J_LOOP;
			trim_e(4);
		}
		else if (strncmp(str, "continue", 8) == 0) {
			OVERFLOW_CONTINUE(1);
			add(C_JMP);
			au16Continue[u8ContinuePos++] = u16PC;
			add(0);
			trim_e(8);
		}
		else if (strncmp(str, "break", 5) == 0) {
			OVERFLOW_BREAK(1);
			add(C_JMP);
			au16Break[u8BreakPos++] = u16PC;
			add(0);
			trim_e(5);
		}
		else if (strncmp(str, "cout", 4) == 0) {
			str += 4;
			nospace();
			trim();

			expr();
			add(C_COUT);
		}
		else if (strncmp(str, "var", 3) == 0) {
			str += 3;
			nospace();
			trim();

			while (!(*str == '\0' || *str == '\n')) {
				if (!(*str == ',' || (*str >= '0' && *str <= '9') || (*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z'))) {
					throw ScriptException(ERROR_VAR);
				}
				str++;
			}
		}
		else if (strncmp(str, "set", 3) == 0) {
			str += 3;
			nospace();
			trim();
			expr();

			trim();
			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();
			expr();

			trim();
			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();
			expr();

			trim();
			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();
			expr();

			trim();
			if (*str != ';') {
				throw ScriptException(ERROR_SYNTAX);
			}
			str++;
			trim();
			expr();

			add(C_LED);
			noend();
		}
		else if (strncmp(str, "clear", 5) == 0) {
			str += 5;
			trim();
			add(C_CLEAR);
			noend();
		}
		else if (strncmp(str, "sleep", 5) == 0) {
			str += 5;
			nospace();
			trim();
			expr();
			add(C_SLEEP);
			noend();
		}
		else if (strncmp(str, "output", 6) == 0) {
			str += 6;
			trim();
			add(C_OUTPUT);
			noend();
		}
		else {
			trim();
			expr();
			add(C_PULL);
			noend();
		}
	} while (*str != '\0');

	if (u8SP > 0 || u8BreakPos > 0 || u8ContinuePos > 0 || u8IfEndPos > 0) {
		throw ScriptException(ERROR_BRACKET);
	}
}

int8_t Script::varpos(char* name, uint8_t p) {
	uint8_t u8Count = 0;
	char* str_l = pCode;

	trim(&str_l);    //Alle Leerzeichen entfernen

	do {
		if (strncmp(str_l, "var", 3) == 0) {    //DIese Zeile enth�#lt Variablendefinitionen?
			str_l += 3;
			nospace(str_l);
			trim(&str_l);

			uint8_t i = strcnt(str_l, ',', '\n') + 1;    //Variablen z�hlen

			while (i > 0) {
				int8_t j = charfind(str_l, ',', '\n');    //Zeilenende suchen
				if (j == -1) {
					j = charfind(str_l, '\n', '\0');
				}

				if (strncmp(str_l, name, p) == 0) {    //Zeichen bis Zeilenende vergleichen
					return u8Count;    //Ist dies die variable?
				}

				if (u8Count++ >= STACKSIZE) {    //Z�hler + 1
					throw ScriptException(ERRORCODE::ERROR_OVERFLOW);
				}

				str_l += j;    //Zum Zeilenende springen

				if (j == '\0') {
					throw ScriptException(ERRORCODE::ERROR_VAR);
				}
				str_l++;

				i--;

			}
		}
		else if (*str_l == '\n') {
			str_l++;
		}
		else {    //Keine variablendefinitionen
			throw ScriptException(ERRORCODE::ERROR_VAR);
		}
	} while (str_l != NULL);    //EOF? abbrechen

	throw ScriptException(ERRORCODE::ERROR_VAR);    //Variable NICHT gefunden!
}

uint8_t Script::loadElement() {
	uint8_t u8Result = DataTypes::Def;

	if (*str == '(') {
		str++;
		expr();
		str++;

		u8Result = DataTypes::Expr;
	}
	else if (isChar(*str)) {
		char* tmp = str;
		uint8_t u8Size = 0;
		while (isChar(*str)) {
			u8Size++;
			str++;
		}
		uint8_t u8Var = varpos(tmp, u8Size);

		trim(&str);
		if ((*str == '=' && *(str + 1) != '=') || strncmp(str, "+=", 2) == 0 || strncmp(str, "-=", 2) == 0 || strncmp(str, "/=", 2) == 0 || strncmp(str, "*=", 2) == 0
			|| strncmp(str, "%=", 2) == 0) {
			add(C_LDK);
			add(u8Var);
			u8Result = DataTypes::Adress;
		}
		else {
			add(C_LDI);
			add(u8Var);
			u8Result = DataTypes::Var;
		}

	}
	else if (isNum(*str)) {
		uint32_t sum = 0;

		while (isNum(*str)) {
			sum *= 10;
			sum += *str - '0';
			str++;
		}

		if (sum > 0) {
			u8Result = DataTypes::ConstGrNull;
		}
		else {
			u8Result = DataTypes::ConstNull;
		}

		add(C_LDK);
		add(sum);
	}
	else {
		throw ScriptException(ERRORCODE::ERROR_SYNTAX);
	}

	return u8Result;
}

bool Script::expr() {
	bool bBegin = true;
	bool bSetFalse = false;
	bool bSetTrue = false;
	uint16_t u16ExpStart = u16PC;
	uint16_t u16JumpCounter = 0;
	uint16_t u16JumpData[100];

	loadElement();

	while (*str != '\0' && *str != '\n' && *str != ')' && *str != ';') {
		bBegin = false;
		trim();

		if (strncmp(str, "+=", 2) == 0 || strncmp(str, "-=", 2) == 0 || strncmp(str, "*=", 2) == 0 || strncmp(str, "/=", 2) == 0 || strncmp(str, "%=", 2) == 0) {
			add(C_LDI);
			add(u16ByteCode[u16PC - 2]);
			char op = *str;
			str += 2;
			trim();
			expr();
			switch (op) {
				case '+':
					add(C_ADD);
				break;

				case '-':
					add(C_SUB);
				break;

				case '*':
					add(C_MUL);
				break;

				case '/':
					add(C_DIV);
				break;

				case '%':
					add(C_MOD);
				break;

				default:
					throw ScriptException(ERRORCODE::ERROR_SYNTAX);
				break;
			}
			add(C_PULLS);
		}
		else if (strncmp(str, "&&", 2) == 0) {
			str += 2;
			trim();
			if (loadElement() == DataTypes::ConstNull) {
				bSetFalse = true;
			}
			add(C_AND);
			u16JumpData[u16JumpCounter++] = u16PC;
			add(0);
		}
		else if (strncmp(str, "||", 2) == 0) {
			str += 2;
			trim();
			if (loadElement() == DataTypes::ConstGrNull) {
				bSetTrue = true;
			}
			add(C_OR);
			u16JumpData[u16JumpCounter++] = u16PC;
			add(0);
		}
		else if (strncmp(str, "!=", 2) == 0) {
			str += 2;
			trim();
			loadElement();
			add(C_NEQ);
			u16JumpData[u16JumpCounter++] = u16PC;
			add(0);
		}
		else if (strncmp(str, "==", 2) == 0) {
			str += 2;
			trim();
			loadElement();
			add(C_EQ);
			u16JumpData[u16JumpCounter++] = u16PC;
			add(0);
		}
		else if (*str == '&') {
			str++;
			trim();
			loadElement();
			add(C_ANDI);
		}
		else if (*str == '|') {
			str++;
			trim();
			loadElement();
			add(C_ORI);
		}
		else if (*str == '^') {
			str++;
			trim();
			loadElement();
			add(C_XORI);
		}
		else if (*str == '=') {
			str++;
			trim();
			expr();
			add(C_PULLS);
		}
		else if (*str == '-' || *str == '+') {
			char op1 = *str;
			str++;
			trim();
			loadElement();
			trim();

			while (charcmp(*str, (char*) "*/%")) {
				char op2 = *str;
				str++;
				trim();
				loadElement();
				trim();

				switch (op2) {
					case '*':
						add(C_MUL);
					break;

					case '/':
						add(C_DIV);
					break;

					case '%':
						add(C_MOD);
					break;

					default:
						throw ScriptException(ERRORCODE::ERROR_SYNTAX);
					break;
				}
			}

			switch (op1) {
				case '+':
					add(C_ADD);
				break;

				case '-':
					add(C_SUB);
				break;

				default:
					throw ScriptException(ERRORCODE::ERROR_SYNTAX);
				break;
			}
		}
		else if (*str == '*' || *str == '/' || *str == '%') {
			char op = *str;
			str++;
			trim();
			DataTypes dt = (DataTypes) loadElement();
			/*if (dt == DataTypes::ConstGrNull || dt == DataTypes::ConstNull) {
			 u16PC -= 2;
			 }*/

			switch (op) {
				case '*':
					add(C_MUL);
				break;

				case '/':
					add(C_DIV);
				break;

				case '%':
					add(C_MOD);
				break;

				default:
					throw ScriptException(ERRORCODE::ERROR_SYNTAX);
				break;
			}
		}
		else if (*str == '\n' || *str == ';') {
			break;
		}
		else {
			throw ScriptException(ERRORCODE::ERROR_SYNTAX);
		}
	}

	while (u16JumpCounter > 0) {
		u16JumpCounter--;
		u16ByteCode[u16JumpData[u16JumpCounter]] = u16PC;
	}

	if (bSetFalse) {
		u16PC = u16ExpStart;
		add(C_LDK);
		add(0);
	}

	if (bSetTrue) {
		u16PC = u16ExpStart;
		add(C_LDK);
		add(1);
	}

	return !bBegin;
}
