#include "cpu/exec/template-start.h"

#define instr jmpnear

static void do_execute () {

	cpu.eip = op_src->val;
	
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
