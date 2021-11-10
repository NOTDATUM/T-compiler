#include "Table.hpp"
#define MAXCODE 200	
#define MAXMEM 2000	
#define MAXREG 20
#define MAXLEVEL 5
#include <stdio.h>

// jmp: jumps to another code if the top value of Calcstack is 0, jmp {CodeNumber} ex) jmp 3 
// psh: pushes number into Calcstack, push {num} ex) push 5
// def: moves the top value of Calcstack into the Memstack, def {AddressOfVariable} ex) def 0
// mov: pushes a variable's value into Calcstack, mov {AddressOfVariable} ex) mov 1
// ret: returns Value
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
// prt: prints the top value

typedef enum codes {
	jmp, psh, def, mov, ret, opr
} OpCode;

typedef enum ops {
	neg, add, sub, mul, dvs, eql, lss, grt, neq, lsq, grq, prt, nothing
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
int Calcstacktop = 0, Memstacktop = 0;

void genCode(OpCode op, int val, Operator opr) {
	code[cIndex].opCode = op;
	code[cIndex].val = val;
	code[cIndex].oprCode = opr;
	cIndex++;
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
		case psh: printf("psh "); break;
		case def: printf("def "); break;
		case mov: printf("mov "); break;
		case ret: printf("ret "); break;
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
				case prt: printf("prt "); break;
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
	int i = 0;
	while(i<cIndex) {
		switch(code[i].opCode) {
			case jmp:
				if(calcstack[calcstacktop]==0) i = code[i].val - 1;
				break;
			case psh:
				calcstacktop++;
				calcstack[calcstacktop] = code[i].val;
				break;
			case def:
				memstack[code[i].val] = calcstack[calcstacktop];
				calcstacktop--;
				break;
			case mov:
				calcstacktop++;
				calcstack[calcstacktop] = memstack[code[i].val];
				break;
			case ret:
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
					case prt:
						printf("print:%d\n", calcstack[calcstacktop]);
						break;
					default:
						break;
				}
			default:
				break;
		}
		i++;
		for(int j = 0; j<10; j++) {
			printf("%d ", calcstack[j]);
		}
		printf("\n");
	}
}

