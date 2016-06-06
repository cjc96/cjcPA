#include "cpu/exec/helper.h"

/* for instruction encoding overloading */

make_helper(ret_i_b)
{
	print_asm("ret");
	
	return 1;
}

