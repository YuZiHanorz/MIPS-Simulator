#include "pipeline.h"

using namespace std;

/*int main() {
	Parser parser;
	ifstream inf("12.s");
	parser.parse(inf);
	int next_line = parser.label_text["main"];
	regi[34] = next_line;
	pc = regi[34];
	//ofstream out("ans.txt");
	while (next_line < parser.operation.size()) {
		Token tmp = parser.operation[next_line];
		exec_token(tmp, next_line, memory, memPos);
		regi[34] = next_line;
		cout << tmp;
		cout << "display" << (cnt++) << '\n';
		for (int i = 0; i < 35; ++i)
			cout << regi[i] << ' ';
		cout << '\n';
	}
	while (true) {
		cyc++;
		Write_Back();
		Memory_Access();
		Execution();
		Instruction_Decode();
		Instruction_Fetch(parser);
		if (ed) 
			edcnt--;
		if (edcnt == 0)
			break;
	}

	return 0;
}*/

int main(int argc, char *argv[]) {
	string str = string(argv[1]);
	ifstream inf(str);
	Parser parser;
	parser.parse(inf);
	int next_line = parser.label_text["main"];
	pc = regi[34] = next_line;

	while (true) {
		cyc++;
		Write_Back();
		Memory_Access();
		Execution();
		Instruction_Decode();
		Instruction_Fetch(parser);
		if (ed)
			edcnt--;
		if (edcnt == 0)
			break;
	}

	inf.close();
	return 0;
}
