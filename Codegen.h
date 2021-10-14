typedef enum codes {
	lit, opr, lod, sto, cal, ret, ict, jmp, jpc
} OpCode;

typedef enum ops {
	neg, add, sub, mul, div, odd, eq, ls, gr, neq, lseq, greq, wrt, wrl
} Operator;

int genCodeV(OpCode op, int v);
int genCodeT(OpCode op, int ti);
int genCode0(Operator p);
int genCodeR();
void backPatch(int i);

int nextCode();
void listCode();
void execute();
