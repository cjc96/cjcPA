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
		printf("EAX = %X\t",cpu.eax);
		printf("EDX = %X\n",cpu.edx);
		printf("ECX = %X\t",cpu.ecx);
		printf("EBX = %X\n",cpu.ebx);
		printf("ESI = %X\t",cpu.esi);
		printf("EDI = %X\n",cpu.edi);
		printf("ESP = %X\t",cpu.esp);
		printf("EBP = %X\n",cpu.ebp);
		printf("AX = %X\t",cpu.ax);
		printf("DX = %X\n",cpu.dx);
		printf("CX = %X\t",cpu.cx);
		printf("BX = %X\n",cpu.bx);
		printf("BP = %X\t",cpu.bp);
		printf("SI = %X\n",cpu.si);
		printf("DI = %X\t",cpu.di);
		printf("SP = %X\n",cpu.sp);
		printf("AL = %X\t",cpu.al);
		printf("DL = %X\n",cpu.dl);
		printf("CL = %X\t",cpu.cl);
		printf("BL = %X\n",cpu.bl);
		printf("AH = %X\t",cpu.ah);
		printf("DH = %X\n",cpu.dh);
		printf("CH = %X\t",cpu.ch);
		printf("BH = %X\n",cpu.bh);
		printf("EIP = %X\n",cpu.eip);
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
		printf("%d\n",temp);
	else
		printf("Invalid input\n");
	
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
	{ "p", "Compute the value of an expression", cmd_p}

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
