#include "cpu/exec/helper.h"

make_helper(leave)
{
	cpu.esp = cpu.ebp;
#ifdef SEGMENT
	cpu.ebp = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
#endif
#ifndef SEGMENT
    cpu.ebp = swaddr_read(cpu.esp, 4);
#endif
	cpu.esp += 4;
	
	print_asm("leave");
	return 1;
}
