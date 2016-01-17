#include "cpu/exec/template-start.h"

#define instr movzbl

static void do_execute () {
	
	op_dest->val = op_src->val;
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
