#include "cpu/exec/helper.h"

/* for instruction encoding overloading */

make_helper(ret)
{
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += 4;

	print_asm("ret");
	return 0;
}

make_helper(ret_i_w)
{
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	int16_t temp = (op_src->val & 0x0000ffff);
	cpu.esp += (int32_t)temp;

	print_asm("ret");
	return 0;
}

