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
	Token token;
public:
	
	int compile(string input) {
		setsource(input);
		printf("start compilation\n");
		block();
		while(token.showtk()!=Eof) {
			block();
		}
	}
	
	void block() {
		while(1) {
			switch(token.showtk()) {
				case Declint: case Declstring:
					varDecl(); continue;
				default: break;
			}
			break;
		}
		statement();
	}
	
	void setsource(string input) {
		lexer.setsource(input);
		token = lexer.GetToken();
	}
	
	void varDecl() {
		if(token.showtk()==Declint) {
			token = lexer.GetToken();
			token = lexer.checkGet(token, Ident);
			if(token.showtk()==Eq) {
				token = lexer.GetToken();
				expression();
			}
			else if(token.showtk()==Semi) {
				token = lexer.GetToken();
				return;
			}
		}
		else if(token.showtk()==Declstring) {
			token = lexer.GetToken();
			token = lexer.checkGet(token, Ident);
			token = lexer.checkGet(token, Eq);
			expression();
		}
		token = lexer.checkGet(token, Semi);
	}
	
	void statement() {
		switch(token.showtk()) {
		case If:
			token = lexer.GetToken();
			token = lexer.checkGet(token, LPar);
			expression();
			token = lexer.checkGet(token, RPar);
			token = lexer.checkGet(token, LMPar);
			statement();
			token = lexer.checkGet(token, RMPar);
			return;
		case Loop:
			token = lexer.GetToken();
			token = lexer.checkGet(token, LPar);
			condition();
			token = lexer.checkGet(token, RPar);
			token = lexer.checkGet(token, LMPar);
			statement();
			token = lexer.checkGet(token, RMPar);
			return;
		case Print:
			token = lexer.GetToken();
			token = lexer.checkGet(token, LPar);
			expression();
			token = lexer.checkGet(token, RPar);
			token = lexer.checkGet(token, Semi);
			return;
		case RMPar:
			return;
		case Ident:
			token = lexer.GetToken();
			switch(token.showtk()) {
				case Eq: case Pleq: case Mieq: case Tieq: case Diveq: case Expeq: break;
				default: error(); break;
			}
			token = lexer.GetToken();
			expression();
			token = lexer.checkGet(token, Semi);
		case Eof:
			return;
		default:
			error();
			return;
		}
		return;
	}
	
	
	void expression() {
		Tokenkind k;
		k = token.showtk();
		if(k==Plus || k==Minus) {
			token = lexer.GetToken();
		}
		term();
		k = token.showtk();
		while(k==Plus || k==Minus) {
			token = lexer.GetToken();
			term();
			k = token.showtk();
		}
		return;
	}
	
	void term() {
		Tokenkind k;
		factor();
		k = token.showtk();
		while(k==Times || k==Div) {
			token = lexer.GetToken();
			factor();
			k = token.showtk();
		}
	}
	
	void factor() {
		Tokenkind k;
		while(token.showtk() == Plus || token.showtk()==Minus) {
			token = lexer.GetToken();
		}
		if(token.showtk()==Ident) {
			token = lexer.GetToken();
		}
		else if(token.showtk()==Number) {
			token = lexer.GetToken();
		}
		else if(token.showtk()==LPar) {
			token = lexer.GetToken();
			expression();
			token = lexer.checkGet(token, RPar);
		}
		else if(token.showtk()==String) {
			token = lexer.GetToken();
		}
		switch(token.showtk()) {
			case Ident: case Number: case LPar: error();
			default: return;
		}
	}
	
	void error() {
		cout<<"Unexpected Token "<<token.showtext()<<".";
		throw(2);
	}
	
	void condition() {
		Tokenkind k;
		expression();
		k = token.showtk();
		switch(k) {
			case Eqeq: case Noteq: case Gr: case Less: case Greq: case Lesseq: break;
			default: error();
		}
		token = lexer.GetToken();
		expression();
		switch(k) {
			case Eqeq: break;
			case Noteq: break;
			case Gr: break;
			case Less: break;
			case Greq: break;
			case Lesseq: break;
		}
	}
};

int main() {
    string input = "";
    Parser parser;
    Token tok;
    try{
        parser.compile(input);
    }
    catch(int exception) {
        return 0;
    }
}
