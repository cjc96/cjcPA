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
		printf("EAX = 0x%08x\t",cpu.eax);
		printf("EDX = 0x%08x\n",cpu.edx);
		printf("ECX = 0x%08x\t",cpu.ecx);
		printf("EBX = 0x%08x\n",cpu.ebx);
		printf("ESI = 0x%08x\t",cpu.esi);
		printf("EDI = 0x%08x\n",cpu.edi);
		printf("ESP = 0x%08x\t",cpu.esp);
		printf("EBP = 0x%08x\n",cpu.ebp);
		printf("AX = 0x%04x\t",cpu.ax);
		printf("DX = 0x%04x\n",cpu.dx);
		printf("CX = 0x%04x\t",cpu.cx);
		printf("BX = 0x%04x\n",cpu.bx);
		printf("BP = 0x%04x\t",cpu.bp);
		printf("SI = 0x%04x\n",cpu.si);
		printf("DI = 0x%04x\t",cpu.di);
		printf("SP = 0x%04x\n",cpu.sp);
		printf("AL = 0x%02x\t",cpu.al);
		printf("DL = 0x%02x\n",cpu.dl);
		printf("CL = 0x%02x\t",cpu.cl);
		printf("BL = 0x%02x\n",cpu.bl);
		printf("AH = 0x%02x\t",cpu.ah);
		printf("DH = 0x%02x\n",cpu.dh);
		printf("CH = 0x%02x\t",cpu.ch);
		printf("BH = 0x%02x\n",cpu.bh);
		printf("EIP = 0x%08x\t",cpu.eip);
		printf("EFLAGS = 0x%08x\n",cpu.EFLAGS);
		printf("CF = %d\t",cpu.CF);
		printf("OF = %d\t",cpu.OF);
		printf("SF = %d\t",cpu.SF);
		printf("PF = %d\t",cpu.PF);
		printf("ZF = %d\t\n",cpu.ZF);
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
	{ "d", "Delete a watchpoint", cmd_d}

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
