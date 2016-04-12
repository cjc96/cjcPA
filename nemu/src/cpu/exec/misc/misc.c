#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cwtl) 
{
	int16_t temp1 = cpu.ax;
	int32_t temp2 = temp1;
	cpu.eax = temp2;	
	print_asm("cwtl");
	
	return 1;
}

make_helper(nop) 
{
	print_asm("nop");
	
	return 1;
}

make_helper(int3) 
{
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}

make_helper(lea) 
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	
	return 1 + len;
}

make_helper(cltd) 
{
	if (cpu.eax>>31)
		cpu.edx = 0xFFFFFFFF;
	else
		cpu.edx = 0;
	print_asm("cltd");
	
	return 1;
}

make_helper(cld) 
{
	cpu.DF = 0;
	print_asm("cld");
	
	return 1;
}

