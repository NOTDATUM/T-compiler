//
//  main.cpp
//  T_Compiler
//
//  Created by 박세진 on 2021/08/18.
//  Copyright © 2021 박세진. All rights reserved.
//

#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

enum Tokenkind {
    Eof = -1, Plus = 0, Minus, Times, Div, Eq, Exp, Gr, Less, Greq, Lesseq, Eqeq, Noteq,
    Number, Ident, String, Pleq, Mieq, Tieq, Diveq, Expeq, Plpl, Mimi, If, Loop
};

class Token {
private:
    Tokenkind tk;
    string tokentext;
public:
    void init(Tokenkind nowtk, string nowtokentext) {
        tk = nowtk;
        tokentext = nowtokentext;
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
            if(curPos==source.size()-1) break;
        }
    }
    
    Token GetToken() {
        Token token;
        skipSpace();
        if(curChar=='#') {
            nextChar();
            while(curChar!='#') {
                nextChar();
                if(curChar=='\0') abort("No Ending '#'");
            }
            nextChar();
            skipSpace();
        }
        if(curChar=='+') {
            if(peek()=='=') {
                nextChar();
                token.init(Pleq, "+=");
            }
            else if(peek()=='+') {
                nextChar();
                token.init(Plpl, "++");
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
            else if(peek()=='-') {
                nextChar();
                token.init(Mimi, "--");
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
        else if(curChar=='^') {
            if(peek()=='=') {
                nextChar();
                token.init(Expeq, "^=");
            }
            else {
                token.init(Exp, string(1, curChar));
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
        else if(curChar=='\0') {
            token.init(Eof, string(1, curChar));
        }
        else {
            abort("Unknown Token: "+string(1, curChar));
        }
        nextChar();
        return token;
    }
};





int main() {
    string input = "\"\nHello, World!\"+=+";
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
        cout<<tok.showtext()<<'\n';
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
