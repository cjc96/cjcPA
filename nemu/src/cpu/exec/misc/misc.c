#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include "device/port-io.h"

make_helper(stc)
{
	cpu.CF = 1;
	print_asm("stc");
	
	return 1;
}

make_helper(clc)
{
	cpu.CF = 0;
	print_asm("clc");
	
	return 1;
}

make_helper(cli)
{
	cpu.IF = 0;
	print_asm("cli");
	
	return 1;
}

make_helper(sti)
{
	cpu.IF = 1;
	print_asm("sti");
	
	return 1;
}

make_helper(hlt)
{
	//opcode = f4
	while (!cpu.INTR);
	print_asm("hlt");
	
	return 1;
}

make_helper(iret)
{
    assert(cpu.cr0.protect_enable == 1);
	
    cpu.eip = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
    cpu.esp += 4;
    cpu.CS.val = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
    cpu.esp += 4;
    cpu.EFLAGS = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
    cpu.esp += 4;
    
    uint32_t temp_addr = 8 * ((cpu.CS.val >> 3) & 0x1fff) + cpu.gdtr.base;
	uint32_t temp_describe7 = swaddr_read(temp_addr, 1, SEG_TYPE_DS), temp_describe6 = swaddr_read(temp_addr + 1, 1, SEG_TYPE_DS), temp_describe5 = swaddr_read(temp_addr + 2, 1, SEG_TYPE_DS), temp_describe4 = swaddr_read(temp_addr + 3, 1, SEG_TYPE_DS), temp_describe3 = swaddr_read(temp_addr + 4, 1, SEG_TYPE_DS), temp_describe2 = swaddr_read(temp_addr + 5, 1, SEG_TYPE_DS), temp_describe1 = swaddr_read(temp_addr + 6, 1, SEG_TYPE_DS), temp_describe0 = swaddr_read(temp_addr + 7, 1, SEG_TYPE_DS);
	uint32_t temp_base = (temp_describe0 << 24) + (temp_describe3 << 16) + (temp_describe4 << 8) + temp_describe5;
	uint32_t temp_limit = ((temp_describe1 & 0xf) << 16) + (temp_describe6 << 8) +temp_describe7;
	if (temp_describe1 >> 15)
	{
		temp_limit = (temp_limit << 12) | 0xfff;
	}
		
	assert(temp_describe2 >> 15 == 0);
    
    cpu.CS.cache.base = temp_base;
    cpu.CS.cache.limit = temp_limit;
    
    print_asm("iret");
    
    return 0;
}

make_helper(popa)
{
	cpu.edi = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.esi = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.ebp = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.esp += 4;
	
	cpu.ebx = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.edx = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.ecx = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	
	cpu.eax = swaddr_read(cpu.esp, 4, SEG_TYPE_SS);
	cpu.esp += 4;
	print_asm("popa");
	
	return 1;
}

make_helper(pusha)
{
	uint32_t temp = cpu.esp;
	
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eax, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ecx, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edx, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebx, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, temp, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.ebp, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.esi, SEG_TYPE_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.edi, SEG_TYPE_SS);

	print_asm("pusha");
	
	return 1;
}

make_helper(std)
{
	cpu.DF = 1;
	print_asm("std");
	
	return 1;
}

make_helper(cwtl) 
{
	if (!ops_decoded.is_data_size_16)
	{
		int16_t temp1 = cpu.ax;
		int32_t temp2 = temp1;
		cpu.eax = temp2;	
		print_asm("cwtl");
	}
	else
	{
		int8_t temp1 = cpu.al;
		int16_t temp2 = temp1;
		cpu.ax = temp2;
		print_asm("cbtw");
	}
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
	if (!ops_decoded.is_data_size_16)
	{
		if (cpu.eax>>31)
			cpu.edx = 0xFFFFFFFF;
		else
			cpu.edx = 0;
		print_asm("cltd");
	}
	else
	{
		if (cpu.ax >> 15)
			cpu.dx = 0xFFFF;
		else
			cpu.dx = 0;
		print_asm("cwtd");
	}
	return 1;
}

make_helper(cld) 
{
	cpu.DF = 0;
	print_asm("cld");
	
	return 1;
}

