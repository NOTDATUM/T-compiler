#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "Lexer.h"

using namespace std;

int main() {
    string input = "int a = 0; if(a>5) a++; #4#";
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
