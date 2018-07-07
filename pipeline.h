#pragma once
#include "command.h"


int pc, cyc = 0, cnt = 0, wait = 0, jump = -1;
Token if_id;
int id_exec[7];   
int exec_mem[7];
int mem_wb[7]; 
int line_num;
bool if_success; //control hazard
bool id_stall = false; //data hazard
bool mem_stall = false; //structure hazard
bool ed = false; //loop end
int edcnt = 4;
map<int, int> predict_value;
ofstream out("ans1.txt");
/*
[5]:
this line's pc;
[6]:
offset
*/

/*id_exec[0]:
case -1: stall
case 0: common			1: regi_num(write_address); 2: num1; 3:num2(if exist); 4:op;
case 1:	mul_,div_		1: num1; 2: num2; 3: op; 
case 2:	li,mov,mfh,mfl	1: regi_num; 2: num; 
case 3:	b,j,jr			1: address;
case 4: jal, jalr		1: address; 2: next_line;
case 5: conditionalJump 1: address; 2: num1; 3:num2; 4:op;
case 6: load			1: regi_num; 2: address_1; 3: address_2(if exist, otherwise 0); 4:op;
case 7: store			1: address_1; 2: address_2; 3: num; 4: op;
case 8: nop				
case 9: syscall			1: num($v0); 2: num1($a0); 3: num2($a1 / memPos);
*/

/*exec_mem[0]:
case -1: stall
case 0:					1: regi_num; 2: result;
case 1:					1: result1($hi); 2: result2($lo)
case 2:					1: regi_num; 2: num;
case 3:					1: address;
case 4:					1: address; 2: next_line;
case 5:					1: address; 2: result(T/F);
case 6:					1: regi_num; 2: address; 3:len(1/2/4);
case 7:					1: address; 2: len(1/2/4);
case 8:					
case 9:					1: num($v0); 2: num1($a0); 3: num2($a1 / memPos);
*/

/*mem_wb[0]:
case -1: stall
case 0:					1: regi_num; 2: result;
case 1:					1: result1($hi); 2: result2($lo)
case 2:					1: regi_num; 2: num;
case 3:					1: address;
case 4:					1: address; 2: next_line;
case 5:					1: address; 2: result(T/F);
case 6:	load			1: regi_num; 2: num
case 7:	store			
case 8:
case 9:	syscall			1: num($v0); 2: num1($a0); 3: num2($a1); 4: type;
*/

void Instruction_Fetch(const Parser &parser) {
	/*if (regi_lock[34]) {
		if_success = false;
		return;
	}*/

	if (id_stall) {
		return;
	}

	if (mem_stall) {
		if_success = false;
		mem_stall = false;
		return;
	}

	if (jump >= 0) {
		pc = jump;
		jump = -1;
	}
	if (ed) {
		if_success = false;
		return;
	}
	if (pc >= parser.operation.size()) {
		ed = true;
		if_success = false;
		return;
	}
	line_num = pc;
	if_id = parser.operation[pc];
	++pc;
	if_success = true;
}

void Instruction_Decode() {
	if (cyc == 1) 
		return;
	if (!if_success) {
		id_exec[0] = -1;
		return;
	}
	
	id_exec[5] = pc;
	id_exec[6] = line_num;
	switch (if_id.op) {
	case 8:
	case 9:
	case 12:
	case 13:
		id_exec[0] = 1;
		if (regi_lock[if_id.operand[0]]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[1] = regi[if_id.operand[0]];
		if (if_id.operand_kind[1] == operandKind::reg) {
			if (regi_lock[if_id.operand[1]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[if_id.operand[1]];
		}
		else id_exec[2] = if_id.operand[1];
		id_exec[3] = if_id.op;
		regi_lock[32] = regi_lock[33] = true;
		break;
	case 20:
	case 44:
	case 51:
	case 52:
	case 53:
		id_exec[0] = 2;
		id_exec[1] = if_id.operand[0];
		if (if_id.op == 20)
			id_exec[2] = if_id.operand[1];
		else if (if_id.op == 51) {
			if (regi_lock[if_id.operand[1]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[if_id.operand[1]];
		}
		else if (if_id.op == 52) {
			if (regi_lock[32]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[32];
		}
		else if (if_id.op == 53) {
			if (regi_lock[33]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[33];
		}
		else {
			if (if_id.operand_kind[1] == reg) {
				if (regi_lock[if_id.operand[1]]) {
					id_stall = true;
					id_exec[0] = -1;
					return;
				}
				id_exec[2] = regi[if_id.operand[1]];
			}
			else id_exec[2] = if_id.operand[1];
		}
		regi_lock[id_exec[1]] = true;
		break;
	case 27:
	case 40:
	case 41:
		id_exec[0] = 3;
		if (if_id.op == 41) {
			if (regi_lock[if_id.operand[0]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[1] = regi[if_id.operand[0]];
		}
		else id_exec[1] = if_id.operand[0];
		id_exec[5] = jump = id_exec[1];
		break;
	case 42:
	case 43:
		id_exec[0] = 4;
		if (if_id.op == 42)
			id_exec[1] = if_id.operand[0];
		else {
			if (regi_lock[if_id.operand[0]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[1] = regi[if_id.operand[0]];
		}
		id_exec[2] = pc;
		id_exec[5] = jump = id_exec[1];
		regi_lock[31] = true;
		break;
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
		id_exec[0] = 5;
		if (regi_lock[if_id.operand[0]]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[1] = if_id.operand[2];
		id_exec[2] = regi[if_id.operand[0]];
		if (if_id.operand_kind[1] == operandKind::reg) {
			if (regi_lock[if_id.operand[1]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[3] = regi[if_id.operand[1]];
		}
		else id_exec[3] = if_id.operand[1];
		id_exec[4] = if_id.op;
		//regi_lock[34] = true;
		if (predict_value[line_num] > 1) 
			id_exec[5] = jump = id_exec[1];
		break;
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		id_exec[0] = 5;
		id_exec[1] = if_id.operand[1];
		if (regi_lock[if_id.operand[0]]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[2] = regi[if_id.operand[0]];
		id_exec[3] = 0;
		id_exec[4] = if_id.op;
		//regi_lock[34] = true;
		if (predict_value[line_num] > 1) 
			id_exec[5] = jump = id_exec[1];
		break;
	case 45:
	case 46:
	case 47:
		id_exec[0] = 6;
		id_exec[1] = if_id.operand[0];
		if (if_id.operand_kind[1] == operandKind::num) {
			id_exec[2] = if_id.operand[1];
			if (regi_lock[if_id.operand[2]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[3] = regi[if_id.operand[2]];
		}
		else if (if_id.operand_kind[1] == operandKind::reg) {
			if (regi_lock[if_id.operand[1]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[if_id.operand[1]];
			id_exec[3] = 0;
		}
		else {
			id_exec[2] = if_id.operand[1];
			id_exec[3] = 0;
		}
		id_exec[4] = if_id.op;
		regi_lock[id_exec[1]] = true;
		break;
	case 48:
	case 49:
	case 50:
		id_exec[0] = 7;
		if (if_id.operand_kind[1] == operandKind::num) {
			id_exec[1] = if_id.operand[1];
			if (regi_lock[if_id.operand[2]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[2] = regi[if_id.operand[2]];
		}
		else if (if_id.operand_kind[1] == operandKind::reg) {
			if (regi_lock[if_id.operand[1]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[1] = regi[if_id.operand[1]];
			id_exec[2] = 0;
		}
		else {
			id_exec[1] = if_id.operand[1];
			id_exec[2] = 0;
		}
		if (regi_lock[if_id.operand[0]]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[3] = static_cast<size_t>(regi[if_id.operand[0]]);
		id_exec[4] = if_id.op;
		break;
	case 54:
		id_exec[0] = 8;
		break;
	case 55:
		id_exec[0] = 9;
		if (regi_lock[2]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[1] = regi[2];
		if (regi[2] == 1 || regi[2] == 4 || regi[2] == 8 || regi[2] == 9) {
			if (regi_lock[4]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
		}
		id_exec[2] = regi[4];
		if (regi[2] == 9)
			id_exec[3] = memPos;
		else {
			if (regi_lock[5]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[3] = regi[5];
		}
		regi_lock[2] = true;
		break;
	default:
		id_exec[0] = 0;
		id_exec[1] = if_id.operand[0];
		if (regi_lock[if_id.operand[1]]) {
			id_stall = true;
			id_exec[0] = -1;
			return;
		}
		id_exec[2] = regi[if_id.operand[1]];
		if (if_id.operand_kind[2] == operandKind::reg) {
			if (regi_lock[if_id.operand[2]]) {
				id_stall = true;
				id_exec[0] = -1;
				return;
			}
			id_exec[3] = regi[if_id.operand[2]];
		}
		else id_exec[3] = if_id.operand[2];
		id_exec[4] = if_id.op;
		regi_lock[id_exec[1]] = true;
	}
	id_stall = false;
}

void Execution(){
	if (cyc <= 2) return;
	int address;
	long long ret1;
	unsigned long long ret2;
	short *n1;
	int *n2;
	size_t n3;
	short as;
	char *s;
	exec_mem[0] = id_exec[0];
	exec_mem[5] = id_exec[5];
	exec_mem[6] = id_exec[6];
	switch (id_exec[0]) {
	case -1:
		return;
	case 0:
		exec_mem[1] = id_exec[1];
		switch (id_exec[4]) {
		case 1:
			exec_mem[2] = id_exec[2] + id_exec[3];
			break;
		case 2:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) + static_cast<size_t>(id_exec[3]);
			break;
		case 3:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) + static_cast<size_t>(id_exec[3]);
			break;
		case 4:
			exec_mem[2] = id_exec[2] - id_exec[3];
			break;
		case 5:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) - static_cast<size_t>(id_exec[3]);
			break;
		case 6:
			exec_mem[2] = id_exec[2] * id_exec[3];
			break;
		case 7:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) * static_cast<size_t>(id_exec[3]);
			break;
		case 10:
			exec_mem[2] = id_exec[2] / id_exec[3];
			break;
		case 11:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) / static_cast<size_t>(id_exec[3]);
			break;
		case 14:
			exec_mem[2] = id_exec[2] ^ id_exec[3];
			break;
		case 15:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) ^ static_cast<size_t>(id_exec[3]);
			break;
		case 16:
			exec_mem[2] = -id_exec[2];
			break;
		case 17:
			exec_mem[2] = ~(static_cast<size_t>(id_exec[2]));
			break;
		case 18:
			exec_mem[2] = id_exec[2] % id_exec[3];
			break;
		case 19:
			exec_mem[2] = static_cast<size_t>(id_exec[2]) % static_cast<size_t>(id_exec[3]);
			break;
		case 21:
			exec_mem[2] = id_exec[2] == id_exec[3];
			break;
		case 22:
			exec_mem[2] = id_exec[2] >= id_exec[3];
			break;
		case 23:
			exec_mem[2] = id_exec[2] > id_exec[3];
			break;
		case 24:
			exec_mem[2] = id_exec[2] <= id_exec[3];
			break;
		case 25:
			exec_mem[2] = id_exec[2] < id_exec[3];
			break;
		case 26:
			exec_mem[2] = id_exec[2] != id_exec[3];
			break;
		}
		break;
	case 1:
		switch (id_exec[3]) {
		case 8:
			ret1 = static_cast<long long>(id_exec[1]) * static_cast<long long>(id_exec[2]);
			exec_mem[1] = ret1 / static_cast<long long>(pow(2, 32));
			exec_mem[2] = ret1 % static_cast<long long>(pow(2, 32));
			break;
		case 9:
			ret2 = static_cast<unsigned long long>(id_exec[1]) * static_cast<unsigned long long>(id_exec[2]);
			exec_mem[1] = ret2 / static_cast<unsigned long long>(pow(2, 32));
			exec_mem[2] = ret2 % static_cast<unsigned long long>(pow(2, 32));
			break;
		case 12:
			exec_mem[1] = id_exec[1] % id_exec[2];
			exec_mem[2] = id_exec[1] / id_exec[2];
			break;
		case 13:
			exec_mem[1] = static_cast<size_t>(id_exec[1]) % static_cast<size_t>(id_exec[2]);
			exec_mem[2] = static_cast<size_t>(id_exec[1]) / static_cast<size_t>(id_exec[2]);
			break;
		}
		break;
	case 2:
		exec_mem[1] = id_exec[1];
		exec_mem[2] = id_exec[2];
		break;
	case 3:
		exec_mem[1] = id_exec[1];
		break;
	case 4:
		exec_mem[1] = id_exec[1];
		exec_mem[2] = id_exec[2];
		break;
	case 5:
		exec_mem[1] = id_exec[1];
		//regi_lock[34] = false;
		switch (id_exec[4]) {
		case 28:
		case 34:
			exec_mem[2] = id_exec[2] == id_exec[3];
			break;
		case 29:
		case 35:
			exec_mem[2] = id_exec[2] != id_exec[3];
			break;
		case 30:
		case 37:
			exec_mem[2] = id_exec[2] >= id_exec[3];
			break;
		case 31:
		case 36:
			exec_mem[2] = id_exec[2] <= id_exec[3];
			break;
		case 32:
		case 38:
			exec_mem[2] = id_exec[2] > id_exec[3];
			break;
		case 33:
		case 39:
			exec_mem[2] = id_exec[2] < id_exec[3];
			break;
		}
		if (exec_mem[2] == 1) {
			if (predict_value[id_exec[6]] <= 1) {
				if_success = false;
				jump = exec_mem[1];
			}
			if (predict_value[id_exec[6]] < 3)
				++predict_value[id_exec[6]];
			
			exec_mem[5] = exec_mem[1];
		}
		else {
			if (predict_value[id_exec[6]] > 1) {
				if_success = false;
				jump = id_exec[6] + 1;
			}
			if (predict_value[id_exec[6]] > 0)
				--predict_value[id_exec[6]];
			exec_mem[5] = exec_mem[6] + 1;
		}
		break;
	case 6:
		exec_mem[1] = id_exec[1];
		exec_mem[2] = id_exec[2] + id_exec[3];
		switch (id_exec[4]) {
		case 45:
			exec_mem[3] = 1;
			break;
		case 46:
			exec_mem[3] = 2;
			break;
		case 47:
			exec_mem[3] = 4;
			break;
		}
		break;
	case 7:
		exec_mem[1] = id_exec[1] + id_exec[2];
		switch (id_exec[4]) {
		case 48:
			exec_mem[2] = 1;
			break;
		case 49:
			exec_mem[2] = 2;
			break;
		case 50:
			exec_mem[2] = 4;
			break;
		}
		exec_mem[3] = id_exec[3];
		break;
	case 8:
		break;
	case 9:
		exec_mem[1] = id_exec[1];
		exec_mem[2] = id_exec[2];
		exec_mem[3] = id_exec[3];
		break;

	}
}

void Memory_Access(){
	if (cyc <= 3) return;
	short *n1;
	short as;
	int *n2;
	size_t n3;
	char *s;
	string str;
	int st, i;
	for (int i = 0; i < 7; ++i)
		mem_wb[i] = exec_mem[i];
	switch (mem_wb[0]) {
	case -1:
		return;
	case 6:
		mem_stall = true;
		switch (exec_mem[3]) {
		case 1:
			memset(&mem_wb[2], 0, 1);
			mem_wb[2] = memory[exec_mem[2]];
			break;
		case 2:
			memset(&mem_wb[2], 0, 1);
			n1 = reinterpret_cast<short *>(&memory[exec_mem[2]]);
			mem_wb[2] = *n1;
			break;
		case 4:
			n2 = reinterpret_cast<int *>(&memory[exec_mem[2]]);
			mem_wb[2] = *n2;
			break;
		}
		break;
	case 7:
		mem_stall = true;
		switch (exec_mem[2]) {
		case 1:
			n3 = static_cast<size_t>(exec_mem[3]);
			memory[exec_mem[1]] = n3;
			break;
		case 2:
			as = exec_mem[3];
			s = reinterpret_cast<char *>(&as);
			memory[exec_mem[1]] = s[0];
			memory[exec_mem[1] + 1] = s[1];
			break;
		case 4:
			s = reinterpret_cast<char *>(&exec_mem[3]);
			memory[exec_mem[1]] = s[0];
			memory[exec_mem[1] + 1] = s[1];
			memory[exec_mem[1] + 2] = s[2];
			memory[exec_mem[1] + 3] = s[3];
			break;
		}
		break;
	case 9:
		mem_stall = true;
		switch (exec_mem[1]) {
		case 1:
			cout << exec_mem[2];
			mem_wb[3] = 1;
			break;
		case 4:
			printf("%s", memory + exec_mem[2]);
			mem_wb[3] = 4;
			break;
		case 5:
			cin >> mem_wb[1];
			mem_wb[3] = 5;
			break;
		case 8:
			getline(cin, str);
			if (str == "")
				getline(cin, str);
			for (i = 0, st = exec_mem[2]; i < str.length() && i < exec_mem[3] + 1; ++i, ++st)
				memory[st] = str[i];
			memory[st++] = '\0';
			mem_wb[3] = 8;
			break;
		case 9:
			mem_wb[1] = memPos;
			memPos += exec_mem[2];
			mem_wb[3] = 9;
			break;
		case 10:
			exit(0);
			break;
		case 17:
			exit(0);
			break;
		}
		break;
	}
}

void Write_Back(){
	if (cyc <= 4) return;
	switch (mem_wb[0]) {
	case -1:
		return;
	case 0:
		regi[mem_wb[1]] = mem_wb[2];
		regi_lock[mem_wb[1]] = false;
		regi[34] = mem_wb[5];
		break;
	case 1:
		regi[32] = mem_wb[1];
		regi[33] = mem_wb[2];
		regi_lock[32] = regi_lock[33] = false;
		regi[34] = mem_wb[5];
		break;
	case 2:
		regi[mem_wb[1]] = mem_wb[2];
		regi_lock[mem_wb[1]] = false;
		regi[34] = mem_wb[5];
		break;
	case 3:
		regi[34] = mem_wb[5];
		break;
	case 4:
		regi[34] = mem_wb[5];
		regi_lock[31] = false;
		regi[31] = mem_wb[2];
		break;
	case 5:
		/*if (mem_wb[2] == 1)
			regi[34] = mem_wb[1];
		else regi[34] = pc;*/
		regi[34] = mem_wb[5];
		break;
	case 6:
		regi[34] = mem_wb[5];
		memset(&regi[mem_wb[1]], 0, 1);
		regi[mem_wb[1]] = mem_wb[2];
		regi_lock[mem_wb[1]] = false;
		break;
	case 7:
		regi[34] = mem_wb[5];
		break;
	case 8:
		regi[34] = mem_wb[5];
		break;
	case 9:
		regi[34] = mem_wb[5];
		if (mem_wb[3] == 5 || mem_wb[3] == 9)
			regi[2] = mem_wb[1];
		regi_lock[2] = false;
		break;
	}
	
	for (int i = 0; i < 35; ++i)
		out << i << ':' << regi[i] << ' ';
	out << '\n';
}
