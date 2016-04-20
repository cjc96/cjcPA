#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {	

	int incdec;
	
	if (DATA_BYTE == 1)
	{
#ifndef SEGMENT	
		swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
#endif
#ifdef SEGMENT
        swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, SEG_TYPE_DS), SEG_TYPE_DS);
#endif
	
		if (!cpu.DF)
			incdec = 1;
		else
			incdec = -1;
	}
	else if (DATA_BYTE == 2)
	{
	
#ifndef SEGMENT	
		swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
#endif
#ifdef SEGMENT
        swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, SEG_TYPE_DS), SEG_TYPE_DS);
#endif
	
		if (!cpu.DF)
			incdec = 2;
		else
			incdec = -2;
	}
	else
	{

#ifndef SEGMENT	
		swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
#endif
#ifdef SEGMENT
        swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, SEG_TYPE_DS), SEG_TYPE_DS);
#endif
	
		if (!cpu.DF)
			incdec = 4;
		else
			incdec = -4;
	}
	cpu.esi += incdec;
	cpu.edi += incdec;
	
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
