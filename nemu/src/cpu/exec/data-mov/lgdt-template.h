#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute () {
#ifdef SEGMENT
	cpu.gdtr.limit = swaddr_read(op_src->addr, 2, SEG_TYPE_DS);
	cpu.gdtr.base = swaddr_read(op_src->addr + 2, 4, SEG_TYPE_DS);
#endif
#ifndef SEGMENT
	cpu.gdtr.limit = swaddr_read(op_src->addr, 2);
	cpu.gdtr.base = swaddr_read(op_src->addr + 2, 4);
#endif

	
	print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
