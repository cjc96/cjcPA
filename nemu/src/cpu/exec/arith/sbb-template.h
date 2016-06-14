#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute () {
	DATA_TYPE result;
	DATA_TYPE ain = op_dest->val, bin;
	
	if ((sizeof(op_src->val) == 8) && (sizeof(op_dest->val) == 16 || sizeof(op_dest->val) == 32))
	{
		result = op_dest->val - ((DATA_TYPE_S)op_src->val + cpu.CF);
		bin = (DATA_TYPE_S)op_src->val + cpu.CF;
	}
	else
	{
		result = op_dest->val - (op_src->val + cpu.CF);
		bin = op_src->val + cpu.CF;
	}
	OPERAND_W(op_dest, result);
	
	
	int sin = 1, cin = 1;
	set_eflags(ain,bin,sin,cin);
	
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
