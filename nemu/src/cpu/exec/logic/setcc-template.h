#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	int op_temp = swaddr_read(cpu.eip + 1,1);
	printf("%x\n", op_src->val);
	switch (op_temp)
	{
		case 0x90 : if (cpu.OF) op_src->val = 1; else op_src->val = 0; break;
		case 0x91 : if (!cpu.OF) op_src->val = 1; else op_src->val = 0; break;
		case 0x92 : if (cpu.CF) op_src->val = 1; else op_src->val = 0; break;
		case 0x93 : if (!cpu.CF) op_src->val = 1; else op_src->val = 0; break;
		case 0x94 : if (cpu.ZF) op_src->val = 1; else op_src->val = 0; break;
		case 0x95 : if (!cpu.ZF) op_src->val = 1; else op_src->val = 0; break;
		case 0x96 : if (cpu.CF || cpu.ZF) op_src->val = 1; else op_src->val = 0; break;
		case 0x97 : if (!cpu.CF && !cpu.ZF) op_src->val = 1; else op_src->val = 0; break;
		case 0x98 : if (cpu.SF) op_src->val = 1; else op_src->val = 0; break;
		case 0x99 : if (!cpu.SF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9a : if (cpu.PF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9b : if (!cpu.PF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9c : if (cpu.SF != cpu.OF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9d : if (cpu.SF == cpu.OF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9e : if (cpu.ZF || cpu.SF != cpu.OF) op_src->val = 1; else op_src->val = 0; break;
		case 0x9f : if (cpu.ZF || cpu.SF == cpu.OF) op_src->val = 1; else op_src->val = 0; break; 
		default : panic("Please implent me!");
	}
	printf("%x\n", op_src->val);
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
