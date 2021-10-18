#include "Table.hpp"
#define MAXCODE 200	
#define MAXMEM 2000	
#define MAXREG 20
#define MAXLEVEL 5
#include <stdio.h>

typedef enum codes {
	jmp, def, psh, mov, cmp, opr, ret, cal
} OpCode;

typedef enum ops {
	neg, add, sub, mul, dvs, eql, lss, grt, neq, lsq, grq, str
} Operator;

class instcode {
public:
	int value;
	VariableStore v;
	Operator optr;
};

typedef struct inst{
	OpCode opCode;
	instcode u;
} Inst;

Inst code[200];	
int cIndex = -1;
void checkMax();
void printCode(int i);

int nextCode()
{
	return cIndex+1;
}

int genCodeV(OpCode op, int v)	
{
	checkMax();
	code[cIndex].opCode = op;
	code[cIndex].u.value = v;
	return cIndex;
}

int genCodeT(OpCode op, VariableStore ti)	
{
	checkMax();
	code[cIndex].opCode = op;
	code[cIndex].u.v = ti;
	return cIndex;
}

int genCodeO(Operator p)		
{
	checkMax();
	code[cIndex].opCode = opr;
	code[cIndex].u.optr = p;
	return cIndex;
}

int genCodeR()				
{
	if (code[cIndex].opCode == ret)
		return cIndex;		
	checkMax();
	code[cIndex].opCode = ret;
	// code[cIndex].u.addr.level = bLevel();
	// code[cIndex].u.addr.addr = fPars();	
	return cIndex;
}

void checkMax()	
{
	if (++cIndex < MAXCODE)
		return;
	// errorF("too many code");
}
	
void backPatch(int i)
{
	code[i].u.value = cIndex+1;
}

void listCode()	
{
	int i;
	/* printf("\ncode\n");
	for(i=0; i<=cIndex; i++){
		printf("%3d: ", i);
		printCode(i);
	}*/
}

void printCode(int i)
{
	int flag;
	switch(code[i].opCode){
	case psh: printf("psh"); flag=1; break;
	case opr: printf("opr"); flag=3; break;
	case mov: printf("mov"); flag=2; break;
	case cal: printf("cal"); flag=2; break;
	case ret: printf("ret"); flag=2; break;
	case def: printf("def"); flag=1; break;
	case jmp: printf("jmp"); flag=1; break;
	case cmp: printf("cmp"); flag=1; break;
	}
	switch(flag){
	case 1:
		printf(",%d\n", code[i].u.value);
		return;
	case 2:
		//printf(",%d", code[i].u.addr.level);
		//printf(",%d\n", code[i].u.addr.addr);
		return;
	case 3:
		switch(code[i].u.optr){
		case neg: printf(",neg\n"); return;
		case add: printf(",add\n"); return;
		case sub: printf(",sub\n"); return;
		case mul: printf(",mul\n"); return;
		case dvs: printf(",dvs\n"); return;
		case eql: printf(",eq\n"); return;
		case lss: printf(",lss\n"); return;
		case grt: printf(",grt\n"); return;
		case neq: printf(",neq\n"); return;
		case lsq: printf(",lsq\n"); return;
		case grq: printf(",grq\n"); return;
		case str: printf(",str\n"); return;
		}
	}
}	

void execute()
{
	int stack[MAXMEM];
	int display[MAXLEVEL];
	int pc, top, lev, temp;
	Inst i;
	printf("start execution\n");
	top = 0;  pc = 0;
	stack[0] = 0;  stack[1] = 0;
	display[0] = 0;	
	do {
		i = code[pc++];		
		switch(i.opCode){
		case psh: stack[top++] = i.u.value; 
				break;
		case mov: //stack[top++] = stack[display[i.u.addr.level] + i.u.addr.addr]; 
				 break;
		case cal: //lev = i.u.addr.level +1;
				stack[top] = display[lev]; 
				stack[top+1] = pc; display[lev] = top; 
				//pc = i.u.addr.addr;
				 break;
		case ret: temp = stack[--top];	
				//top = display[i.u.addr.level];  
				//display[i.u.addr.level] = stack[top];
				pc = stack[top+1];
				//top -= i.u.addr.addr;
				stack[top++] = temp;
				break;
		case def: top += i.u.value; 
				if (top >= MAXMEM-MAXREG)
					//errorF("stack overflow");
				break;
		case jmp: pc = i.u.value; break;
		case cmp: if (stack[--top] == 0)
					pc = i.u.value;
				break;
		case opr: 
			switch(i.u.optr){
			case neg: stack[top-1] = -stack[top-1]; continue;
			case add: --top;  stack[top-1] += stack[top]; continue;
			case sub: --top; stack[top-1] -= stack[top]; continue;
			case mul: --top;  stack[top-1] *= stack[top];  continue;
			case dvs: --top;  stack[top-1] /= stack[top]; continue;
			case eql: --top;  stack[top-1] = (stack[top-1] == stack[top]); continue;
			case lss: --top;  stack[top-1] = (stack[top-1] < stack[top]); continue;
			case grt: --top;  stack[top-1] = (stack[top-1] > stack[top]); continue;
			case neq: --top;  stack[top-1] = (stack[top-1] != stack[top]); continue;
			case lsq: --top;  stack[top-1] = (stack[top-1] <= stack[top]); continue;
			case grq: --top;  stack[top-1] = (stack[top-1] >= stack[top]); continue;
			case str: printf("%d ", stack[--top]); continue;
			}
		}
	} while (pc != 0);
}

