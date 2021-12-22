#include "Lexer.hpp"
#include "Codegen.hpp"

class Parser {
private:
	Lexer lexer;
	Token token;
public:
	int returnnumber = 0;
	
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
							vartable.newVar(token.showtext(), 1);
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
							genCode(def, vartable.addr-1, nothing);
							return;
						default:
							error();
							return;
					}
				case Declarr:
					token = lexer.checkGet(token, Declarr);
					token = lexer.checkGet(token, LBPar);
					if(token.showtk()==Number) {
						int arrlen = token.tokenvalue;
						token = lexer.checkGet(token, Number);
						token = lexer.checkGet(token, RBPar);
						vartable.newVar(token.showtext(), arrlen);
						token = lexer.checkGet(token, Ident);
						token = lexer.checkGet(token, Semi);
					}
					break;
				case Declstring:
					token = lexer.GetToken();
					token = lexer.checkGet(token, Ident);
					token = lexer.checkGet(token, Eq);
					expression();
					token = lexer.checkGet(token, Semi);
					return;
				case Func:
					token = lexer.GetToken();
					functable.newFunc(cIndex, token.showtext());
					token = lexer.checkGet(token, Ident);
					if(token.showtk()==Eq) {
						backpatchindex = cIndex;
						genCode(cal, 0, nothing);
						functable.Functable[functable.findex-1].codeline = cIndex;
						token = lexer.checkGet(token, Eq);
						token = lexer.checkGet(token, LMPar);
						returnnumber = Calcstacktop;
						while(token.showtk()!=RMPar) {
							statement();
						}
						code[backpatchindex].val = cIndex;
						token = lexer.checkGet(token, RMPar);
						Calcstacktop = returnnumber;
					}
					break;
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
				vartable.newVar("cnt", 1);
				int cntindex = vartable.addr-1;
				int doline = cIndex;
				int lasttop = Calcstacktop;
				token = lexer.GetToken();
				genCode(def, cntindex, nothing);
				token = lexer.checkGet(token, LMPar);
				while(token.showtk()!=RMPar) {
					statement();
				}
				token = lexer.checkGet(token, RMPar);
				genCode(top, lasttop-1, nothing);
				genCode(mov, cntindex, nothing);
				genCode(psh, 1, nothing);
				genCode(opr, 0, sub);
				genCode(jmp, doline, nothing);
			}
			else {
				token = lexer.checkGet(token, Colon);
				int doline = cIndex;
				vartable.newVar("cnt", 1);
				int cntindex = vartable.addr-1;
				int lasttop = Calcstacktop;
				genCode(def, cntindex, nothing);
				condition();
				genCode(opr, 0, inv);
				if(token.showtk()==RPar) {
					token = lexer.GetToken();
					backpatchindex = cIndex;
					genCode(jmp, 0, nothing);
					token = lexer.checkGet(token, LMPar);
					while(token.showtk()!=RMPar) {
						statement();
					}
					token = lexer.checkGet(token, RMPar);
					
					genCode(top, lasttop-1, nothing);
					genCode(mov, cntindex, nothing);
					genCode(psh, 1, nothing);
					genCode(opr, 0, sub);
					genCode(jmp, doline, nothing);
					code[backpatchindex].val = cIndex;
				}
				else {
					token = lexer.checkGet(token, Colon);
					
				}
			}
			break;
		case Return:
			token = lexer.checkGet(token, Return);
			expression();
			genCode(swp, -(Calcstacktop-returnnumber)+1, nothing);
			genCode(ttp, -(Calcstacktop - returnnumber), nothing);
			genCode(ret, 0, nothing);
			token = lexer.checkGet(token, Semi);
			return;
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
			if(token.showtk()==String) {
				for(int stringcnt = 0; stringcnt<token.showtext().length() ; stringcnt++) {
					genCode(psh, (int)token.showtext()[stringcnt], nothing);
					genCode(opr, 0, pch);
				}
				token = lexer.checkGet(token, String);
				token = lexer.checkGet(token, RPar);
				token = lexer.checkGet(token, Semi);
				return;
			}
			expression();
			token = lexer.checkGet(token, RPar);
			token = lexer.checkGet(token, Semi);
			genCode(opr, 0, prt);
			return;
		case RMPar:
			return;
		case Ident:
			identname = token.showtext();
			if(functable.searchFunc(identname).codeline!=-1) {
				token = lexer.checkGet(token, Ident);
				token = lexer.checkGet(token, Semi);
				genCode(psh, cIndex+2, nothing);
				genCode(cal, functable.searchFunc(identname).codeline, nothing);
				genCode(ttp, 1, nothing);
				genCode(swp, -1, nothing);
				genCode(ttp, -1, nothing);
				return;
			}
			token = lexer.GetToken();
			if(token.showtk()==LBPar) {
				token = lexer.checkGet(token, LBPar);
				expression();
				genCode(psh, vartable.search(identname).addr, nothing);
				genCode(opr, 0, add);
				token = lexer.checkGet(token, RBPar);
				oprtoken = token.showtk();
				switch(token.showtk()) {
					case Eq: case Pleq: case Mieq: case Tieq: case Diveq: case Remeq: break;
					default: error(); break;
				}
				token = lexer.GetToken();
				expression();
				genCode(ttp, -1, nothing);
				genCode(mtp, 0, nothing);
				genCode(ttp, 2, nothing);
				switch(oprtoken) {
					case Eq:
						break;
					case Pleq:
						genCode(mop, 0, nothing);
						genCode(opr, 0, add);
						break;
					case Mieq:
						genCode(mop, 0, nothing);
						genCode(opr, 0, sub);
						genCode(opr, 0, neg);
						break;
					case Tieq:
						genCode(mop, 0, nothing);
						genCode(opr, 0, mul);
						break;
					case Diveq:
						genCode(mop, 0, nothing);
						genCode(opr, 0, dvs);
						break;
					case Remeq:
						genCode(mop, 0, nothing);
						genCode(opr, 0, add); // !!
						break;
					default:
						error();
						break;
				}
				genCode(sav, 0, nothing);
				token = lexer.checkGet(token, Semi);
				return;
			}
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
//		k = token.showtk();
//		if(k==Plus || k==Minus) {
//			token = lexer.GetToken();
//		}
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
//			cout<<token.showtext()<<"!!";
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
		
		if(token.showtk()==Ident) {
			string name = token.showtext();
			if(functable.searchFunc(name).codeline!=-1) {
				token = lexer.checkGet(token, Ident);
				genCode(psh, cIndex+2, nothing);
				genCode(cal, functable.searchFunc(name).codeline, nothing);
				genCode(ttp, 1, nothing);
				genCode(swp, -1, nothing);
				genCode(ttp, -1, nothing);
				return;
			}
			token = lexer.checkGet(token, Ident);
			if(token.showtk()==LBPar) {
				token = lexer.checkGet(token, LBPar);
				expression();
				token = lexer.checkGet(token, RBPar);
				genCode(psh, vartable.search(name).addr, nothing);
				genCode(opr, 0, add);
				genCode(mtp, 0, nothing);
				genCode(mop, 0, nothing);
				return;
			}
			while(token.showtk()=='.') {
				token = lexer.checkGet(token, Point);
				name = name + ".";
				token = lexer.GetToken();
				name = name + token.showtext();
			}
			genCode(mov, vartable.search(name).addr, nothing);
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
		if(i==1) {
			genCode(opr, 0, neg);
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
