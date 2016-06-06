#include "cpu/exec/helper.h"
#include "cpu/exec/interrupt.h"

/* for instruction encoding overloading */

make_helper(int_i_b)
{
	print_asm_template1();
	raise_intr(swaddr_read(cpu.eip + 1, 1, SEG_TYPE_CS));
	
	return 2;
}

