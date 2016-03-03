#include "cpu/exec/template-start.h"

#define instr movsb
#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute() {
    signed char val = op_src->val;
    signed int result = val;
    OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movsw
#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute() {
    signed short val = op_src->val;
    signed int result = val;
    OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr



#include "cpu/exec/template-end.h"
