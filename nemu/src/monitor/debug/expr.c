#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NLT, NMT, MT, LT, LAND, LOR, LN, XOR, AAND, AOR, AN, NUM, REG

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	 
	{" +", NOTYPE},					// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},	 				// minus
	{"\\*", '*'},					// times
	{"\\/", '/'},					// divided
	{"\\%", '%'},					// modify
	{"\\(", '('},					// right parenthese
	{"\\)", ')'},					// left parenthese
	{"==", EQ},						// equal
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
	{"\\$[a-z]+", REG}				// register
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
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int num;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i,temp;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s\n", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE: 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case EQ:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
					
					case NLT:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
					
					case NMT:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case MT:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case LT:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case LAND:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case LOR:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case LN:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case XOR:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case AAND:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case AOR:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case AN:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case NUM:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						for (temp = 0; temp < substr_len; ++temp)
						{
							tokens[nr_token].num = tokens[nr_token].num * 10 + substr_start[temp] - '0';
						}
						break;
						
					case REG:
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '+': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '-': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '*': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '/': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '%': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case '(': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
						break;
						
					case ')': 
						tokens[nr_token].type = rules[i].token_type; 
						strcpy(tokens[nr_token++].str,substr_start);
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
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	printf("1");
	panic("please implement me");
	return 0;
}

