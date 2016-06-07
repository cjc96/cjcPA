#include "common.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include <setjmp.h>

enum { SEG_TYPE_DS, SEG_TYPE_SS, SEG_TYPE_ES, SEG_TYPE_CS , SEG_TYPE_NONE};

extern jmp_buf jbuf;

void raise_intr(uint8_t NO) {
    assert(NO * 8 <= cpu.idtr.limit);
    lnaddr_t pidt = cpu.idtr.base + NO * 8;
    uint64_t idt_des = ((uint64_t) lnaddr_read(pidt + 4, 4) << 32) | lnaddr_read(pidt, 4); 
    assert((idt_des >> 47) & 1);
    uint8_t gate_type = (idt_des >> 40) & 0x7;
    //uint8_t gate_size = (idt_des >> 43) & 1;
    assert(gate_type != 5);
    //if (NO != 2 && !cpu.ief) return; // not NMI, IF is not present
    
    // push registers into stack
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.EFLAGS, SEG_TYPE_SS);
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.CS.val, SEG_TYPE_SS);
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eip, SEG_TYPE_SS); // next instruction now
    
     // no error code currently
    if (gate_type == 6) { // interrupt gate
        cpu.IF = 0;
        printf("set1");
    }
    
    // long jump
    cpu.CS.val = (idt_des >> 16) & 0xFFFF;
    
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
    
    cpu.eip = (idt_des & 0xFFFF) | ((idt_des >> 32LL) & 0xFFFF0000);
    
    /* Jump back to cpu_exec() */
    longjmp(jbuf, 1);
}
