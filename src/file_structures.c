/*
	All the file structures that aid to the working of assmebler.
*/
#include "file_structures.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct link_node mc_head = INIT_HEAD(mc_head);
struct link_node st_head = INIT_HEAD(st_head);
static struct link_node* temp = NULL;

void mc_add(char *address, char *opcode, char *operand){
/*
	Adds a machine code tuple to the machine code struct.
*/
	struct mc* node = (struct mc*)calloc(1,sizeof(struct mc));
	strcpy(node->address, address);
	strcpy(node->data, opcode);
	if(operand != NULL){
		node->symbol = (char*)calloc(strlen(operand), sizeof(char));
		strcpy(node->symbol, operand);
	}
	else
		node->symbol = NULL;
	init_node(&(node->link));
	add_at_tail(&mc_head, &node->link);
}

struct st *check_sym(char* name){
/*
	Checks for the presence of symbol with same name and returns one.
*/
    struct st *node = NULL;
    temp = &st_head;
	for(temp = temp->next; temp != &st_head; temp = temp->next){
		node = ADDRESS(st, temp, link);
		if(!strcmp(name, node->name)){
            return node;
        }
	}
    return NULL;
}

void st_add(char *name, char type, char* value, char *defined_at, int bytes){
/*
	Adds a symbol and all its corresponding data to symbol struct. It also
	updates data of previously added struct incase  of variables and constants.
*/
    struct st *node = check_sym(name);
	if(node == NULL || strcmp(name, node->name)){
        node = (struct st*)calloc(1, sizeof(struct st));
        strcpy(node->name, name);
        init_node(&(node->link));
        add_at_tail(&st_head, &node->link);
    }
    node->type = type;
	node->value = (char *)calloc(strlen(value) + 1, sizeof(char));
    strcpy(node->value, value);
    strcpy(node->defined_at, defined_at);
    node->bytes = bytes;
}
