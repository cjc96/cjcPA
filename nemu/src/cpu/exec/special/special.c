#include "cpu/exec/helper.h"
#include "monitor/monitor.h"

make_helper(inv) {
	/* invalid opcode */

	uint32_t temp[8];
	temp[0] = instr_fetch(eip, 4);
	temp[1] = instr_fetch(eip + 4, 4);

	uint8_t *p = (void *)temp;
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n", 
			eip, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

	extern char logo [];
	printf("There are two cases which will trigger this unexpected exception:\n\
1. The instruction at eip = 0x%08x is not implemented.\n\
2. Something is implemented incorrectly.\n", eip);
	printf("Find this eip value(0x%08x) in the disassembling result to distinguish which case it is.\n\n", eip);
	printf("\33[1;31mWash Wash Sleep\n%s\n\33[0m\n\n", logo);

	assert(0);
}

make_helper(nemu_trap) {
	print_asm("nemu trap (eax = %d)", cpu.eax);
	int temp;

	switch(cpu.eax) {
		case 2:
			for (temp = 0; temp < cpu.edx; temp++)
				printf("%c", swaddr_read(cpu.ecx + temp, 1, SEG_TYPE_DS));
		   	break;

		default:
			printf("\33[1;31mnemu: HIT %s TRAP\33[0m at eip = 0x%08x\n\n",
					(cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
			nemu_state = END;
	}

	return 1;
}

