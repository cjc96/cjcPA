#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
#ifdef SEGMENT
	OPERAND_W(op_src, swaddr_read(cpu.esp, DATA_BYTE, SEG_TYPE_SS));
#endif
#ifdef SEGMENT
	OPERAND_W(op_src, swaddr_read(cpu.esp, DATA_BYTE));
#endif
	cpu.esp += DATA_BYTE;
	
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
