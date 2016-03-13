#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	int temp_flag = cpu.CF;
	uint32_t ain = op_src->val, bin = 1, sin = 1, cin = 1;
	set_eflags(ain, bin, sin, cin);
	cpu.CF = temp_flag;

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
