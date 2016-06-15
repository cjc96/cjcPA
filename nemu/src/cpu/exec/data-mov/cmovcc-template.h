#include "cpu/exec/template-start.h"

#define instr cmovcc

static void do_execute () {
	int op_temp = instr_fetch(cpu.eip + 1,1);
	
	switch (op_temp)
	{
		case 0x40 : if (cpu.OF) OPERAND_W(op_dest, op_src->val); break;
		case 0x41 : if (!cpu.OF) OPERAND_W(op_dest, op_src->val); break;
		case 0x42 : if (cpu.CF) OPERAND_W(op_dest, op_src->val); break;
		case 0x43 : if (!cpu.CF) OPERAND_W(op_dest, op_src->val); break;
		case 0x44 : if (cpu.ZF) OPERAND_W(op_dest, op_src->val); break;
		case 0x45 : if (!cpu.ZF) OPERAND_W(op_dest, op_src->val); break;
		case 0x46 : if (cpu.CF || cpu.ZF) OPERAND_W(op_dest, op_src->val); break;
		case 0x47 : if (!cpu.CF && !cpu.ZF) OPERAND_W(op_dest, op_src->val); break;
		case 0x48 : if (cpu.SF) OPERAND_W(op_dest, op_src->val); break;
		case 0x49 : if (!cpu.SF) OPERAND_W(op_dest, op_src->val); break;
		case 0x4a : if (cpu.PF) OPERAND_W(op_dest, op_src->val); break;
		case 0x4b : if (!cpu.PF) OPERAND_W(op_dest, op_src->val); break;
		case 0x4c : if (cpu.SF != cpu.OF) OPERAND_W(op_dest, op_src->val); break;
		case 0x4d : if (cpu.SF == cpu.OF) OPERAND_W(op_dest, op_src->val); break;
		case 0x4e : if (cpu.ZF || (cpu.SF != cpu.OF)) OPERAND_W(op_dest, op_src->val); break;
		case 0x4f : if (!cpu.ZF && (cpu.SF == cpu.OF)) OPERAND_W(op_dest, op_src->val); break; 
		default : panic("Please implent me!");
	}
	
	print_asm_template1();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
