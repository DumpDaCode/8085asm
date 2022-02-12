#ifndef FILE_STRUCTURES_H
#define FILE_STRUCTURES_H

#include "link_node.h"

struct mc{
	char address[5];
	char data[3];
	char *symbol;
	struct link_node link;
};

struct st{
	char name[7];
	char type;
	char *value;
	char defined_at[5];
	int bytes;
	struct link_node link;
};

extern struct link_node mc_head;
extern struct link_node st_head;

struct st *check_sym(char *);
void mc_add(char *, char *, char *);
void st_add(char *, char, char *, char *, int);
void free_st();

#endif
