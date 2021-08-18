//
//  main.cpp
//  T_Compiler
//
//  Created by 박세진 on 2021/08/18.
//  Copyright © 2021 박세진. All rights reserved.
//

#include <string>
#include <stdio.h>
using namespace std;
class Lexer {
private:
    string source;
    char curChar;
    int curPos;
public:
    void nextChar() {
        curPos++;
        if(curPos>=source.size()) {
            curChar = '\0';
        }
        else {
            curChar = source[curPos];
        }
    }
};

int main() {
    
}
