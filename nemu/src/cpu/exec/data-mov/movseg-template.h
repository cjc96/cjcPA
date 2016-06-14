#include "cpu/exec/template-start.h"

#define instr movseg

static void do_execute () {
#ifdef SEGMENT
	uint32_t rmcode = (swaddr_read(cpu.eip + 1, 1, SEG_TYPE_CS) >> 3) & 0x7;
#endif
#ifndef SEGMENT
	uint32_t rmcode = (swaddr_read(cpu.eip + 1, 1) >> 3) & 0x7;
#endif

#ifndef SEGMENT	
	uint32_t temp_addr = 8 * ((op_dest->val >> 3) & 0x1fff) + cpu.gdtr.base;
	uint32_t temp_describe7 = swaddr_read(temp_addr, 1), temp_describe6 = swaddr_read(temp_addr + 1, 1), temp_describe5 = swaddr_read(temp_addr + 2, 1), temp_describe4 = swaddr_read(temp_addr + 3, 1), temp_describe3 = swaddr_read(temp_addr + 4, 1), temp_describe2 = swaddr_read(temp_addr + 5, 1), temp_describe1 = swaddr_read(temp_addr + 6, 1), temp_describe0 = swaddr_read(temp_addr + 7, 1);
	uint32_t temp_base = (temp_describe0 << 24) + (temp_describe3 << 16) + (temp_describe4 << 8) + temp_describe5;
	uint32_t temp_limit = ((temp_describe1 & 0xf) << 16) + (temp_describe6 << 8) +temp_describe7;
#endif

#ifdef SEGMENT
	uint32_t temp_addr = 8 * ((op_dest->val >> 3) & 0x1fff) + cpu.gdtr.base;
	uint32_t temp_describe7 = swaddr_read(temp_addr, 1, SEG_TYPE_DS), temp_describe6 = swaddr_read(temp_addr + 1, 1, SEG_TYPE_DS), temp_describe5 = swaddr_read(temp_addr + 2, 1, SEG_TYPE_DS), temp_describe4 = swaddr_read(temp_addr + 3, 1, SEG_TYPE_DS), temp_describe3 = swaddr_read(temp_addr + 4, 1, SEG_TYPE_DS), temp_describe2 = swaddr_read(temp_addr + 5, 1, SEG_TYPE_DS), temp_describe1 = swaddr_read(temp_addr + 6, 1, SEG_TYPE_DS), temp_describe0 = swaddr_read(temp_addr + 7, 1, SEG_TYPE_DS);
	uint32_t temp_base = (temp_describe0 << 24) + (temp_describe3 << 16) + (temp_describe4 << 8) + temp_describe5;
	uint32_t temp_limit = ((temp_describe1 & 0xf) << 16) + (temp_describe6 << 8) +temp_describe7;
#endif

	if (temp_describe1 >> 15)
	{
		temp_limit = (temp_limit << 12) | 0xfff;
	}
		
	assert(temp_describe2 >> 15 == 0);

	
	if (rmcode == 2)
	{
		cpu.SS.val = op_dest->val;
		cpu.SS.cache.base = temp_base;
		cpu.SS.cache.limit = temp_limit;
	}
	else if (rmcode == 3)
	{
		cpu.DS.val = op_dest->val;
		cpu.DS.cache.base = temp_base;
		cpu.DS.cache.limit = temp_limit;
	}
	else if (rmcode == 0)
	{
		cpu.ES.val = op_dest->val;
		cpu.ES.cache.base = temp_base;
		cpu.ES.cache.limit = temp_limit;
	}
	else
		assert(0);
	
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
