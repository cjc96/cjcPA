#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_, SUFFIX)) {	

	int incdec;
	
	if (DATA_BYTE == 1)
	{
		int sin = 1, cin = 1;
#ifndef SEGMENT
		int ain = swaddr_read(cpu.esi, 1), bin = swaddr_read(cpu.edi, 1);
#endif
#ifdef SEGMENT
		int ain = swaddr_read(cpu.esi, 1, SEG_TYPE_DS), bin = swaddr_read(cpu.edi, 1, SEG_TYPE_DS);
#endif
		set_eflags(ain, bin, sin, cin);
	
		if (!cpu.DF)
			incdec = 1;
		else
			incdec = -1;
	}
	else if (DATA_BYTE == 2)
	{
		int sin = 1,cin = 1;
#ifndef SEGMENT
		int ain = swaddr_read(cpu.esi, 2), bin = swaddr_read(cpu.edi, 2);
#endif
#ifdef SEGMENT
		int ain = swaddr_read(cpu.esi, 2, SEG_TYPE_DS), bin = swaddr_read(cpu.edi, 2, SEG_TYPE_DS);
#endif
		set_eflags(ain, bin, sin, cin);
	
		if (!cpu.DF)
			incdec = 2;
		else
			incdec = -2;
	}
	else
	{
		int sin = 1,cin = 1;
#ifndef SEGMENT
		int ain = swaddr_read(cpu.esi, 4), bin = swaddr_read(cpu.edi, 4);
#endif
#ifdef SEGMENT
		int ain = swaddr_read(cpu.esi, 4, SEG_TYPE_DS), bin = swaddr_read(cpu.edi, 4, SEG_TYPE_DS);
#endif
		set_eflags(ain, bin, sin, cin);
	
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
