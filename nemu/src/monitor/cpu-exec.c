#include "monitor/monitor.h"
#include "cpu/helper.h"
#include <setjmp.h>
#include "monitor/expr.h"
#include "monitor/watchpoint.h"

/* PA4: interrupt */
#include "device/i8259.h"
#include "cpu/exec/interrupt.h"

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the ``si'' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10

int nemu_state = STOP;

int exec(swaddr_t);

char assembly[80];
char asm_buf[128];
int do_call,do_jmpnear,do_rm_call;

/* Used with exception handling. */
jmp_buf jbuf;

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		 
		do_call = 0; do_jmpnear = 0; do_rm_call = 0;
		int former_eip = cpu.eip;		
		int instr_len = exec(cpu.eip);
		
		cpu.eip += instr_len;
		former_eip += instr_len;
		if (do_call)
		{
			cpu.esp -= 4;
#ifndef SEGMENT
			swaddr_write(cpu.esp,4,former_eip);
#endif
#ifdef SEGMENT
			swaddr_write(cpu.esp,4,former_eip,SEG_TYPE_SS);
#endif
			
			if (do_rm_call)
				cpu.eip -= instr_len;
				
			//printf("%x\n",former_eip);
		}
		if (do_jmpnear)
		{
			cpu.eip -= instr_len;
		}

#ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif

		/* check watchpoints here. */
		WP *temp_node;
		int temp_value;
		bool temp_success;
		
		temp_value = 0;
		temp_success = true;
		
		temp_node = get_head();
		while (temp_node != NULL)
		{
			temp_value = expr(temp_node->expr,&temp_success);
			if (temp_value != temp_node->value)
			{
				nemu_state = STOP;
				temp_node->value = temp_value;
				printf("break at watchpoint %d\n",temp_node->NO);
				break;
			}
			temp_node = temp_node->next;
		}
		
		if(nemu_state != RUNNING) { return; }

	
		/* PA4:check if an interrupt is called whenever an instruction was excecuted */
		if(cpu.INTR & cpu.IF) 
		{
			uint32_t intr_no = i8259_query_intr();
			i8259_ack_intr();
			raise_intr(intr_no);
		}
		
		
		// Debug for pal
		if (instr_fetch(cpu.eip, 1) == 0)
		{
			printf("%x\n", cpu.eip);
			nemu_state = STOP;
			return;
		}
		
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}
