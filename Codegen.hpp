#include "Table.hpp"
#define MAXCODE 200	
#define MAXMEM 2000	
#define MAXREG 20
#define MAXLEVEL 5
#include <stdio.h>

// jmp: jumps to another code if the top value of Calcstack is not 0, jmp {CodeNumber} ex) jmp 3 
// cal: jumps to another code with no condition, cal {CodeNumber} ex) cal 3
// psh: pushes number into Calcstack, push {num} ex) push 5
// def: moves the top value of Calcstack into the Memstack, def {AddressOfVariable} ex) def 0
// mtp: sets the top value as the pointer of Memstack, mtp ex) mtp
// sav: saves the top value to the Memstack with the pointer, sav ex) sav
// mop: moves the Memstack with the pointer into Calcstack, mop ex) mop
// mov: pushes a variable's value into Calcstack, mov {AddressOfVariable} ex) mov 1
// ret: jumps to the line of top value of Calcstack, ret, ex) ret
// top: changes the top address of Calcstack, top {Address} ex) top 1
// ttp: changes the top address of Calcstack by delta, top {+-deltaAddress} ex) top -1
// swp: changes the Calcstack with the top value, swp {Address} ex) swp 3
// <oprerators>: Only interacts with Calcstack
// neg: changes the sign of the top value, opr neg
// add: adds the top two values, opr add
// sub: subtracts the top two values, opr sub
// mul: multiplies the top two values, opr sub
// dvs: divides the top two values in an integer value, opr sub
// eql: checks if the top two values are equal, opr eql
// lss: checks if the second value is smaller than the first value, opr lss
// grt: checks if the second value is greater than the first value, opr grt
// neq: checks if the top two values are different, opr neq
// lsq: checks if the second value is smaller than or equal with the first value, opr lsq
// grq: checks if the second value is greater than or equal with the first value, opr grq
// inv: returns 1 if the top value is 0, 0 if the top value isn't 0, opr inv
// prt: prints the top value
// pch: prints the top value as char
// inp: inputs one value and pushes it

typedef enum codes {
	jmp, cal, psh, def, mtp, sav, mop, mov, ret, opr, top, ttp, swp
} OpCode;

typedef enum ops {
	neg, add, sub, mul, dvs, eql, lss, grt, neq, lsq, grq, inv, prt, pch, inp, nothing
} Operator;


class Inst {
public:
	OpCode opCode;
	int val = 0;
	Operator oprCode;
};

Inst code[200];	
int cIndex = 0;
void printCode(int i);
int Calcstacktop = -1, Memstacktop = 0;

void genCode(OpCode op, int val, Operator oper) {
	code[cIndex].opCode = op;
	code[cIndex].val = val;
	code[cIndex].oprCode = oper;
	cIndex++;
	switch(op) {
		case psh: case mov: case mop:
			Calcstacktop++; break;
		case def: case mtp: case sav: Calcstacktop--; break;
		case top: Calcstacktop = val; break;
		case ttp: Calcstacktop += val; break;
		case opr:
			switch(oper) {
				case add: case sub: case mul: case dvs: case eql: case lss: case grt: case neq: case lsq: case grq: 
				Calcstacktop--; break;
				default: break;
			}
			break;
		default: break;
	}
}
/*
void genCodeS(OpCode op, int v) {
	code[cIndex].opCode = op;
	code[cIndex].addr1 = v;
	cIndex++; top = v+1;
	cout<<top<<"??";
}

void genCodeV(OpCode op, int v)	
{
	code[cIndex].opCode = op;
	code[cIndex].addr1 = v;
	code[cIndex].addr2 = top;
	cIndex++; top++;
}

int genCodeT(OpCode op, string ti, int first)	
{
	code[cIndex].opCode = op;
	if(first==-1) {
		code[cIndex].addr1 = vartable.search(ti).addr;
		code[cIndex].addr2 = top; top++;
	}
	else {
		code[cIndex].addr1 = top-1;
		code[cIndex].addr2 = vartable.search(ti).addr;
	}
	cIndex++;
	cout<<top<<"!!"<<"\n";
	return cIndex;
}

int genCodeO(Operator p)		
{
	code[cIndex].opCode = opr;
	code[cIndex].oprCode = p;
	cIndex++;
	top--;
	return cIndex;
}

int genCodeR()				
{
	if (code[cIndex].opCode == ret)
		return cIndex;	
	code[cIndex].opCode = ret;
	// code[cIndex].u.addr.level = bLevel();
	// code[cIndex].u.addr.addr = fPars();	
	cIndex++;
	return cIndex;
}
	
void backPatch(int i)
{
	code[i].addr1 = cIndex+1;
}*/

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
	switch(code[i].opCode) {
		case jmp: printf("jmp "); break;
		case cal: printf("cal "); break;
		case psh: printf("psh "); break;
		case def: printf("def "); break;
		case mtp: printf("mtp "); break;
		case sav: printf("sav "); break;
		case mop: printf("mop "); break;
		case mov: printf("mov "); break;
		case ret: printf("ret "); break;
		case top: printf("top "); break;
		case ttp: printf("ttp "); break;
		case swp: printf("swp "); break;
		case opr:
			printf("opr ");
			switch(code[i].oprCode) {
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
	if(code[i].opCode!=opr) {
		printf("%d ", code[i].val);
	}
	printf("\n");
}	

void execute()
{
	int memstack[MAXMEM];
	int calcstack[MAXMEM];
	int calcstacktop = -1;
	int mempointer = 0;
	int i = 0;
	while(i<cIndex) {
		switch(code[i].opCode) {
			case jmp:
				if(calcstack[calcstacktop]!=0) i = code[i].val - 1;
				break;
			case cal:
				i = code[i].val - 1;
				break;
			case psh:
				calcstacktop++;
				calcstack[calcstacktop] = code[i].val;
				break;
			case def:
				memstack[code[i].val] = calcstack[calcstacktop];
				calcstacktop--;
				break;
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
			case mov:
				calcstacktop++;
				calcstack[calcstacktop] = memstack[code[i].val];
				break;
			case ret:
				i = calcstack[calcstacktop] - 1;
				break;
			case top:
				calcstacktop = code[i].val;
				break;
			case ttp:
				calcstacktop += code[i].val;
				break;
			case swp:
				calcstack[calcstacktop+code[i].val] = calcstack[calcstacktop];
				break;
			case opr:
				switch(code[i].oprCode) {
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
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] < calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case grt:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] > calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case neq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] != calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case lsq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] <= calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case grq:
						calcstack[calcstacktop-1] = (calcstack[calcstacktop-1] >= calcstack[calcstacktop]);
						calcstacktop--;
						break;
					case inv:
						calcstack[calcstacktop] = int(not calcstack[calcstacktop]);
						break;
					case prt:
						printf("%d", calcstack[calcstacktop]);
						break;
					case pch:
						printf("%c", calcstack[calcstacktop]);
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
//		printf("&&%d\n", i);
//		for(int j = 0; j<10; j++) {
//			printf("%d ", calcstack[j]);
//		}
//		printf("\n");
//		for(int j = 0; j<10; j++) {
//			printf("%d ", memstack[j]);
//		}
//		printf("\n\n");
	}
}

