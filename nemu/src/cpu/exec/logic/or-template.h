#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);

	cpu.SF = result >> (DATA_BYTE * 8 -1);
	cpu.ZF = !result;
	
	int i, temp_ans= 1;
	for (i = 0; i < 8; i++)
	{
		if ((result >> i) & 1)
		{
			temp_ans ^= 1;
		}
	}
	cpu.PF = temp_ans;
	
	cpu.CF = 0;
	cpu.OF = 0;
	
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
