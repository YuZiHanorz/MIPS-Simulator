#include "pipeline.h"

using namespace std;

/*int main() {
	int cnt = 1;
	Parser parser;
	ifstream inf("53.s");
	parser.parse(inf);
	int next_line = parser.label_text["main"];
	regi[34] = next_line;
	ofstream out("ans.txt");
	while (next_line < parser.operation.size()) {
		Token tmp = parser.operation[next_line];
		exec_token(tmp, next_line, memory, memPos);
		regi[34] = next_line;
		out << tmp;
		out << "display" << (cnt++) << '\n';
		for (int i = 0; i < 35; ++i)
			out << i << ':' << regi[i] << ' ';
		out << '\n';
	}

	while (true) {
		Instruction_Fetch(parser);
		if (pc - 1 >= parser.operation.size())
			break;
		Instruction_Decode();
		Execution();
		Memory_Access();
		Write_Back();
		out << if_id;
		out << "display" << (cnt++) << '\n';
		for (int i = 0; i < 35; ++i)
			out << i << ':' << regi[i] << ' ';
		out << '\n';
	}

	return 0;
}*/

int main(int argc, char *argv[]) {
	string str = string(argv[1]);
	ifstream inf(str);
	Parser parser;
	parser.parse(inf);
	int next_line = parser.label_text["main"];
	regi[34] = next_line;

	while (true) {
		Instruction_Fetch(parser);
		if (pc - 1 >= parser.operation.size())
			break;
		Instruction_Decode();
		Execution();
		Memory_Access();
		Write_Back();
	}

	inf.close();
	return 0;
}
