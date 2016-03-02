#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	int temp[32] , i ;
	cpu.OF = 0;
	cpu.CF = 0;
	cpu.ZF = 1;
	for (i = 0; i < DATA_BYTE * 8; i++)
	{
		temp[i] = ((op_dest->val >> i) & 1) & ((op_src->val >> i) & 1);
		if (temp[i])
			cpu.ZF = 0;
	}
	cpu.SF = temp[i - 1];
	cpu.PF = temp[0];
	for (i = 1; i < 8; i++)
		cpu.PF ^= temp[i];
	cpu.PF = 1 - cpu.PF;

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
