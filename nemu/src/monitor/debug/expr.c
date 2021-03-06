#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NEQ, NLT, NMT, MT, LT, LAND, LOR, LN, XOR, AAND, AOR, AN, NUM, REG, NEG, SHL, SHR, HEX, POINTER, OBJ
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	
	{"0x[0-9a-fA-F]+",HEX},			// hex
	{" +", NOTYPE},					// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},	 				// minus
	{"\\*", '*'},					// times
	{"\\/", '/'},					// divided
	{"\\%", '%'},					// modify
	{"\\(", '('},					// right parenthese
	{"\\)", ')'},					// left parenthese
	{"==", EQ},						// equal
	{">>", SHR},					// shr
	{"<<", SHL},					// shl
	{"!=", NEQ},					// not equal
	{">=", NLT},					// no less than
	{"<=", NMT},					// no more than
	{">", MT},						// more than
	{"<", LT},						// less than
	{"&&", LAND},					// logical and
	{"\\|\\|", LOR},				// logical or
	{"!", LN},						// logical not
	{"\\^", XOR},					// xor
	{"\\&", AAND},					// algebra and
	{"\\|", AOR},					// algebra or
	{"~", AN},						// algebra not
	{"[0-9]+", NUM},				// number
	{"\\$[a-zA-Z]+", REG},				// register
	{"[a-zA-Z_][a-zA-Z0-9_]*",OBJ}  // object

	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	unsigned int num;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	char regtemp[32];
	unsigned int i,temp;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s\n", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
				    case OBJ:
				        tokens[nr_token].type = rules[i].token_type;
				        for (temp = 0; temp < substr_len; ++temp)
				            tokens[nr_token].str[temp] = substr_start[temp];
				        tokens[nr_token].str[temp] = '\0';
				        
				        extern uint32_t get_address_from_name(char *);
				        tokens[nr_token].num = get_address_from_name(tokens[nr_token].str);
				        assert(tokens[nr_token].num);
				        
				        nr_token++;
				        break;
				
					case NOTYPE: 
						break;
						
					case EQ:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;

					case SHL:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;

					case SHR:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;

					case NEQ:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
					
					case NLT:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
					
					case NMT:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case MT:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case LT:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case LAND:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case LOR:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case LN:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case XOR:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case AAND:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case AOR:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case AN:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case NUM:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token].str[temp] = '\0';
						
						tokens[nr_token].num = 0;
						for (temp = 0; temp < substr_len; ++temp)
						{
							tokens[nr_token].num = tokens[nr_token].num * 10 + substr_start[temp] - '0';
						}
						nr_token++;
						break;
						
					case REG:
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token].str[temp] = '\0';
						
						for (temp = 1; temp < substr_len; ++temp)
							regtemp[temp-1] = substr_start[temp];
						if (strcmp(regtemp,"eax") == 0)
							tokens[nr_token].num = cpu.eax;
						if (strcmp(regtemp,"edx") == 0)
							tokens[nr_token].num = cpu.edx;
						if (strcmp(regtemp,"ecx") == 0)
							tokens[nr_token].num = cpu.ecx;
						if (strcmp(regtemp,"ebx") == 0)
							tokens[nr_token].num = cpu.ebx;
						if (strcmp(regtemp,"esi") == 0)
							tokens[nr_token].num = cpu.esi;
						if (strcmp(regtemp,"edi") == 0)
							tokens[nr_token].num = cpu.edi;
						if (strcmp(regtemp,"esp") == 0)
							tokens[nr_token].num = cpu.esp;
						if (strcmp(regtemp,"ebp") == 0)
							tokens[nr_token].num = cpu.ebp;
						if (strcmp(regtemp,"ax") == 0)
							tokens[nr_token].num = cpu.ax;
						if (strcmp(regtemp,"dx") == 0)
							tokens[nr_token].num = cpu.dx;
						if (strcmp(regtemp,"cx") == 0)
							tokens[nr_token].num = cpu.cx;
						if (strcmp(regtemp,"bx") == 0)
							tokens[nr_token].num = cpu.bx;
						if (strcmp(regtemp,"bp") == 0)
							tokens[nr_token].num = cpu.bp;	
						if (strcmp(regtemp,"si") == 0)
							tokens[nr_token].num = cpu.si;
						if (strcmp(regtemp,"di") == 0)
							tokens[nr_token].num = cpu.di;
						if (strcmp(regtemp,"sp") == 0)
							tokens[nr_token].num = cpu.sp;
						if (strcmp(regtemp,"al") == 0)
							tokens[nr_token].num = cpu.al;
						if (strcmp(regtemp,"dl") == 0)
							tokens[nr_token].num = cpu.dl;
						if (strcmp(regtemp,"cl") == 0)
							tokens[nr_token].num = cpu.cl;
						if (strcmp(regtemp,"bl") == 0)
							tokens[nr_token].num = cpu.bl;
						if (strcmp(regtemp,"ah") == 0)
							tokens[nr_token].num = cpu.ah;
						if (strcmp(regtemp,"dh") == 0)
							tokens[nr_token].num = cpu.dh;
						if (strcmp(regtemp,"ch") == 0)
							tokens[nr_token].num = cpu.ch;
						if (strcmp(regtemp,"bh") == 0)
							tokens[nr_token].num = cpu.bh;
						if (strcmp(regtemp,"eip") == 0)
							tokens[nr_token].num = cpu.eip;
						if (strcmp(regtemp,"CS") == 0)
							tokens[nr_token].num = cpu.CS.val;
						if (strcmp(regtemp,"DS") == 0)
							tokens[nr_token].num = cpu.DS.val;
						if (strcmp(regtemp,"ES") == 0)
							tokens[nr_token].num = cpu.ES.val;
						if (strcmp(regtemp,"SS") == 0)
							tokens[nr_token].num = cpu.SS.val;
						nr_token++;
						break;
						
					case '+': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case '-': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case '*': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case '/': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case '%': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case '(': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case ')': 
						tokens[nr_token].type = rules[i].token_type; 
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token++].str[temp] = '\0';
						break;
						
					case HEX:
						tokens[nr_token].type = NUM;
						for (temp = 0; temp < substr_len; ++temp)
							tokens[nr_token].str[temp] = substr_start[temp];
						tokens[nr_token].str[temp] = '\0';
						sscanf(tokens[nr_token].str,"%x",&tokens[nr_token].num);
						nr_token++;
						break;
						
					default: panic("please implement me");
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

uint32_t expr(char *e, bool *success) {
	int i;											// loop varible
	unsigned int sta[32] = {0};						// stack
	int  sta_len = 0;								// stack length
	int pro[32] = {0}, pro_len = 0;					// order of process
	int priority[300]={0};							// priority
	unsigned int temp1,temp2;						// execute numbers
	
	// priority table
	priority[EQ] = 6; priority[NEQ] = 6; priority[NLT] = 7;
	priority[NMT] = 7; priority[MT] = 7; priority[LT] = 7;
	priority[LAND] = 2; priority[LOR] = 1; priority[LN] = 11;
	priority[XOR] = 4; priority[AAND] = 5; priority[AOR] = 3;
	priority[AN] = 11; priority[NUM] = 13; priority[REG] = 13;
	priority[NEG] = 11; priority['+'] = 9; priority['-'] = 9;
	priority['*'] = 10; priority['/'] = 10; priority['%'] = 10;
	priority[SHL] = 8; priority[SHR] = 8; priority[HEX] = 13;
	priority[POINTER] = 11; priority[OBJ] = 13;

	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	// correct '-' from minus into negative 
	for (i = 0; i < nr_token; ++i)
	{
		if (tokens[i].type == '-' && tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != ')')
			tokens[i].type = NEG;
		if (tokens[i].type == '*' && tokens[i-1].type != NUM && tokens[i-1].type != REG && tokens[i-1].type != ')')
			tokens[i].type = POINTER;
	}
	
	// get infix expression into suffix expression
	for (i = 0; i < nr_token; ++i)
	{
		if (tokens[i].type == REG || tokens[i].type == NUM)
			pro[pro_len++] = i;
		else if (tokens[i].type == '(')
			sta[sta_len++] = i;
		else if (tokens[i].type == ')')
		{
			while (tokens[sta[sta_len-1]].type != '(')
			{
				pro[pro_len++] = sta[sta_len-1];
				sta_len--;
			}
			sta_len--;
		}
		else
		{
			while (sta_len > 0 && priority[tokens[sta[sta_len-1]].type] >= priority[tokens[i].type])
			{
				pro[pro_len++] = sta[sta_len-1];
				sta_len--;
			}
			sta[sta_len++] = i;
		}
	}
	while (sta_len > 0)
		pro[pro_len++] = sta[--sta_len];
	
	
	/*for (i = 0; i < pro_len; i++)
	{
		printf("%s %u\n",tokens[pro[i]].str,tokens[pro[i]].num);
	}
	printf("\n");*/
	
	// calculate the value of the expression with the help of array "pro"
	sta_len = 0;
	memset(sta,0,sizeof(sta));
	for (i = 0; i < pro_len; i++)
	{
		switch (tokens[pro[i]].type)
		{
			case EQ:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 == temp2);
				sta_len--;
				break;
				
			case SHL:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 << temp2);
				sta_len--;
				break;

			case SHR:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 >> temp2);
				sta_len--;
				break;
				
			case NEQ:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 != temp2);
				sta_len--;
				break;

			case NLT:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 >= temp2);
				sta_len--;
				break;
					
			case NMT:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 <= temp2);
				sta_len--;
				break;
					
			case MT:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 > temp2);
				sta_len--;
				break;
						
			case LT:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 < temp2);
				sta_len--;
				break;
				
			case LAND:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 && temp2);
				sta_len--;
				break;
						
			case LOR:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 || temp2);
				sta_len--;
				break;
						
			case LN:
				temp2 = sta[sta_len-1];
				sta[sta_len-1] = (!temp2);
				break;
						
			case XOR:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 ^ temp2);
				sta_len--;
				break;
						
			case AAND:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 & temp2);
				sta_len--;
				break;
				
			case AOR:
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 | temp2);
				sta_len--;
				break;
						
			case AN:
				temp2 = sta[sta_len-1];
				sta[sta_len-1] = (~temp2);
				break;
						
			case NUM:
				sta[sta_len++] = tokens[pro[i]].num;
				break;
						
			case REG:
				sta[sta_len++] = tokens[pro[i]].num;
				break;
				
			case OBJ:
			    sta[sta_len++] = tokens[pro[i]].num;
			    break;
				
			case POINTER:
				temp2 = sta[sta_len-1];
#ifdef SEGMENT
				sta[sta_len-1] = swaddr_read(temp2, 4, 0);
#endif
#ifndef SEGMENT
				sta[sta_len-1] = swaddr_read(temp2, 4);
#endif
				break;
						
			case '+': 
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 + temp2);
				sta_len--;
				break;
						
			case '-': 
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 - temp2);
				sta_len--;
				break;
						
			case '*': 
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				if (temp2 == 0)
				{
					*success = false;
					return 0;
				}
				sta[sta_len-2] = (temp1 * temp2);
				sta_len--;
				break;
						
			case '/': 
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				sta[sta_len-2] = (temp1 / temp2);
				sta_len--;
				break;
						
			case '%': 
				temp1 = sta[sta_len-2];
				temp2 = sta[sta_len-1];
				if (temp2 == 0)
				{
					*success = false;
					return 0;
				}
				sta[sta_len-2] = (temp1 % temp2);
				sta_len--;
				break;
				
			case NEG:
				temp2 = sta[sta_len-1];
				sta[sta_len-1] = 0-temp2;
				break;
						
			default: panic("please implement me");	
		}
	}
	
	if (sta_len != 1)
	{
		*success = false;
		return 0;
	}
	
	return sta[0];
}
