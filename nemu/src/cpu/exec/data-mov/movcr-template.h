#include "cpu/exec/template-start.h"

#define instr movcr

static void do_execute () {
	uint32_t temp = instr_fetch(cpu.eip + 1, 1), rmcode = (instr_fetch(cpu.eip + 2, 1) >> 3) & 0x7;
	
	if (temp == 0x20)
	{
		if (rmcode == 0)
			cpu.eax = cpu.cr0.val;
		else if (rmcode == 3)
			cpu.eax = cpu.cr3.val;
		else
			assert(0);
	}
	else
	{
		if (rmcode == 0)
			cpu.cr0.val = op_dest->val;
		else if (rmcode == 3)
			cpu.cr3.val = op_dest->val;
		else
			assert(0);
	}
	
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
