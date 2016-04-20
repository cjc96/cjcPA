#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#ifndef SEGMENT
#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#endif
#ifdef SEGMENT
#define MEM_R(addr) swaddr_read(addr, DATA_BYTE, SEG_TYPE_DS)
#endif
#ifndef SEGMENT
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)
#endif
#ifdef SEGMENT
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data, SEG_TYPE_DS)
#endif

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))

#define set_eflags(ain , bin , sin , cin) \
{\
	int temp[32]={0},i;\
	cpu.CF = cin;\
	for (i = 0; i < sizeof(DATA_TYPE)*8; i++)\
	{\
		temp[i] = (ain & 1) ^ ((bin & 1) ^ sin) ^ cin;\
		cin = (((ain & 1) & ((bin & 1) ^ sin)) | ((ain & 1) & cin)) | (((bin & 1) ^ sin) & cin);\
		ain = ain >> 1;\
		bin = bin >> 1;\
		if (i == 8*sizeof(DATA_TYPE) - 2)\
			cpu.OF = cin;\
	}\
	cpu.OF ^= cin;\
	cpu.CF ^= cin;\
	cpu.SF = temp[i-1];\
	cpu.ZF = 0;\
	for (i = 0; i < sizeof(DATA_TYPE) * 8; i++)\
		cpu.ZF = cpu.ZF | temp[i];\
	cpu.ZF = 1 - cpu.ZF;\
	cpu.PF = temp[0];\
	for (i = 1; i < 8; i++)\
		cpu.PF ^= temp[i];\
	cpu.PF = 1 - cpu.PF;\
}
