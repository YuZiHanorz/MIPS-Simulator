#pragma once
#include "command.h"

void Instruction_Fetch(int &next_line) {
	next_line = regi[34];
}

void Data_Preparation(const Parser &parser, int & line) {

}

void Execution(){}

void Memory_Access(){}

void Write_Back(){}