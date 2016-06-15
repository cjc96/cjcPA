#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)) {	

	int incdec;
	
	
	
#ifndef SEGMENT	
		swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
#endif
#ifdef SEGMENT
        swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE, SEG_TYPE_DS), SEG_TYPE_ES);
#endif
	
		if (!cpu.DF)
			incdec = DATA_BYTE;
		else
			incdec = -DATA_BYTE;
	
	cpu.esi += incdec;
	cpu.edi += incdec;
	
	print_asm_template1();
	return 1;
}

#include "cpu/exec/template-end.h"
