#pragma once
#include "token.h"

using namespace std;

int regi[35];
bool regi_lock[35];
char memory[4 * 1024 * 1024];
int memPos;


class Parser {
public:
	map<string, int> label_text;
	map<string, int> label_data;
	map<string, int> op_sign;
	map<string, int> reg_num;
	vector<Token> operation;
	int opCnt;

private:
	inline string get_data_operation(const string &t, bool &isLabel, int &i){
		string ret;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t'); ++i);
		for (; i < t.length() && t[i] != ' ' && t[i] != ':' && t[i] != '#' && t[i] != '\t'; ++i) ret += t[i];
		if (t[i] == ':') 
			isLabel = true;
		else isLabel = false;
		++i;
		return ret;
	}

	inline int get_int(const string &t, int &i) {
		int n = 0;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t' || t[i] == ','); ++i);
		bool minus = false;
		if (t[i] == '-') {
			minus = true;
			++i;
		}
		for (; i < t.length() && t[i] <= '9' && t[i] >= '0'; ++i)
			n = 10 * n + t[i] - '0';
		if (minus)
			n *= (-1);
		return n;
	}

	inline short get_short(const string &t, int &i) {
		short n = 0;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t' || t[i] == ','); ++i);
		bool minus = false;
		if (t[i] == '-')
			minus = true;
		for (; i < t.length() && t[i] <= '9' && t[i] >= '0'; ++i)
			n = 10 * n + t[i] - '0';
		if (minus)
			n *= (-1);
		return n;
	}

	inline vector<char> get_char(const string &t, int &i) {
		vector<char> ret;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t'); ++i);
		while (i < t.length()) {
			if (t[i] == '\\') {
				++i;
				if (t[i] == 'a') 
					ret.push_back('\a');
				else if (t[i] == 'b')
					ret.push_back('\b');
				else if (t[i] == 'f')
					ret.push_back('\f');
				else if (t[i] == 'n')
					ret.push_back('\n');
				else if (t[i] == 'r')
					ret.push_back('\r');
				else if (t[i] == 't')
					ret.push_back('\t');
				else if (t[i] == 'v')
					ret.push_back('\v');
				else if (t[i] == '\\')
					ret.push_back('\\');
				else if (t[i] == '\'')
					ret.push_back('\'');
				else if (t[i] == '\"')
					ret.push_back('\"');
				else if (t[i] == '\?')
					ret.push_back('\?');
				else if (t[i] == '0')
					ret.push_back('\0');
				else if (t[i] == 'd' && t[i + 1] == 'd' && t[i + 2] == 'd')
					ret.push_back('\ddd');
			}
			else
				ret.push_back(t[i]);
			++i;
			for (; i < t.length() && (t[i] == ' ' || t[i] == '\t' || t[i] == ','); ++i);
		}
		return ret;
	}

	inline string get_string(const string &t, int &i) {
		string ret;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t'); ++i);
		if (t[i] == '\"') ++i;
		for (; i < t.length(); ++i) {
			if (t[i] == '\"') 
				break;
			if (t[i] == '\\') {
				++i;
				if (t[i] == 'a')
					ret += '\a';
				else if (t[i] == 'b')
					ret += '\b';
				else if (t[i] == 'f')
					ret += '\f';
				else if (t[i] == 'n')
					ret += '\n';
				else if (t[i] == 'r')
					ret += '\r';
				else if (t[i] == 't')
					ret += '\t';
				else if (t[i] == 'v')
					ret += '\v';
				else if (t[i] == '\\')
					ret += '\\';
				else if (t[i] == '\'')
					ret += '\'';
				else if (t[i] == '\"')
					ret += '\"';
				else if (t[i] == '\?')
					ret += '\?';
				else if (t[i] == '0')
					ret += '\0';
				
			}
			else
				ret += t[i];

		}
		return ret;

	}

	void deal_with_data(string &tmp, bool &isData) {
		bool isLabel;
		int pos = 0;
		string str = get_data_operation(tmp, isLabel, pos);
		if (isLabel) {
			label_data[str] = memPos;
			return;
		}

		if (str[0] == '.') {
			if (str == ".align") {
				int n = get_int(tmp, pos);
				int d = 1 << n, m = 0;
				while (m < memPos)
					m += d;
				memPos = m;
			}

			else if (str == ".ascii") {
				string ret = get_string(tmp, pos);
				for (int i = 0; i < ret.length(); ++i, ++memPos)
					memory[memPos] = ret[i];

			}

			else if (str == ".asciiz") {
				string ret = get_string(tmp, pos);
				for (int i = 0; i < ret.length(); ++i, ++memPos)
					memory[memPos] = ret[i];
				memory[memPos++] = '\0';
			}

			else if (str == ".byte") {
				vector<char> ret = get_char(tmp, pos);
				for (int i = 0; i < ret.size(); ++i, ++memPos)
					memory[memPos] = ret[i];
			}

			else if (str == ".half") {
				vector<short> ret;
				while (pos < tmp.length()) {
					short n = get_short(tmp, pos);
					ret.push_back(n);
				}
				for (int i = 0; i < ret.size(); ++i) {
					short a = ret[i];
					char *s = reinterpret_cast<char*> (&a);
					memory[memPos++] = s[0];
					memory[memPos++] = s[1];
				}
			}

			else if (str == ".word") {
				vector<int> ret;
				while (pos < tmp.length()) {
					int n = get_int(tmp, pos);
					ret.push_back(n);
				}
				for (int i = 0; i < ret.size(); ++i) {
					int a = ret[i];
					char *s = reinterpret_cast<char*> (&a);
					memory[memPos++] = s[0];
					memory[memPos++] = s[1];
					memory[memPos++] = s[2];
					memory[memPos++] = s[3];
				}
			}

			else if (str == ".space") {
				int n = get_int(tmp, pos);
				memPos += n;
			}

			else if (str == ".data") {
				isData = true;
			}

			else if (str == ".text") {
				isData = false;
			}

		}
		else if (tmp == "")
			return;

	}

	bool deal_with_text_label(string &tmp, bool &isData) {
		string ret;
		int i;
		for (i = 0; i < tmp.length() && (tmp[i] == ' ' || tmp[i] == '\t'); ++i);
		for (; i < tmp.length() && tmp[i] != ' ' && tmp[i] != ':' && tmp[i] != '#' && tmp[i] != '\t'; ++i) 
			ret += tmp[i];

		if (tmp[i] == ':') {
			label_text[ret] = opCnt;
			return true;
		}
		if (ret[0] == '.') {
			if (ret == ".data")
				isData = true;
			else if (ret == ".text")
				isData = false;
			return true;
		}
		if (ret == "")
			return true;

		return false;

	}

	inline string get_op(const string &t, int &i) {
		string ret;
		for (; i < t.length() && (t[i] == ' ' || t[i] == '\t' || t[i] == ',' || t[i] == '('); ++i);
		for (; i < t.length() && t[i] != ' ' && t[i] != ':' && t[i] != '#' && t[i] != '\t' && t[i] != ')' && t[i] != ',' && t[i] != '('; ++i) ret += t[i];
		return ret;
	}


public:
	Parser(): opCnt(0){
		memPos = 0;
		memset(memory, 0, sizeof(memory));
		memset(regi, 0, sizeof(regi));
		memset(regi_lock, false, sizeof(regi_lock));
		regi[29] = 4 * 1024 * 1024 - 1;

		op_sign["add"] = 1;
		op_sign["addu"] = 2;
		op_sign["addiu"] = 3;
		op_sign["sub"] = 4;
		op_sign["subu"] = 5;
		op_sign["mul"] = 6;
		op_sign["mulu"] = 7;
		op_sign["mul_"] = 8;
		op_sign["mulu_"] = 9;
		op_sign["div"] = 10;
		op_sign["divu"] = 11;
		op_sign["div_"] = 12;
		op_sign["divu_"] = 13;
		op_sign["xor"] = 14;
		op_sign["xoru"] = 15;
		op_sign["neg"] = 16;
		op_sign["negu"] = 17;
		op_sign["rem"] = 18;
		op_sign["remu"] = 19;
		op_sign["li"] = 20;
		op_sign["seq"] = 21;
		op_sign["sge"] = 22;
		op_sign["sgt"] = 23;
		op_sign["sle"] = 24;
		op_sign["slt"] = 25;
		op_sign["sne"] = 26;
		op_sign["b"] = 27;
		op_sign["beq"] = 28;
		op_sign["bne"] = 29;
		op_sign["bge"] = 30;
		op_sign["ble"] = 31;
		op_sign["bgt"] = 32;
		op_sign["blt"] = 33;
		op_sign["beqz"] = 34;
		op_sign["bnez"] = 35;
		op_sign["blez"] = 36;
		op_sign["bgez"] = 37;
		op_sign["bgtz"] = 38;
		op_sign["bltz"] = 39;
		op_sign["j"] = 40;
		op_sign["jr"] = 41;
		op_sign["jal"] = 42;
		op_sign["jalr"] = 43;
		op_sign["la"] = 44;
		op_sign["lb"] = 45;
		op_sign["lh"] = 46;
		op_sign["lw"] = 47;
		op_sign["sb"] = 48;
		op_sign["sh"] = 49;
		op_sign["sw"] = 50;
		op_sign["move"] = 51;
		op_sign["mfhi"] = 52;
		op_sign["mflo"] = 53;
		op_sign["nop"] = 54;
		op_sign["syscall"] = 55;

		reg_num["$zero"] = 0;
		reg_num["$0"] = 0;
		reg_num["$at"] = 1;
		reg_num["$1"] = 1;
		reg_num["$v0"] = 2;
		reg_num["$2"] = 2;
		reg_num["$v1"] = 3;
		reg_num["$3"] = 3;
		reg_num["$a0"] = 4;
		reg_num["$4"] = 4;
		reg_num["$a1"] = 5;
		reg_num["$5"] = 5;
		reg_num["$a2"] = 6;
		reg_num["$6"] = 6;
		reg_num["$a3"] = 7;
		reg_num["$7"] = 7;
		reg_num["$t0"] = 8;
		reg_num["$8"] = 8;
		reg_num["$t1"] = 9;
		reg_num["$9"] = 9;
		reg_num["$t2"] = 10;
		reg_num["$10"] = 10;
		reg_num["$t3"] = 11;
		reg_num["$11"] = 11;
		reg_num["$t4"] = 12;
		reg_num["$12"] = 12;
		reg_num["$t5"] = 13;
		reg_num["$13"] = 13;
		reg_num["$t6"] = 14;
		reg_num["$14"] = 14;
		reg_num["$t7"] = 15;
		reg_num["$15"] = 15;
		reg_num["$s0"] = 16;
		reg_num["$16"] = 16;
		reg_num["$s1"] = 17;
		reg_num["$17"] = 17;
		reg_num["$s2"] = 18;
		reg_num["$18"] = 18;
		reg_num["$s3"] = 19;
		reg_num["$19"] = 19;
		reg_num["$s4"] = 20;
		reg_num["$20"] = 20;
		reg_num["$s5"] = 21;
		reg_num["$21"] = 21;
		reg_num["$s6"] = 22;
		reg_num["$22"] = 22;
		reg_num["$s7"] = 23;
		reg_num["$23"] = 23;
		reg_num["$t8"] = 24;
		reg_num["$24"] = 24;
		reg_num["$t9"] = 25;
		reg_num["$25"] = 25;
		reg_num["$gp"] = 28;
		reg_num["$28"] = 28;
		reg_num["$sp"] = 29;
		reg_num["$29"] = 29;
		reg_num["$fp"] = 30;
		reg_num["$30"] = 30;
		reg_num["$ra"] = 31;
		reg_num["$31"] = 31;
		reg_num["$hi"] = 32;
		reg_num["$lo"] = 33;
		reg_num["$pc"] = 34;

	}

	~Parser() {
		label_data.clear();
		label_text.clear();
	}

	void parse(istream &inFile) {
		vector<string> First_read;
		string tmp;

		//next_is_data
		bool isData = true;

		while (getline(inFile, tmp)) {
			if (isData) 
				deal_with_data(tmp, isData);
			else {
				if (deal_with_text_label(tmp, isData)) continue;
				First_read.push_back(tmp);
				++opCnt;
			}
		}

		for (auto tmp : First_read) {
			int pos = 0;
			string t = get_op(tmp, pos);
			if (t == "mul" || t == "mulu" || t == "div" || t == "divu") {
				int p = pos;
				get_op(tmp, p);
				get_op(tmp, p);
				string s = get_op(tmp, p);
				if (s == "") 
					t += '_';
			}
			Token token;
			token.op = op_sign[t];

			for (int i = 0; i < 3; ++i) {
				string op = get_op(tmp, pos);
				if (op[0] == '$') {
					token.operand_kind[i] = operandKind::reg;
					token.operand[i] = reg_num[op];
				}

				else if (op == "") {
					token.operand_kind[i] = operandKind::non;
					token.operand[i] = 0;
				}
				
				else if (op[0] == '-' || (op[0] <= '9' && op[0] >= '0')) {
					int p = 0;
					int n = get_int(op, p);
					token.operand_kind[i] = operandKind::num;
					token.operand[i] = n;
				}
				
				else {
					if (label_data.find(op) != label_data.end()) {
						token.operand_kind[i] = operandKind::lab_data;
						token.operand[i] = label_data[op];
					}
					else if (label_text.find(op) != label_text.end()) {
						token.operand_kind[i] = operandKind::lab_text;
						token.operand[i] = label_text[op];
					}
				}

			}
			
			operation.push_back(token);
			//cout << token << '\n';
		}



	}


};

