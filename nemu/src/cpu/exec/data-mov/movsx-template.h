#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute () {
	
	DATA_TYPE_S temp = op_src->val;
	OPERAND_W(op_dest, temp);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
