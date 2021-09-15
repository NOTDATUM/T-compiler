//
//  Parser.cpp
//  T_Compiler
//
//  Created by 박세진 on 2021/09/01.
//  Copyright © 2021 박세진. All rights reserved.
//

#include "Lexer.h"

class Parser {
private:
	Lexer lexer;
public:
	void setsource(string input) {
		lexer.setsource(input);
	}
	
};

int main() {
    string input = "loop(5;i>0) { }";
    Lexer lexer;
    lexer.setsource(input);
    Token tok;
    try{
        tok = lexer.GetToken();
    }
    catch(int exception) {
        return 0;
    }
    while(1) {
        cout<<tok.showtk()<<" "<<tok.showtext()<<'\n';
        try{
            tok = lexer.GetToken();
        }
        catch(int exception) {
            return 0;
        }
        if(tok.showtk()==Eof) {
            break;
        }
    }
}
