#include "cpu/exec/template-start.h"

#define instr movzbl

static void do_execute () {
	DATA_TYPE temp = op_src->val;
	unsigned int result = temp;
	OPERAND_W(op_dest, result);
	//printf("%d\n",DATA_BYTE);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
