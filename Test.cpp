#include "Parser.hpp"

int main() {
	string SettingCode = "def num.int a; do(1:1==1) {a = 1;} output(a);";
	Parser parser;
	parser.compile(SettingCode);
	listCode();
	execute();
}
