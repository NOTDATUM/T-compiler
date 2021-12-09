#include "Parser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;

string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

int main(int argc, char *argv[]) {
	if(argc > 2) printf("Path error: The file path is invalid.");
	else {
    	string filename(argv[1]);
    	string input = readFileIntoString(filename);
   		Parser parser;
    	Token tok;
    	try{
	        parser.compile(input);
	    }
	    catch(int exception) {
	        return 0;
	    }
	    int i = 0;	
		//	listCode();
	    execute();
	}
	exit(EXIT_SUCCESS);
}
