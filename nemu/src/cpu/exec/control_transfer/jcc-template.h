#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute () {
	printf("%d",swaddr_read(cpu.eip,1));
	switch (swaddr_read(cpu.eip,1))
	{
		case 74 : if (cpu.ZF) cpu.eip += op_src->val; break;
		default : panic("Please implent me!");
	}
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
