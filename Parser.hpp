#include "Lexer.hpp"
#include "Codegen.hpp"

class Parser {
private:
	Lexer lexer;
	Token token;
public:
	
	int compile(string input) {
		setsource(input);
		// printf("start compilation\n");
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
		string identname;
		int backpatchindex;
		int oprtoken = 0;
		switch(token.showtk()) {
		case Def:
			token = lexer.GetToken();
			switch(token.showtk()) {
				case Num:
					token = lexer.GetToken();
					token = lexer.checkGet(token, Point);
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
							genCode(def, vartable.tIndex-1, nothing);
							return;
						default:
							error();
							return;
					}
					
				case Declstring:
					token = lexer.GetToken();
					token = lexer.checkGet(token, Ident);
					token = lexer.checkGet(token, Eq);
					expression();
					token = lexer.checkGet(token, Semi);
					return;
				default:
					error();
					return;
			}
			break;
		case Do:
			token = lexer.GetToken();
			token = lexer.checkGet(token, LPar);
			expression();
			if(token.showtk()==RPar) {
				vartable.newVar("cnt");
				int cntindex = vartable.tIndex-1;
				int doline = cIndex;
				int lasttop = Calcstacktop;
				token = lexer.GetToken();
				genCode(def, cntindex, nothing);
				token = lexer.checkGet(token, LMPar);
				while(token.showtk()!=RMPar) {
					statement();
				}
				token = lexer.checkGet(token, RMPar);
				genCode(top, lasttop, nothing);
				genCode(mov, cntindex, nothing);
				genCode(psh, 1, nothing);
				genCode(opr, 0, sub);
				genCode(jmp, doline, nothing);
			}
			else {
				token = lexer.checkGet(token, Colon);
				condition();
				if(lexer.peek()==RPar) {
					
				}
				else {
					token = lexer.checkGet(token, Colon);
					
				}
			}
			break;
//			condition();
//		case If:
//			token = lexer.GetToken();
//			token = lexer.checkGet(token, LPar);
//			condition();
//			token = lexer.checkGet(token, RPar);
//			backpatchindex = cIndex;
//			genCode(jmp, 0, nothing);
//			token = lexer.checkGet(token, LMPar);
//			statement();
//			code[backpatchindex].val = cIndex;
//			token = lexer.checkGet(token, RMPar);
//			return;
//		case Loop:
//			token = lexer.GetToken();
//			token = lexer.checkGet(token, LPar);
//			condition();
//			token = lexer.checkGet(token, RPar);
//			token = lexer.checkGet(token, LMPar);
//			statement();
//			token = lexer.checkGet(token, RMPar);
//			return;
		case Print:
			token = lexer.GetToken();
			token = lexer.checkGet(token, LPar);
			expression();
			token = lexer.checkGet(token, RPar);
			token = lexer.checkGet(token, Semi);
			genCode(opr, 0, prt);
			return;
		case RMPar:
			return;
		case Ident:
			identname = token.showtext();
			token = lexer.GetToken();
			oprtoken = token.showtk();
			switch(token.showtk()) {
				case Eq: case Pleq: case Mieq: case Tieq: case Diveq: case Remeq: break;
				default: error(); break;
			}
			token = lexer.GetToken();
			expression();
			switch(oprtoken) {
				case Eq:
					break;
				case Pleq:
					genCode(mov, vartable.search(identname).addr, nothing);
					genCode(opr, 0, add);
					break;
				case Mieq:
					genCode(mov, vartable.search(identname).addr, nothing);
					genCode(opr, 0, sub);
					genCode(opr, 0, neg);
					break;
				case Tieq:
					genCode(mov, vartable.search(identname).addr, nothing);
					genCode(opr, 0, mul);
					break;
				case Diveq:
					genCode(mov, vartable.search(identname).addr, nothing);
					genCode(opr, 0, dvs);
					break;
				case Remeq:
					genCode(mov, vartable.search(identname).addr, nothing);
					genCode(opr, 0, add); // !!
					break;
				default:
					error();
					break;
			}
			genCode(def, vartable.search(identname).addr, nothing);
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
			if(k==Plus) {
				genCode(opr, 0, add);
			}
			else {
				genCode(opr, 0, sub);
			}
			k = token.showtk();
		}
		return;
	}
	
	void term() {
		Tokenkind k;
		factor();
		k = token.showtk();
		while(k==Times || k==Div) {
			cout<<token.showtext()<<"!!";
			token = lexer.GetToken();
			factor();
			if(k==Times) {
				genCode(opr, 0, mul);
			}
			else {
				genCode(opr, 0, dvs);
			}
			k = token.showtk();
		}
	}
	
	void factor() {
		Tokenkind k;
		int i = 0;
		while(token.showtk() == Plus || token.showtk()==Minus) {
			if(token.showtk()==Minus) {
				i += 1;
			}
			token = lexer.GetToken();
			i%=2;
		}
		if(i==1) {
			genCode(opr, 0, neg);
		}
		if(token.showtk()==Ident) {
			genCode(mov, vartable.search(token.showtext()).addr, nothing);
			token = lexer.GetToken();
		}
		else if(token.showtk()==Number) {
			genCode(psh, token.tokenvalue, nothing);
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
			case Eqeq: genCode(opr, 0, eql); break;
			case Noteq: genCode(opr, 0, neq); break;
			case Gr: genCode(opr, 0, grt); break;
			case Less: genCode(opr, 0, lss); break;
			case Greq: genCode(opr, 0, grq); break;
			case Lesseq: genCode(opr, 0, lsq); break;
			default: break;
		}
	}
};
