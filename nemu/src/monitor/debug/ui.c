#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "../../../include/memory/memory.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
	int n;
	if (args != NULL) {
		sscanf(args, "%d", &n);

	}
	else
		n = 1;
	cpu_exec(n);
	return 0;
}

static int cmd_info(char *args)
{
	if (strcmp(args,"r") == 0)
	{
		printf("EAX = 0x%08X\t",cpu.eax);
		printf("EDX = 0x%08X\n",cpu.edx);
		printf("ECX = 0x%08X\t",cpu.ecx);
		printf("EBX = 0x%08X\n",cpu.ebx);
		printf("ESI = 0x%08X\t",cpu.esi);
		printf("EDI = 0x%08X\n",cpu.edi);
		printf("ESP = 0x%08X\t",cpu.esp);
		printf("EBP = 0x%08X\n",cpu.ebp);
		/*printf("AX = 0x%04X\t\t",cpu.ax);
		printf("DX = 0x%04X\n",cpu.dx);
		printf("CX = 0x%04X\t\t",cpu.cx);
		printf("BX = 0x%04X\n",cpu.bx);
		printf("BP = 0x%04X\t\t",cpu.bp);
		printf("SI = 0x%04X\n",cpu.si);
		printf("DI = 0x%04X\t\t",cpu.di);
		printf("SP = 0x%04X\n",cpu.sp);
		printf("AL = 0x%02X\t\t",cpu.al);
		printf("DL = 0x%02X\n",cpu.dl);
		printf("CL = 0x%02X\t\t",cpu.cl);
		printf("BL = 0x%02X\n",cpu.bl);
		printf("AH = 0x%02X\t\t",cpu.ah);
		printf("DH = 0x%02X\n",cpu.dh);
		printf("CH = 0x%02X\t\t",cpu.ch);
		printf("BH = 0x%02X\n",cpu.bh);*/
		printf("EIP = 0x%08X\t",cpu.eip);
		printf("EFLAGS = 0x%08X\n",cpu.EFLAGS);
		printf("CF = %d\t",cpu.CF);
		printf("OF = %d\t",cpu.OF);
		printf("SF = %d\t",cpu.SF);
		printf("PF = %d\t",cpu.PF);
		printf("ZF = %d\t",cpu.ZF);
		printf("DF = %d\t\n",cpu.DF);
	}
	if (strcmp(args,"w") == 0)
	{
		WP* temp;
		
		temp = get_head();
		if (temp == NULL)
		{
			printf("no watchpoint!\n");
		}
		while (temp != NULL)
		{
			printf("No. %d\t%s\t%d\n",temp->NO,temp->expr,temp->value);
			temp = temp->next;
		}
	}
	return 0;
}

static int cmd_x(char *args)
{
	unsigned int addr;
	int n,i;
	char *temp;
	
	temp = strtok(args , " ");
	sscanf(temp,"%d",&n);
	sscanf(temp + strlen(temp)+1 , "%x" , &addr);
	
	for (i = 0; i < n; i++)
		printf("%x\t :\t %d\n",addr+i,swaddr_read(addr+i,4));

	return 0;
}

static int cmd_p(char *args)
{
	bool success;
	unsigned int temp;
	
	success = true;
	temp = 0;
	temp = expr(args,&success);
	if (success)
		printf("DEC : %u\t\tHEX : %x\n",temp,temp);
	else
		printf("Invalid input\n");
	
	return 0;
}

static int cmd_w(char *args)
{
	WP *temp;
	static int no;
	bool success;
	int check;
	
	success = true;
	check = 0;
	check = expr(args,&success);
	if (success)
	{
		temp = new_wp();
		temp->NO = no++;
		strcpy(temp->expr,args);
		temp->value = check;
	}
	else
		printf("Invalid input\n");

	return 0;
}

static int cmd_d(char *args)
{
	int no;
	WP *temp;

	sscanf(args,"%d",&no);	
	temp = get_head();
	while (temp->NO != no && temp != NULL)
		temp = temp->next;
	if (temp == NULL)
	{
		printf("Invalid input\n");
		return 0;
	}
	free_wp(temp);

	return 0;
}

static int cmd_clear()
{
		
	printf("%s", "\033[1H\033[2J");
	
	return 0;
}

static int cmd_bt()
{
	uint32_t temp = cpu.ebp,label = 1, flag = 1;
	int nend_flag;
	
	extern int get_now_func_name(uint32_t,uint32_t);
	printf("#0 0x%x in ",cpu.eip);
	nend_flag = get_now_func_name(cpu.eip,cpu.ebp);
	
	if (nend_flag == -1)
			panic("Something wrong with command \"bt\"\n");

	if (nend_flag)
		while (temp != 0 && flag)
		{
			printf("#%d 0x%x in ",label,swaddr_read(temp+4,4));
		
			extern int get_func_name(uint32_t);
			if (get_func_name(temp))
				flag = 0;
		
			temp = swaddr_read(temp,4);
			label++;
		}
		
	return 0;
}

#ifdef CACHE
static int cmd_addr(char *args)
{
	uint32_t addr, len;
	sscanf(args, "%x %d", &addr, &len);
	
	uint32_t musk = ~0u >> ((4 - len) << 3);
	uint32_t tag = addr & 0xfffffe00, offset = addr & 0x0000003f, tag_sp = (addr + len - 1) & 0xfffffe00, start_sp = (addr + len - 1) & 0x000001c0 << 1;
	bool flag = 0;
	
	uint32_t i, start =addr & 0x000001c0 << 1, end = start + 128;
	for (i = start; i < end; i++)
	{
		if (l1_cache[i].sign && l1_cache[i].tag == tag)
		{
			if (start_sp != start)
			{
				uint8_t ans[4], *temp = ans, loc = 0;
				while (((addr + loc) & 0x000001c0 << 1) == start)
				{
					ans[loc] = l1_cache[i].data_b[offset + loc];
					loc++;
				}
				
				uint32_t j, end_sp = start_sp + 128, loc_sp = 0;
				for (j = start_sp; j < end_sp; j++)
				{
					if (l1_cache[j].sign && l1_cache[j].tag == tag_sp)
					{
						assert(loc < len);
						while (loc < len)
						{
							ans[loc] = l1_cache[j].data_b[loc_sp];
							loc++;
							loc_sp++;
						}
						
						printf("Hit in Two Blocks!!!!!!!!!!\n");
						printf("Ans = 0x%X\n",*(uint32_t *)temp & musk);
						printf("Group_id = %X\n", addr & 0x000001c0 << 1);
						printf("Offset = %x\n", offset);
						printf("Tag = 0x%X\tTag_sp = %X\n\n", tag, tag_sp);
						uint32_t k;
						printf("Block 1 : l1_cache[%d] = ", i);
						for (k = 0; k < 64; k++)
						{
							if (k % 16 == 0)
								printf("\n");
							printf("%X ", l1_cache[i].data_b[k]);
						}
						printf("Block 2 : l1_cache[%d] = ", j);
						for (k = 0; k < 64; k++)
						{
							if (k % 16 == 0)
								printf("\n");
							printf("%X ", l1_cache[j].data_b[k]);
						}
						flag = true;
					}
				}
				break;
			}
			else
			{
				uint8_t *temp = &l1_cache[i].data_b[offset];
				printf("Ans = 0x%X\n",*(uint32_t *)temp & musk);
				printf("Group_id = %X\n", addr & 0x000001c0 << 1);
				printf("Offset = %x\n", offset);
				printf("Tag = 0x%X\n\n", tag);
				uint32_t k;
				printf("Block : l1_cache[%d] = ", i);
				for (k = 0; k < 64; k++)
				{
					if (k % 16 == 0)
						printf("\n");
					printf("%X ", l1_cache[i].data_b[k]);
				}
				flag = true;
			}
		}
	}
	
	if (!flag)
		printf("Woops, not hit~~~");
		
	return 0;
}
#endif

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute n commands", cmd_si },
	{ "info", "Show the information", cmd_info},
	{ "x", "Scan the Ram", cmd_x},
	{ "p", "Compute the value of an expression", cmd_p},
	{ "w", "Set a watchpoint", cmd_w},
	{ "d", "Delete a watchpoint", cmd_d},
	{ "bt", "Print all stack frames", cmd_bt},
#ifdef CACHE
	{ "addr", "Check address in cache", cmd_addr},
#endif
	{ "clear", "Clear the screen", cmd_clear}

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
