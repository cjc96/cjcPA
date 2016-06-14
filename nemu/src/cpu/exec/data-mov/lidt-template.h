#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute () {
#ifdef SEGMENT
	cpu.idtr.limit = swaddr_read(op_src->addr, 2, SEG_TYPE_DS);
	cpu.idtr.base = swaddr_read(op_src->addr + 2, 4, SEG_TYPE_DS);
#endif
	
	print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
