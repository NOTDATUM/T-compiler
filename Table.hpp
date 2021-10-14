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
	void newVar(string name) {
		tIndex++;
		Vartable[tIndex].name = name;
	}
	
	int search(string name) {
		for(int i = 0; i<=tIndex; i++) {
			if(name==Vartable[i].name) {
				return i;
			}
		}
		cout<<"Unknown Variable "<<name;
		throw(0);
		return 0;
	}
};



