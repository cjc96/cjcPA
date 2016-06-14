#include "cpu/exec/template-start.h"
#include "cpu/exec/interrupt.h"

#define instr int

static void do_execute () {
	/* This is VERY IMPROPER */
	cpu.eip += 2;
	
	raise_intr(op_src->val);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
