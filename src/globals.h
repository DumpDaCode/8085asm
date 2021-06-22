#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>

extern FILE *pl_file, *st_file, *input_file, *asm_file;
extern int line_no, no_of_fields, no_of_operands, program_size;
extern char file_end;
extern struct link_node macro_head;

#endif
