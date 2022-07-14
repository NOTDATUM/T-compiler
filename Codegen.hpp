#include "Table.hpp"
#define MAXCODE 200	
#define MAXMEM 20002
#define MAXREG 20
#define MAXLEVEL 5
#include <stdio.h>
#include <time.h>
#include <iostream>
using namespace std;

// <codes> cod
// jmp: jumps to another code if the top value of Calcstack is 0, jmp {CodeNumber} ex) jmp 3 
// gto: jumps to another code with no condition, gto {CodeNumber} ex) gto 3
// cal: jumps to another code and saves its line number+1 in scopestack, cal {CodeNumber} ex) cal 3

// <memories> mem
// psh: pushes number into Calcstack, push {num} ex) push 5
// def: moves the top value of Calcstack to the top of Memstack, def ex) def 
// asg: assigns the top value of Calacstack to the address of Memstack, asg {Address} ex) asg 1
// mov: pushes a variable's value into Calcstack, mov {AddressOfVariable} ex) mov 1

// <pointers> pnt
// mtp: sets the top value as the pointer of Memstack, mtp ex) mtp
// sav: saves the top value to the Memstack with the pointer, sav ex) sav
// mop: moves the Memstack with the pointer into Calcstack, mop ex) mop

// <address> adr
// top: changes the top address of Calcstack, top {Address} ex) top 1
// ttp: changes the top address of Calcstack by delta, top {+-deltaAddress} ex) top -1
// swp: changes the Calcstack with the top value, swp {Address} ex) swp 3

// <scopes> scp: interacts with scope
// nsc: pushs a new scope and only saves the memstacktop, ex) nsc
// dsc: pops the top of the scope stack, ex) dsc
// dls: delete memory of last scope, ex) dls
// gts: goto the top of scope and pops the top of the scope, ex) gts

// <oprerators> opr: Only interacts with Calcstack
// [arithmetic]
// neg: changes the sign of the top value, opr neg
// add: adds the top two values, opr add
// sub: subtracts the top two values, opr sub
// mul: multiplies the top two values, opr mul
// dvs: divides the top two values in an integer value, opr dvs

// [compare]
// eql: checks if the top two values are equal, opr eql
// lss: checks if the second value is smaller than the first value, opr lss
// grt: checks if the second value is greater than the first value, opr grt
// neq: checks if the top two values are different, opr neq
// lsq: checks if the second value is smaller than or equal with the first value, opr lsq
// grq: checks if the second value is greater than or equal with the first value, opr grq

// [bits]
// inv: returns 1 if the top value is 0, 0 if the top value isn't 0, opr inv

// [input/output]
// prt: prints the top value, opr prt
// pch: prints the top value as char, opr pch
// inp: inputs one value and pushes it, opr inp

typedef enum typecodes {
	cod, mem, pnt, adr, scp, opr, tpNone
} TypeCode;

typedef enum codes {
	jmp, gto, cal,
	psh, def, asg, mov,
	mtp, sav, mop,
	top, ttp, swp,
	nsc, dsc, dls, gts,
	neg, add, sub, mul, dvs, eql, lss, grt, neq, lsq, grq, inv, prt, pch, inp, opNone
} OpCode;

bool haveVal[35] = 
{
	1, 1, 1,
	1, 0, 1, 1,
	0, 0, 0,
	1, 1, 1,
	0, 0, 0,
	0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

class Inst {
public:
	TypeCode tpCode;
	OpCode opCode;
	int val = 0;
	Inst() {
		tpCode = tpNone;
		opCode = opNone;
		val = -1;
	}
	
	Inst(TypeCode tp, OpCode op, int codeval) {
		tpCode = tp; opCode = op; val = codeval;
	}
};

Inst code[200];	
int cIndex = 0;
void printCode(int i);
int Calcstacktop = -1, Memstacktop = 0;

void genCode(TypeCode tp, OpCode op, int val) {
	code[cIndex] = Inst(tp, op, val);
	cIndex++;
	switch(op) {
		case psh: case mov: case mop:
			Calcstacktop++; break;
		case def: case asg: case mtp: case sav: Calcstacktop--; break;
		case top: Calcstacktop = val; break;
		case ttp: Calcstacktop += val; break;
		case add: case sub: case mul: case dvs: case eql: case lss: case grt: case neq: case lsq: case grq: 
		case prt: case pch: case jmp: Calcstacktop--; break;
		default: break;
	}
}

void listCode()	
{
	int i;
	printf("\ncode\n");
	for(i=0; i<cIndex; i++) {
		printf("%3d: ", i);
		printCode(i);
	}
}

void printCode(int i)
{
	
	switch(code[i].tpCode) {
		case cod:
			printf("cod "); 
			switch(code[i].opCode) {
				case jmp: printf("jmp "); break;
				case gto: printf("gto "); break;
				case cal: printf("cal "); break;
				default: break;
			}
			break;
		case mem:
			printf("mem "); 
			switch(code[i].opCode) {
				case psh: printf("psh "); break;
				case def: printf("def "); break;
				case asg: printf("asg "); break;
				case mov: printf("mov "); break;
				default: break;
			}
			break;
		case pnt:
			printf("pnt ");
			switch(code[i].opCode) {
				case mtp: printf("mtp "); break;
				case sav: printf("sav "); break;
				case mop: printf("mop "); break;
				default: break;
			}
			break;
		case adr:
			printf("adr ");
			switch(code[i].opCode) {
				case top: printf("top "); break;
				case ttp: printf("ttp "); break;
				case swp: printf("swp "); break;
				default: break;
			}
			break;
		case scp:
			printf("scp ");
			switch(code[i].opCode) {
				case nsc: printf("nsc "); break;
				case dsc: printf("dsc "); break;
				case dls: printf("dls "); break;
				case gts: printf("gts "); break;
				default: break;
			}
			break;
		case opr:
			printf("opr ");
			switch(code[i].opCode) {
				case neg: printf("neg "); break;
				case add: printf("add "); break;
				case sub: printf("sub "); break;
				case mul: printf("mul "); break;
				case dvs: printf("dvs "); break;
				case eql: printf("eql "); break;
				case lss: printf("lss "); break;
				case grt: printf("grt "); break;
				case neq: printf("neq "); break;
				case lsq: printf("lsq "); break;
				case grq: printf("grq "); break;
				case inv: printf("inv "); break;
				case prt: printf("prt "); break;
				case pch: printf("pch "); break;
				case inp: printf("inp "); break;
				default: break;
			}	
			break;
		default: break;
	}
	if(haveVal[code[i].opCode]) {
		printf("%d ", code[i].val);
	}
	printf("\n");
}	

void execute()
{
	int memstack[MAXMEM]; int mempointer = 0; int memstacktop = -1;
	int calcstack[MAXMEM]; int calcstacktop = -1;
	int scopestack[MAXMEM]; int scopestacktop = -1; int scopememory[MAXMEM];
	
	int i = 0;
	clock_t st = clock();
	while(i<cIndex) { 
		printf("code:");
		printCode(i);
		printf("output:");
		switch(code[i].tpCode) {
			case cod:
				switch(code[i].opCode) {
					case jmp:
						if(calcstack[calcstacktop]==0) i = code[i].val - 1;
						calcstacktop--;
						break;
					case gto:
						i = code[i].val - 1;
						break;
					case cal:
						scopestacktop++;
						scopestack[scopestacktop] = i;
						scopememory[scopestacktop] = memstacktop;
						i = code[i].val - 1;
						break;
					default: break;
				}
				break;
			case mem:
				switch(code[i].opCode) {
					case psh:
						calcstacktop++;
						calcstack[calcstacktop] = code[i].val;
						break;
					case def:
						memstacktop++;
						memstack[memstacktop] = calcstack[calcstacktop];
						calcstacktop--;
						break;
					case asg:
						if(code[i].val>=0) memstack[code[i].val] = calcstack[calcstacktop];
						else memstack[memstacktop+code[i].val+1] = calcstack[calcstacktop];
						calcstacktop--;
						break;
					case mov:
						calcstacktop++;
						if(code[i].val>=0) calcstack[calcstacktop] = memstack[code[i].val];
						else calcstack[calcstacktop] = memstack[memstacktop+code[i].val+1];
						break;
					default: break;	
				}
				break;
			case pnt:
				switch(code[i].opCode) {
					case mtp:
						mempointer = calcstack[calcstacktop];
						calcstacktop--;
						break;
					case sav:
						memstack[mempointer] = calcstack[calcstacktop];
						calcstacktop--;
						break;
					case mop:
						calcstacktop++;
						calcstack[calcstacktop] = memstack[mempointer];
						break;
					default: break;
				}
				break;
			case adr:
				switch(code[i].opCode) {
					case top:
						calcstacktop = code[i].val;
						break;
					case ttp:
						calcstacktop += code[i].val;
						break;
					case swp:
						calcstack[calcstacktop+code[i].val] = calcstack[calcstacktop];
						break;
					default: break;
				}
			case scp:
				switch(code[i].opCode) {
					case nsc:
						scopestacktop++;
						scopememory[scopestacktop] = memstacktop;
						break;
					case dsc:
						scopestacktop--;
						break;
					case dls:
						memstacktop = scopememory[scopestacktop];
						break;
					case gts:
						i = scopestack[scopestacktop];
						scopestacktop--;
						break;
					default: break;
				}
			case opr:
				switch(code[i].opCode) {
					case neg:
						calcstack[calcstacktop] *= -1;
						break;
					case add:
						calcstack[calcstacktop-1] += calcstack[calcstacktop];
						calcstacktop--;
						break;
					case sub:
						calcstack[calcstacktop-1] -= calcstack[calcstacktop];
						calcstacktop--;
						break;
					case mul:
						calcstack[calcstacktop-1] *= calcstack[calcstacktop];
						calcstacktop--;
						break;
					case dvs:
						calcstack[calcstacktop-1] /= calcstack[calcstacktop];
						calcstacktop--;
						break;
					case eql:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] == calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case lss:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] > calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case grt:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] < calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case neq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] != calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case lsq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] >= calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case grq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] <= calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case inv:
						calcstack[calcstacktop] = int(not calcstack[calcstacktop]);
						break;
					case prt:
						printf("%d", calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case pch:
						printf("%c", calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case inp:
						int x;
						scanf("%d", x);
						calcstacktop++;
						calcstack[calcstacktop] = x;
						break;
					default:
						break;
				}
			default:
				break;
		}
		i++;
		printf("\n");
		printf("calcstacktop:%d\n", calcstacktop);
		printf("memstacktop:%d\n", memstacktop);
		for(int j = 0; j<10; j++) {
			printf("%d ", calcstack[j]);
		}
		printf("\n");
		for(int j = 0; j<10; j++) {
			printf("%d ", memstack[j]);
		}
		printf("\n\n");
	}
//	cout<<clock()-st<<"\n";
}

