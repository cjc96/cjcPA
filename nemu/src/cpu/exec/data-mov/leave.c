#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(leave)
{
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	
	print_asm("nop");
	return 1;
}
