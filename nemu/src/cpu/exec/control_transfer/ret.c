#include "cpu/exec/helper.h"

/* for instruction encoding overloading */

make_helper(ret)
{
#ifdef SEGMENT
	cpu.eip = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
#endif
#ifndef SEGMENT
	cpu.eip = swaddr_read(cpu.esp, 4);
#endif
	cpu.esp += 4;

	print_asm("ret");
	return 1;
}

make_helper(ret_i_w)
{
#ifdef SEGMENT
	cpu.eip = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
#endif
#ifndef SEGMENT
	cpu.eip = swaddr_read(cpu.esp, 4);
#endif
	cpu.esp += 4;
	int16_t temp = (op_src->val & 0x0000ffff);
	cpu.esp += (int32_t)temp;

	print_asm("ret");
	return 1;
}

