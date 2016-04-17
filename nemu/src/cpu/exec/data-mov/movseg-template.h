#include "cpu/exec/template-start.h"

#define instr movseg

static void do_execute () {
	uint32_t rmcode = (swaddr_read(cpu.eip + 1, 1) >> 3) & 0x7;
	
	if (rmcode == 2)
		cpu.SS = op_dest->addr;
	else if (rmcode == 3)
		cpu.DS = op_dest->addr;
	else if (rmcode == 0)
		cpu.ES = op_dest->addr;
	else
		assert(0);
	
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
