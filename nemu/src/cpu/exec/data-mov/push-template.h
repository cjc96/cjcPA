#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	cpu.esp -= 4;
#ifdef SEGMENT
	swaddr_write(cpu.esp, DATA_BYTE, op_src->val, SEG_TYPE_SS);
#endif
#ifndef SEGMENT
	swaddr_write(cpu.esp, DATA_BYTE, op_src->val);
#endif
	
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
