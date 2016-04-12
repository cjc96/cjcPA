#include "cpu/exec/template-start.h"

#define instr movsp

static void do_execute () {
	uint32_t temp = swaddr_read(cpu.eip, 1), rmcode = (swaddr_read(cpu.eip + 1, 1) >> 3) & 0x7;
	printf("%u %u", temp, rmcode);
	if (temp == 0x20)
	{
		if (rmcode == 0)
			op_dest->val = cpu.cr0.val;
		else if (rmcode == 3)
			op_dest->val = cpu.cr3.val;
		else
			assert(0);
	}
	else
	{
		if (rmcode == 0)
			cpu.cr0.val = op_src->val;
		else if (rmcode == 3)
			cpu.cr3.val = op_src->val;
		else
			assert(0);
	}
	
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
