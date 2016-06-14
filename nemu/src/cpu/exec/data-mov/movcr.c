#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movcr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movcr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movcr-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(movcr_rm2r)
make_helper_v(movcr_r2rm)

