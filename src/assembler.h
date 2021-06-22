#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

void init(char *);
void process_macro();
void assemble();
char *read_line(FILE *);
void process_tokens(char *);
char *evaluate_operands(char *);
char *vacin(char, char *, char *, int);
char *dth(int, int);
void urad();

#endif
