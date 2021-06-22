#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H
#define NO_OF_INSTRUCTIONS 77

struct Opcode_list{
	char *opcode;
	char *operand;
};

struct Instruction{
	char *name;
	int no_of_operands;
	int bytes;
	int no_of_variants;
	struct Opcode_list *list;
};

struct Instruction *check_if_mnemonic(char *);
int process_instructions(struct Instruction *, char *);
#endif
