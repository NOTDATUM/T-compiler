//
//  Parser.cpp
//  T_Compiler
//
//  Created by 박세진 on 2021/09/01.
//  Copyright © 2021 박세진. All rights reserved.
//

#include "Lexer.h"
#include "Codegen.hpp"

class Parser {
private:
	Lexer lexer;
	Token token;
	Table vartable;
	
public:
	
	int compile(string input) {
		setsource(input);
		printf("start compilation\n");
		block();
	}
	
	void block() {
		while(token.showtk()!=Eof) {
			statement();
		}
	}
	
	void setsource(string input) {
		lexer.setsource(input);
		token = lexer.GetToken();
	}
	
	void statement() {
		VariableStore tindex;
		switch(token.showtk()) {
		case Declint:
			token = lexer.GetToken();
			vartable.newVar(token.showtext());
			token = lexer.checkGet(token, Ident);
			if(token.showtk()==Eq) {
				token = lexer.GetToken();
				expression();
			}
			else if(token.showtk()==Semi) {
				token = lexer.GetToken();
				return;
			}
			token = lexer.checkGet(token, Semi);
			return;
		case Declstring:
			token = lexer.GetToken();
			token = lexer.checkGet(token, Ident);
			token = lexer.checkGet(token, Eq);
			expression();
			token = lexer.checkGet(token, Semi);
			return;
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
			tindex = vartable.search(token.showtext());
			token = lexer.GetToken();
			switch(token.showtk()) {
				case Eq: case Pleq: case Mieq: case Tieq: case Diveq: case Expeq: break;
				default: error(); break;
			}
			token = lexer.GetToken();
			expression();
			genCodeT(mov, tindex);
			token = lexer.checkGet(token, Semi);
			return;
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
			genCodeV(psh, token.tokenvalue);
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
    string input = "int a = 0; a = 1;";
    Parser parser;
    Token tok;
    try{
        parser.compile(input);
    }
    catch(int exception) {
        return 0;
    }
    int i = 0;
    while(i<3) {
    	if(code[i].opCode!=opr) {
    		cout<<code[i].opCode<<" "<<code[i].u.v.addr<<" "<<code[i].u.v.name<<"\n";
    		i++;
		}
	}
}
