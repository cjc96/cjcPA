#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movsp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movsp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movsp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(movsp_rm2r)
make_helper_v(movsp_r2rm)

