#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {

	/* TODO: Update EFLAGS. */
	int temp[32] , i ;
	cpu.OF = 0;
	cpu.CF = 0;
	cpu.ZF = 1;
	for (i = 0; i < DATA_BYTE; i++)
	{
		temp[i] = ((op_dest->val >> i) & 1) & ((op_src->val >> i) & 1);
		if (temp[i])
			cpu.ZF = 0;
	}
	cpu.SF = temp[i - 1];
	for (i = 0; i < 7; i++)
		cpu.PF ^= temp[i];
	cpu.PF = 1 - cpu.PF;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
