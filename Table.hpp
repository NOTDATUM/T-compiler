#include <iostream>
#include <string>
using namespace std;

class VariableStore {
public:
	string name;
	int addr;
};

class Table {
public:
	VariableStore Vartable[10000];
	int tIndex = 0;
	int addr = 0;
	void newVar(string name) {
		Vartable[tIndex].addr = addr++;
		Vartable[tIndex++].name = name;
	}
	
	VariableStore search(string name) {
		for(int i = 0; i<tIndex; i++) {
			if(name==Vartable[i].name) {
				return Vartable[i];
			}
		}
		cout<<"Unknown Variable "<<name;
		throw(0);
		return Vartable[9999];
	}
};
Table vartable;



