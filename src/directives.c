/*
	Everything related to directives.
*/
#include "directives.h"
#include "globals.h"
#include "file_structures.h"
#include "assembler.h"
#include <stdlib.h>
#include <string.h>

struct link_node macro_head = INIT_HEAD(macro_head);

int check_if_directive(char *string){
/*
	This checks which directive is being requseted.
*/
	char directives[5][6] = {"ORG", "DB", "DW", "EQU", "END"};
	int i = 0;
	for(i = 0; i < 5; i++){
		if(!strcmp(directives[i], string)){
			return 1;
		}
	}
	return 0;
}

int process_directives(char *symbol, char *line, int semi_colon){
/*
	This process all the direcives except macro.
*/
	if(no_of_fields == 2){
		char *data = line+strlen(line)+1;
		char fdata[strlen(line) - 2], type;
		int ascii = (data[0] == '\'' && data[strlen(data) - 1] == '\''), bytes;
		switch(semi_colon){
			case 1:
				if(!strcmp("DB", line)){
					type = 'V';
					bytes = 2;
				}
				else if(!strcmp("DW", line)){
					type = 'V';
					bytes = 4;
				}
				else{
					fprintf(stderr, "%d: Wrong syntax for directive '%s' or unrecognized directive.\n", line_no, line);
					return 0;
				}
				break;
			case -1:
			case 0:
				if(!strcmp("EQU", line)){
					type = 'C';
					bytes = 2;
				}
				else{
					fprintf(stderr, "%d: Wrong syntax for directive '%s' or unrecognized directive.\n", line_no, line);
					return 0;
				}
				break;
			default:{
				fprintf(stderr, "%d: Unrecognized directive '%s'\n", line_no, line);
				return 0;
			}
		}
		switch(ascii){
			case 0:
				fdata[strlen(line) - no_of_operands] = '\0';
				strtok(data, ",");
				memset(fdata, 0, strlen(fdata));
				char *result;
				while(data != NULL){
					result = evaluate_operands(data);
					if(strlen(result) < bytes){
						if(bytes == 4){
							fprintf(stderr, "%d: 8-bit data specified where 16-bit was expected.\n", line_no);
						}
						if(bytes == 2){
							fprintf(stderr, "%d: 16-bit data specified where 8-bit was expected.\n", line_no);
						}
						return 0;
					}
					strncat(fdata, result, strlen(result));
					data = strtok(NULL, ",");
				}
				free(result);
				result = NULL;
				break;
			case 1:
				memset(fdata, 0, strlen(fdata));
				int i;
				for(i = 1; i<strlen(data)-1; i++){
					result = dth((int)data[i],2);
					strncat(fdata, result, strlen(result));
				}
				free(result);
				result = NULL;
				break;
		}
		st_add(symbol, type, fdata, "-", bytes);
		return 1;
	}
	fprintf(stderr, "%d: No data was specified.\n", line_no);
	return 0;
}

struct macro *check_if_macro(char *name){
/*
	A utility function for checking whehter a similar named macro exists in
	macro struct.
*/
	if(is_empty(&macro_head)){
		return NULL;
	}
	struct macro *node = NULL;
	struct link_node *temp = &macro_head;
	for(temp = temp->next; temp != &macro_head; temp = temp->next){
		node = ADDRESS(macro, temp, link);
		if(!strcmp(name, node->name)){
            return node;
        }
	}
    return NULL;
}

int add_macro(char *name, char *parameters, int count){
/*
	This function is use to add macro to macro struct. It first performs a few
	checks. Then it generates a parameter array. After this each line is read
	and parameters are repalced with their indexes. This process continues
	until ENDM is found. Then the macro node is prepared and added.
*/
	if(strlen(name) >= 7){
		fprintf(stderr, "%d: Illegal length for macro name '%s'.\n", line_no, name);
		return 0;
	}
	if(check_if_macro(name) != NULL){
		return 0;
	}
	int i, j, len, k;
	char *para_array[count], *line = NULL, *ref_line = NULL, data[20];
	for(i = 0; i < count; i++){
		if(i == 0)
			para_array[0] = strtok(parameters, ",");
		else
			para_array[i] = strtok(NULL, ",");
	}
	struct macro *mnode = (struct macro *)calloc(1, sizeof(struct macro));
	init_node(&mnode->def_head);
	char *tempL = NULL;
	while(file_end == 0){
		len = 0;
		line = read_line(input_file);
		line_no++;
		ref_line = line;
		tempL = line;
		for (i = 0; i < no_of_fields; i++){
			len += strlen(ref_line);
			ref_line += strlen(ref_line) + 1;
		}
		len += no_of_fields;
		struct def *node = (struct def *)calloc(1, sizeof(struct def));
		node->line = (char *)calloc(len, sizeof(char));
		if(no_of_fields == 0){
			return 0;
		}
		else{
			for(j = 0; j<no_of_fields; j++){
				len = strlen(line);
				if(!strcmp(line, "ENDM")){
					free(node->line);
					free(node);
					free(tempL);
					// node->line = NULL;
					node = NULL;
					tempL= NULL;
					if(no_of_fields == 1){
						init_node(&(mnode->link));
						strcpy(mnode->name, name);
						mnode->no_of_para = count;
						add_at_tail(&macro_head, &mnode->link);
						return 1;
					}
					else{
						free(mnode);
						mnode = NULL;
						return 0;
					}
				}
				ref_line = line;

				strtok(ref_line, ",");
				do{
					for(i = 0; i<count; i++){
						if(!strcmp(ref_line, para_array[i])){
							sprintf(data, "#%d", i);
							strcat(node->line, data);
							break;
						}
					}
					if(i >= count && strcmp(ref_line, para_array[i-1])){
						strcat(node->line, ref_line);
					}
					ref_line = strtok(NULL, ",");
					if(ref_line != NULL){
						strcat(node->line, ",");
					}
					else{
						if(no_of_fields - j> 1)
							strcat(node->line, " ");
					}
				}while(ref_line!=NULL);
				ref_line = line;
				line += len+1;
			}
		}
		free(tempL);
		tempL = NULL;
		init_node(&(node->link));
		add_at_tail(&(mnode->def_head), &node->link);
	}
	init_node(&(mnode->link));
	strcpy(mnode->name, name);
	mnode->no_of_para = count;
	add_at_tail(&macro_head, &mnode->link);
}

int expand_macro(char *name, char *parameters, int count){
/*
	This function is use to dump all the macro defintions to the intermediate
	file. It checks whether the macro has been previously defined. Creates a
	parameter array, tokenizes line on '#' symbol, repalces dummy parameters
	with actual parameters.
*/
	struct macro *node = check_if_macro(name);
	if(node == NULL){
		fprintf(stderr, "%d: Unrecognized macro '%s'\n", line_no, name);
		return 0;
	}
	int i, j, len, k;
	char *para_array[count], *line = NULL, *ref_line = NULL, data[20], *token;
	for(i = 0; i< count; i++){
		if(i == 0)
			para_array[0] = strtok(parameters, ",");
		else
			para_array[i] = strtok(NULL, ",");
	}
	struct def *dnode = NULL;
	struct link_node *temp = &(node->def_head);
	no_of_operands = 0;
	for(temp = temp->next; temp != &(node->def_head); temp=temp->next){
		dnode = ADDRESS(def, temp, link);
		line = (char *)calloc(strlen(dnode->line)+1, sizeof(char));
		char *tempL = line;
		strcpy(line, dnode->line);
		strtok(line, "#");
		fprintf(asm_file, "%s", line);
		line = strtok(NULL, "#");
		while(line != NULL){
			ref_line = line;
			len = strlen(ref_line);
			for(i = 0; i < strlen(ref_line) && ref_line[i] >= '0' && ref_line[i] <= '9'; i++){
				data[i] = ref_line[i];
			}
			data[i] = '\0';
			fprintf(asm_file, "%s", para_array[atoi(data)]);
			line = strtok(NULL, "#");
			if(i < strlen(ref_line))
				fprintf(asm_file, "%s", ref_line+i);
		}
		fprintf(asm_file, "\n");
		free(tempL);
		tempL = NULL;
	}
}

void free_macro(){
	if(!is_empty(&macro_head)){
		struct macro *node = NULL;
		struct link_node *temp = &macro_head;
		for(temp = temp->next; temp != &macro_head; temp = temp->next, free(node), node = NULL){
			node = ADDRESS(macro, temp, link);
			struct link_node *tempD = &(node->def_head);
			struct def *dnode = NULL;
			for(tempD = tempD->next; tempD != &(node->def_head); tempD=tempD->next, free(dnode), dnode = NULL){
				dnode = ADDRESS(def, tempD, link);
				free(dnode->line);
				dnode->line = NULL;
			}
		}
		init_node(&macro_head);
	}
}