#ifndef __OPERAND_H__
#define __OPERAND_H__

enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };
enum { OP_TYPE_DS, OP_TYPE_SS, OP_TYPE_ES, OP_TYPE_CS };

#define OP_STR_SIZE 40

typedef struct {
	uint32_t type;
	size_t size;
	union {
		uint32_t reg;
		hwaddr_t addr;
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
