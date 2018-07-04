#pragma once
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

enum operandKind {
	non, reg, num, lab_data, lab_text
};


struct Token {
	operandKind operand_kind[3];
	int op;
	int operand[3];

	Token(){
		operand_kind[0] = operand_kind[1] = operand_kind[2] = operandKind::non;
		op = 0;
	}
};

ostream & operator<<(ostream &out, const Token & t) {
	out << t.op << ' ';
	for (int i = 0; i < 3; ++i) {
		if (t.operand_kind[i] != non) {
			if (t.operand_kind[i] == operandKind::reg)
				out << '$';
			out << t.operand[i] << ' ';
		}
	}
	out << '\n';
	return out;
}