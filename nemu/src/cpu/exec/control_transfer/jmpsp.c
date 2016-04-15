#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(jmpsp) 
{
	int16_t temp1 = swaddr_read(cpu.eip + 1, 2);
	int32_t temp2 = swaddr_read(cpu.eip + 3, 4);
	cpu.CS = temp1;
	cpu.eip = temp2;	

	extern int do_jmpnear;
	do_jmpnear = 1;

	print_asm("ljmp $0x%x, $0x%x", temp1, temp2);
	return 1;
}
