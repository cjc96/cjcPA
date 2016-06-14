#ifndef __OPERAND_H__
#define __OPERAND_H__

enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };
enum { SEG_TYPE_DS, SEG_TYPE_SS, SEG_TYPE_ES, SEG_TYPE_CS , SEG_TYPE_NONE};

#define OP_STR_SIZE 40

typedef struct {
	uint32_t type;
	size_t size;
	union {
		uint32_t reg;
		swaddr_t addr;
		uint32_t imm;
		int32_t simm;
	};
	uint32_t val;
	uint32_t seg_type;
	char str[OP_STR_SIZE];
} Operand;

typedef struct {
	uint32_t opcode;
	bool is_data_size_16;
	Operand src, dest, src2;
} Operands;

#endif
