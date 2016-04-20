#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_, SUFFIX)) {

	if (DATA_BYTE == 1)
	{
#ifndef SEGMENT
		swaddr_write(cpu.edi, DATA_BYTE, cpu.al);
#endif
#ifdef SEGMENT
        swaddr_write(cpu.edi, DATA_BYTE, cpu.al, SEG_TYPE_ES);
#endif
		if (!cpu.DF)
			cpu.edi += 1;
		else
			cpu.edi -= 1;
	}
	else if (DATA_BYTE == 2)
	{
#ifndef SEGMENT
		swaddr_write(cpu.edi, DATA_BYTE, cpu.ax);
#endif
#ifdef SEGMENT
		swaddr_write(cpu.edi, DATA_BYTE, cpu.ax, SEG_TYPE_ES);
#endif
		if (!cpu.DF)
			cpu.edi += 2;
		else
			cpu.edi -= 2;
	}
	else
	{
#ifndef SEGMENT
		swaddr_write(cpu.edi, DATA_BYTE, cpu.eax);
#endif
#ifdef SEGMENT
	swaddr_write(cpu.edi, DATA_BYTE, cpu.eax, SEG_TYPE_ES);
#endif	
		if (!cpu.DF)
			cpu.edi += 4;
		else
			cpu.edi -= 4;
	}
	
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
