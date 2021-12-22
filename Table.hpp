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
	
	void delVar(string name) {
		VariableStore x = search(name);
		Vartable[x.addr].addr = -1;
	}
	
	VariableStore search(string name) {
		for(int i = 0; i<tIndex; i++) {
			if(name==Vartable[i].name && Vartable[i].addr!=-1) {
				return Vartable[i];
			}
		}
		cout<<"Unknown Variable "<<name;
		throw(0);
		return Vartable[9999];
	}
};
Table vartable;


class FuncStore {
public:
	string name;
	int codeline;
};

class FuncTable {
public:
	FuncStore Functable[1000];
	int findex = 0;
	int addr = 0;
	FuncStore error = {"", -1};
	void newFunc(int codeline, string name) {
		Functable[findex].codeline = codeline;
		Functable[findex++].name = name;
	}
	
	FuncStore searchFunc(string name) {
		for(int i = 0; i<findex; i++) {
			if(name==Functable[i].name && Functable[i].codeline!=-1) {
				return Functable[i];
			}
		}
		return error;
	}
};
FuncTable functable;

