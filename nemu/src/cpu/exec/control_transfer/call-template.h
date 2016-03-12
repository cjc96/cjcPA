#include "cpu/exec/template-start.h"

#define instr call

static void do_execute () {
	uint32_t temp = swaddr_read(cpu.eip, 1);
	if (temp == 0xe8)
		cpu.eip += op_src->val;
	else if (temp == 0xff)
		cpu.eip = op_src->val - 2;
	
	extern int do_call;
	do_call = 1;
	
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
