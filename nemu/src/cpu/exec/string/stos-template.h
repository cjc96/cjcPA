#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_, SUFFIX)) {
	printf("%d %d\n",cpu.edi,cpu.eax);
	if (DATA_BYTE == 1)
	{
		swaddr_write(cpu.edi, DATA_BYTE, cpu.al);
		if (!cpu.DF)
			cpu.edi += 1;
		else
			cpu.edi -= 1;
	}
	else if (DATA_BYTE == 2)
	{
		swaddr_write(cpu.edi, DATA_BYTE, cpu.ax);
		if (!cpu.DF)
			cpu.edi += 2;
		else
			cpu.edi -= 2;
	}
	else
	{
		swaddr_write(cpu.edi, DATA_BYTE, cpu.eax);
		if (!cpu.DF)
			cpu.edi += 4;
		else
			cpu.edi -= 4;
	}
	
	print_asm_template1();
	return 1;
}

//make_instr_helper()

#include "cpu/exec/template-end.h"
