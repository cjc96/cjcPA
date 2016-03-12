#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute () {
	
	int sin = 1,cin = 1, ain = op_dest->val, bin = op_src->val;
	set_eflags(ain, bin, sin, cin);
	
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
