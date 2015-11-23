#include "cpu/exec/template-start.h"

#define instr jcc

static void do_execute () {
	
	switch (swaddr_read(cpu.eip,1))
	{
		case 0x70 : if (cpu.OF) cpu.eip += op_src->val; break;
		case 0x71 : if (!cpu.OF) cpu.eip += op_src->val; break;
		case 0x72 : if (cpu.CF) cpu.eip += op_src->val; break;
		case 0x73 : if (!cpu.CF) cpu.eip += op_src->val; break;
		case 0x74 : if (cpu.ZF) cpu.eip += op_src->val; break;
		case 0x75 : if (!cpu.ZF) cpu.eip += op_src->val; break;
		case 0x76 : if ((cpu.CF) || (cpu.ZF)) cpu.eip += op_src->val; break;
		case 0x77 : if ((!cpu.CF) && (!cpu.ZF)) cpu.eip += op_src->val; break;
		case 0x78 : if (cpu.SF) cpu.eip += op_src->val; break;
		case 0x79 : if (!cpu.SF) cpu.eip += op_src->val; break;
		case 0x7a : if (cpu.PF) cpu.eip += op_src->val; break;
		case 0x7b : if (!cpu.PF) cpu.eip += op_src->val; break;
		case 0x7c : if (cpu.SF != cpu.OF) cpu.eip += op_src->val; break;
		case 0x7d : if (cpu.SF == cpu.OF) cpu.eip += op_src->val; break;
		case 0x7e : if ((cpu.ZF) || (cpu.SF != cpu.OF)) cpu.eip += op_src->val; break;
		case 0x7f : if ((!cpu.ZF) && (cpu.SF == cpu.OF)) cpu.eip += op_src->val; break;
		case 0xe3 : if (!cpu.cx) cpu.eip += op_src->val; break;
		default : panic("Please implent me!");
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
