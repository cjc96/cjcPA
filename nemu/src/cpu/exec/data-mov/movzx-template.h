#include "cpu/exec/template-start.h"

#define instr movzb
#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute() {
    unsigned char val = op_src->val;
    unsigned int result = val;
    OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movzw
#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute() {
    unsigned short val = op_src->val;
    unsigned int result = val;
    OPERAND_W(op_dest, result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr



#include "cpu/exec/template-end.h"
