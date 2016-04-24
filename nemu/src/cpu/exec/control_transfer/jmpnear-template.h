#include "cpu/exec/template-start.h"

#define instr jmpnear

static void do_execute () {
#ifndef PAGE
	cpu.eip = op_src->val;
#endif
#ifdef PAGE
	cpu.eip = op_src->val;
#endif
	extern int do_jmpnear;
	do_jmpnear = 1;
	
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
