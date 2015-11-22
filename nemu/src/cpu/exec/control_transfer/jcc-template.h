#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute () {
	printf("%d\n",op_src->val);
	switch (swaddr_read(cpu.eip,1))
	{
		case 0x74 : if (cpu.ZF) cpu.eip += op_src->val; break;
		default : panic("Please implent me!");
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
