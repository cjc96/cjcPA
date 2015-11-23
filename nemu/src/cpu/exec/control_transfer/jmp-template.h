#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	op_src->val = (DATA_TYPE_S)((DATA_TYPE)op_src->val);
	cpu.eip += op_src->val;
	
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
