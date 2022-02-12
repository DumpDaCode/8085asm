#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "link_node.h"

struct def{
	char *line;
	struct link_node link;
};

struct macro{
	char name[6];
	int no_of_para;
	struct link_node def_head;
	struct link_node link;
};

extern struct link_node macro_head;

int check_if_directive(char *string);
int process_directives(char *, char *, int);
struct macro *check_if_macro(char *);
int add_macro(char *, char *, int);
int expand_macro(char *, char *, int);
void free_macro();
#endif
