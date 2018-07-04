#pragma once
#include "parser.h"

using namespace std;

void exec_token(const Token &token, int &next_line, char *memory, int &memoryPos) {
	int address;
	short *n1;
	int *n2;
	size_t n3;
	short as;
	char *s;
	switch (token.op) {
		case 1:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] + regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] + token.operand[2];
			++next_line;
			break;
		case 2:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) + static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) + static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 3:
			regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) + static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 4:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] - regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] - token.operand[2];
			++next_line;
			break;
		case 5:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) - static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) - static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 6:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] * regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] * token.operand[2];
			++next_line;
			break;
		case 7:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) * static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) * static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 8:
			if (token.operand_kind[1] == operandKind::reg) {
				long long ret = static_cast<long long>(regi[token.operand[0]]) * static_cast<long long>(regi[token.operand[1]]);
				regi[32] = ret / (pow(2, 32));
				regi[33] = ret % static_cast<long long>(pow(2, 32));
			}
			else {
				long long ret = static_cast<long long>(regi[token.operand[0]]) * static_cast<long long>(token.operand[1]);
				regi[32] = ret / static_cast<long long>(pow(2, 32));
				regi[33] = ret % static_cast<long long>(pow(2, 32));
			}
			++next_line;
			break;
		case 9:
			if (token.operand_kind[1] == operandKind::reg) {
				unsigned long long ret = static_cast<unsigned long long>(regi[token.operand[0]]) * static_cast<unsigned long long>(regi[token.operand[1]]);
				regi[32] = ret / (pow(2, 32));
				regi[33] = ret % static_cast<unsigned long long>(pow(2, 32));
			}
			else {
				unsigned long long ret = static_cast<unsigned long long>(regi[token.operand[0]]) * static_cast<unsigned long long>(token.operand[1]);
				regi[32] = ret / (pow(2, 32));
				regi[33] = ret % static_cast<unsigned long long>(pow(2, 32));
			}
			++next_line;
			break;
		case 10:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] / regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] / token.operand[2];
			++next_line;
			break;
		case 11:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) / static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) / static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 12:
			regi[32] = regi[token.operand[0]] % regi[token.operand[1]];
			regi[33] = regi[token.operand[0]] / regi[token.operand[1]];
			++next_line;
			break;
		case 13:
			regi[32] = static_cast<size_t>(regi[token.operand[0]]) % static_cast<size_t>(regi[token.operand[1]]);
			regi[33] = static_cast<size_t>(regi[token.operand[0]]) / static_cast<size_t>(regi[token.operand[1]]);
			++next_line;
			break;
		case 14:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] ^ regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] ^ token.operand[2];
			++next_line;
			break;
		case 15:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) ^ static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) ^ static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 16:
			regi[token.operand[0]] = -regi[token.operand[1]];
			++next_line;
			break;
		case 17:
			regi[token.operand[0]] = ~(static_cast<size_t>(regi[token.operand[1]]));
			++next_line;
			break;
		case 18:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] % regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] % token.operand[2];
			++next_line;
			break;
		case 19:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) % static_cast<size_t>(regi[token.operand[2]]);
			else
				regi[token.operand[0]] = static_cast<size_t>(regi[token.operand[1]]) % static_cast<size_t>(token.operand[2]);
			++next_line;
			break;
		case 20:
			regi[token.operand[0]] = token.operand[1];
			++next_line;
			break;
		case 21:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] == regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] == token.operand[2];
			++next_line;
			break;
		case 22:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] >= regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] >= token.operand[2];
			++next_line;
			break;
		case 23:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] > regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] > token.operand[2];
			++next_line;
			break;
		case 24:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] <= regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] <= token.operand[2];
			++next_line;
			break;
		case 25:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] < regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] < token.operand[2];
			++next_line;
			break;
		case 26:
			if (token.operand_kind[2] == operandKind::reg)
				regi[token.operand[0]] = regi[token.operand[1]] != regi[token.operand[2]];
			else
				regi[token.operand[0]] = regi[token.operand[1]] != token.operand[2];
			++next_line;
			break;
		case 27:
			next_line = token.operand[0];
			break;
		case 28:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] == regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] == token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 29:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] != regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] != token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 30:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] >= regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] >= token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 31:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] <= regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] <= token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 32:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] > regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] > token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 33:
			if (token.operand_kind[1] == operandKind::reg) {
				if (regi[token.operand[0]] < regi[token.operand[1]])
					next_line = token.operand[2];
				else ++next_line;
			}
			else {
				if (regi[token.operand[0]] < token.operand[1])
					next_line = token.operand[2];
				else ++next_line;
			}
			break;
		case 34:
			if (regi[token.operand[0]] == 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 35:
			if (regi[token.operand[0]] != 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 36:
			if (regi[token.operand[0]] <= 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 37:
			if (regi[token.operand[0]] >= 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 38:
			if (regi[token.operand[0]] > 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 39:
			if (regi[token.operand[0]] < 0)
				next_line = token.operand[1];
			else ++next_line;
			break;
		case 40:
			next_line = token.operand[0];
			break;
		case 41:
			next_line = regi[token.operand[0]];
			break;
		case 42:
			regi[31] = next_line + 1;
			next_line = token.operand[0];
			break;
		case 43:
			regi[31] = next_line + 1;
			next_line = regi[token.operand[0]];
			break;
		case 44:
			regi[token.operand[0]] = token.operand[1];
			++next_line;
			break;
		case 45:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			memset(&regi[token.operand[0]], 0, 1);
			regi[token.operand[0]] = memory[address];
			++next_line;
			break;
		case 46:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			n1 = reinterpret_cast<short*>(&memory[address]);
			memset(&regi[token.operand[0]], 0, 1);
			regi[token.operand[0]] = *n1;
			++next_line;
			break;
		case 47:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			n2 = reinterpret_cast<int *>(&memory[address]);
			regi[token.operand[0]] = *n2;
			++next_line;
			break;
		case 48:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			n3 = regi[token.operand[0]];
			memory[address] = n3;
			++next_line;
			break;
		case 49:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			as = regi[token.operand[0]];
			s = reinterpret_cast<char *>(&as);
			memory[address] = s[0];
			memory[address + 1] = s[1];
			++next_line;
			break;
		case 50:
			if (token.operand_kind[1] == operandKind::num)
				address = regi[token.operand[2]] + token.operand[1];
			else if (token.operand_kind[1] == operandKind::reg)
				address = regi[token.operand[1]];
			else address = token.operand[1];
			s = reinterpret_cast<char *>(&regi[token.operand[0]]);
			memory[address] = s[0];
			memory[address + 1] = s[1];
			memory[address + 2] = s[2];
			memory[address + 3] = s[3];
			++next_line;
			break;
		case 51:
			regi[token.operand[0]] = regi[token.operand[1]];
			++next_line;
			break;
		case 52:
			regi[token.operand[0]] = regi[32];
			++next_line;
			break;
		case 53:
			regi[token.operand[0]] = regi[33];
			++next_line;
			break;
		case 54:
			++next_line;
			break;
		case 55:
			if (regi[2] == 1) {
				int a = regi[4];
				cout << regi[4];
			}
			else if (regi[2] == 4) {
				printf("%s", memory + regi[4]); 
				/*for (int i = regi[4]; ; ++i) {
					if (memory[i] == '\0') break;
					cout << memory[i];
				}*/
			}
			else if (regi[2] == 5) {
				cin >> regi[2];
			}
			else if (regi[2] == 8) {
				string str;
				getline(cin, str);
				regi[5] = str.length() + 1;
				regi[4] = memoryPos;
				for (int i = 0; i < str.length(); ++i, ++memoryPos) 
					memory[memoryPos] = str[i];
				memory[memoryPos++] = '\0';
			}
			else if (regi[2] == 9) {
				regi[2] = memoryPos;
				memoryPos += regi[4];
			}
			else if (regi[2] == 10) {
				exit(0);
			}
			else if (regi[2] == 17) {
				exit(0);
			}
			++next_line;
			break;
		
	}



}