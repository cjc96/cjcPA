#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_, SUFFIX)) {	

	int incdec;
	
	
		int sin = 1, cin = 1;
#ifndef SEGMENT
		int ain = swaddr_read(cpu.esi, DATA_BYTE), bin = swaddr_read(cpu.edi, DATA_BYTE);
#endif
#ifdef SEGMENT
		int ain = swaddr_read(cpu.esi, DATA_BYTE, SEG_TYPE_DS), bin = swaddr_read(cpu.edi, DATA_BYTE, SEG_TYPE_ES);
#endif
		set_eflags(ain, bin, sin, cin);
	
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
