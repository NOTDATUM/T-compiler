#include <stdio.h>
#include "Codegen.hpp"
const int N = 11;
Inst Vircode[N] = {
	Inst(cod, gto, 5),
	Inst(mem, mov, -1),
	Inst(opr, prt, 0),
	Inst(scp, dls, 0),
	Inst(scp, gts, 0),
	Inst(scp, nsc, 0),
	Inst(mem, psh, 1),
	Inst(mem, def, 0),
	Inst(cod, cal, 1),
	Inst(scp, dls, 0),
	Inst(scp, dsc, 0)
};

int main() {
	for(int i = 0; i<N; i++) {
		genCode(Vircode[i].tpCode, Vircode[i].opCode, Vircode[i].val);
	}
	listCode();
	execute();
}
