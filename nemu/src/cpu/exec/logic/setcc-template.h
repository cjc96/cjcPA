#include "cpu/exec/template-start.h"

#define instr setcc

static void do_execute () {
	int op_temp = instr_fetch(cpu.eip + 1,1);
	
	switch (op_temp)
	{
		case 0x90 : if (cpu.OF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x91 : if (!cpu.OF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x92 : if (cpu.CF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x93 : if (!cpu.CF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x94 : if (cpu.ZF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x95 : if (!cpu.ZF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x96 : if (cpu.CF || cpu.ZF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x97 : if (!cpu.CF && !cpu.ZF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x98 : if (cpu.SF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x99 : if (!cpu.SF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9a : if (cpu.PF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9b : if (!cpu.PF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9c : if (cpu.SF != cpu.OF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9d : if (cpu.SF == cpu.OF) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9e : if (cpu.ZF || (cpu.SF != cpu.OF)) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break;
		case 0x9f : if (!cpu.ZF && (cpu.SF == cpu.OF)) OPERAND_W(op_src, 1); else OPERAND_W(op_src, 0); break; 
		default : panic("Please implent me!");
	}
	
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
