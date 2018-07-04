#include "pipeline.h"

using namespace std;

int main() {
	Parser parser;
	ifstream inf("37.s");
	parser.parse(inf);
	int next_line = parser.label_text["main"];
	regi[34] = next_line;
	//ofstream out("ans.txt");
	while (next_line < parser.operation.size()) {
		Token tmp = parser.operation[next_line];
		exec_token(tmp, next_line, memory, parser.memPos);
		regi[34] = next_line;
		//out << tmp;
		//out << "display" << '\n';
		//for (int i = 0; i < 35; ++i)
			//out << i << ": " << regi[i] << ' ';
		//out << '\n';
	}

	return 0;
}