#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(jmpsp) 
{
	uint32_t temp1 = swaddr_read(cpu.eip + 1, 4);
	uint16_t temp2 = swaddr_read(cpu.eip + 3, 2);
	cpu.CS.val = temp2;
	cpu.eip = temp1;	

	extern int do_jmpnear;
	do_jmpnear = 1;

	print_asm("ljmp $0x%x, $0x%x", temp2, temp1);
	return 1;
}
