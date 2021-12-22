#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

enum Tokenkind {
    Eof = -1,
	Plus = 0,
	Minus, Times, Div, Eq, Rem,
	Gr, Less, Greq, Lesseq, Eqeq, Noteq,
    Number, Ident, String, Bool,
	Pleq, Mieq, Tieq, Diveq, Remeq,
	Do, Def, Cnt,
	Num, Declint, Declfloat, Declstring, Func,
	Semi, Colon, LPar, RPar, LMPar, RMPar, Point,
	Print, Input, Return
};

struct keyWord {
    string word;
    Tokenkind tk;
};

vector<keyWord> KeyWdT = {
	{"do", Do}, {"def", Def}, {"num", Num}, {"int", Declint}, {"float", Declfloat}, {"str", Declstring}, {"func", Func}, {"output", Print}, {"input", Input},
	{"return", Return}
};

bool isNum(char c) {
    if(48<=c && c<58) {
        return true;
    }
    return false;
}

bool isLetter(char c) {
    if(('a'<=c && c<='z') || ('A'<=c && c<='Z')) {
        return true;
    }
    return false;
}

class Token {
public:
    Tokenkind tk;
    string tokentext = "";
    int tokenvalue = 0;
    void init(Tokenkind nowtk, string nowtokentext) {
        tk = nowtk;
        tokentext = nowtokentext;
    }
    void setValue(int nowtokenvalue) {
        tokenvalue = nowtokenvalue;
    }
    string showtext() {
        return tokentext;
    }
    Tokenkind showtk() {
        return tk;
    }
};

class Lexer {
private:
    string source = "\0";
    char curChar = 0;
    int curPos = 0;
public:
    void setsource(string filestr) {
        source = filestr + "\n";
        curChar = source[0];
    }
    
    void nextChar() {
        curPos++;
        if(curPos>=source.size()) {
            curChar = '\0';
        }
        else {
            curChar = source[curPos];
        }
    }
    
    char peek() {
        if(curPos+1>=source.size()) return '\0';
        return source[curPos+1];
    }
    
    void abort(string errorMessage) {
        cout<<"Lexing Error. "<<errorMessage<<'\n';
        throw(1);
    }
    
    void skipSpace() {
        while(curChar==' ' || curChar=='\t' || curChar=='\n' || curChar=='\r') {
            nextChar();
        }
    }
    
    Token GetToken() {
        Token token;
        skipSpace();
//        while(curChar=='#') {
//            nextChar();
//            while(curChar!='#') {
//                nextChar();
//                if(curChar=='\0') abort("No Ending '#'");
//            }
//            nextChar();
//            skipSpace();
//        }
        if(curChar=='+') {
            if(peek()=='=') {
                nextChar();
                token.init(Pleq, "+=");
            }
            else {
                token.init(Plus, string(1, curChar));
            }
        }
        else if(curChar=='-') {
            if(peek()=='=') {
                nextChar();
                token.init(Mieq, "-=");
            }
            else {
                token.init(Minus, string(1, curChar));
            }
        }
        else if(curChar=='*') {
            if(peek()=='=') {
                nextChar();
                token.init(Tieq, "*=");
            }
            else {
                token.init(Times, string(1, curChar));
            }
        }
        else if(curChar=='/') {
            if(peek()=='=') {
                nextChar();
                token.init(Diveq, "/=");
            }
            else {
                token.init(Div, string(1, curChar));
            }
        }
        else if(curChar=='%') {
            if(peek()=='=') {
                nextChar();
                token.init(Remeq, "%=");
            }
            else {
                token.init(Rem, string(1, curChar));
            }
        }
        else if(curChar=='=') {
            if(peek()=='=') {
                nextChar();
                token.init(Eqeq, "==");
            }
            else {
                token.init(Eq, string(1, curChar));
            }
        }
        else if(curChar=='>') {
            if(peek()=='=') {
                nextChar();
                token.init(Greq, ">=");
            }
            else {
                token.init(Gr, string(1, curChar));
            }
        }
        else if(curChar=='<') {
            if(peek()=='=') {
                nextChar();
                token.init(Lesseq, "<=");
            }
            else {
                token.init(Less, string(1, curChar));
            }
        }
        else if(curChar=='!') {
            if(peek()=='=') {
                nextChar();
                token.init(Noteq, "!=");
            }
            else {
                abort("Expected '!='.");
            }
        }
        else if(curChar=='\"') {
            nextChar();
            string result = "";
            while(curChar!='\"') {
                if(curChar=='\0') {
                    abort("No ending \".");
                }
                result+=curChar;
                nextChar();
            }
            token.init(String, result);
        }
        else if(curChar==';') {
            token.init(Semi, string(1, curChar));
        }
        else if(curChar==':') {
        	token.init(Colon, string(1, curChar));
		}
        else if(curChar=='(') {
            token.init(LPar, string(1, curChar));
        }
        else if(curChar==')') {
            token.init(RPar, string(1, curChar));
        }
        else if(curChar=='{') {
            token.init(LMPar, string(1, curChar));
        }
        else if(curChar=='}') {
            token.init(RMPar, string(1, curChar));
        }
        else if(curChar=='.') {
        	token.init(Point, string(1, curChar));
		}
        else if(isNum(curChar)) {
            string result = string(1, curChar);
            int nowvalue = (int)curChar-48;
            while(isNum(peek())) {
                nextChar();
                result+=curChar;
                nowvalue = nowvalue*10+(int)curChar-48;
            }
            token.init(Number, result);
            token.setValue(nowvalue);
        }
        else if(isLetter(curChar)) {
            string name = string(1, curChar);
            while(isLetter(peek()) || isNum(peek())) {
                nextChar();
                name += curChar;
            }
            for(int i = 0; i<KeyWdT.size(); i++) {
                if(name==KeyWdT[i].word) {
                    token.init(KeyWdT[i].tk, KeyWdT[i].word);
                    break;
                }
                if(i==KeyWdT.size()-1) {
                    token.init(Ident, name);
                }
            }
        }
        else if(curChar=='\0') {
            token.init(Eof, string(1, curChar));
        }
        else {
            abort("Unknown Token: "+string(1, curChar));
        }
        nextChar();
        return token;
    }
    
    Token checkGet(Token t, Tokenkind k) {
        if(t.showtk()==k) {
            return GetToken();
        }
        cout<<"Parsing Error. In correct token for "+t.showtext();
        return t;
    }
};






