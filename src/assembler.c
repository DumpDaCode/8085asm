/*
	This is the main utility file that contains functions important to the
	working of assembler.
*/
#include "assembler.h"
#include "file_structures.h"
#include "instruction_set.h"
#include "directives.h"
#include "globals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

FILE *pl_file = NULL,
	*st_file = NULL,
	*input_file = NULL,
	*asm_file = NULL;
int line_no = 0, no_of_fields, no_of_operands, program_size;
char file_end = 0;

void init(char *file_path){
/*
	This function initializes all the files and the line counter.
*/
	input_file = fopen(file_path, "r");
	pl_file = fopen("../output/programs.lst","w+");
	st_file = fopen("../output/symbol.lst","w+");
	asm_file = fopen("../output/intermediate.asm","w+");
	if(input_file == NULL){
		fprintf(stderr, "File doesn't exist. Exiting...\n");
		exit(0);
	}
	line_no = 0;
	file_end = 0;
	program_size = 0;
}

char *read_line(FILE *file){
/*
	This function reads a line from the inputted asm file.
	It returns the number of fields in the line, if it contains
	legal charcters.
*/
	int loop=0;
	no_of_fields=1, no_of_operands=0;
	char c, ignore=0, string=0;
	char *buffer = NULL;
	buffer = calloc(100, sizeof(char));
	c=getc(file);
	while(loop<100){
		// Checking Characters for their legality
		if(isalnum(c)){
			buffer[loop++] = toupper(c);
			c=getc(file);
			continue;
		}
		switch(c){
			case ';':
				if(loop == 0)
					no_of_fields -= 1;
				else{
					if(buffer[loop-1] == ' ')
						no_of_fields -= 1;
				}
				for(c=getc(file); \
					c!=EOF && c!='\n'; c=getc(file));
				return buffer;
			case EOF:
				file_end = 1;
			case '\n':
				if(loop == 0)
					no_of_fields -= 1;
				return buffer;
			case ' ':
				if(string == 0){
					buffer[loop++] = '\0';
					no_of_fields++;
				}
				else
					buffer[loop++] = toupper(c);
				for(c=getc(file); \
					c== ' '; c=getc(file));
				break;
			case ',':
				buffer[loop++] = toupper(c);
				for(c=getc(file); \
					c== ' '; c=getc(file));
				if(c!='\n')
					no_of_operands++;
			break;
			case '\'':
				buffer[loop++] = toupper(c);
				c= getc(file);
				string = (string+1)%2;
			break;
			case '\t':
				for(c=getc(file); \
					c=='\t'; c=getc(file));
			break;
			case ':':
				buffer[loop++] = toupper(c);
				c = getc(file);
			break;
			default:
				fprintf(stderr, "%d: Illegal character detected.\n", line_no);
				c = getc(file);
			break;
		}
	}
	if(c != '\n'){
		c = getc(file);
		if(c != '\n'){
			fprintf(stderr, "Line too long to be accomodated in the buffer.\n");
			exit(EXIT_FAILURE);
		}
	}
	else
		buffer[loop++] = '\0';
	return buffer;
}

void assemble(){
/*
	This function performs the main functionality of assebler.
	It takes a single line from the inputted file, processes it,
	updates the appropriate data structures and the end dumps all
	the data structures to their respective files.
*/
	line_no = 0;
	file_end = 0;
	no_of_fields = 0;
	no_of_operands = 0;
	rewind(asm_file);

	char *line = NULL;
	while(file_end == 0){
		line = read_line(asm_file);
		line_no++;
		if(no_of_fields > 3){
			fprintf(stderr, "%d: Illegal number of fields detected.\n", line_no);
		}
		else{
			if(no_of_fields != 0)
				process_tokens(line);
		}
	}
	free(line);
}

void urad(){
/*
	Update References and Dump Code.
*/
	struct link_node *temp = &mc_head;
	temp = &st_head;
	struct st* node;
	char data_section[4], buffer[2];
	int i;
	memset(data_section, 0, 4);
	strcpy(data_section, dth(program_size, 4));
	fprintf(st_file, "Symbol Table:\n\nNAME\tTYPE\tVALUE\t\t\t\t\tDEFINED AT\n\n");
	for(temp = temp->next; temp != &st_head; temp= temp->next){
		node = ADDRESS(st, temp,link);
		if(strcmp(node->value, "-")){
			strcpy(node->defined_at, dth(program_size, 4));
		}
		int len = strlen(node->value);
		for(i = 0; i<len; i=i+2){
			if(node->type == 'L')
				continue;
			strncpy(buffer, node->value+i, 2);
			buffer[2] = '\0';
			mc_add(dth(program_size++, 4), buffer, "-");
		}
		fprintf(st_file, "%-8s%-8c%-24s%-4s\n", node->name ,node->type, node->value, node->defined_at);
	}
	fprintf(st_file, "\nReferrence Table:\n\nADDRESS\t\tSYMBOL\t\n\n");
	fprintf(pl_file, "Machine Code:\n\nADDRESS\t\tDATA\t\n\n");
	temp = &mc_head;
	int freeze = 0;
	char abuffer[5];
	struct mc* snode;
	for(temp = temp->next; temp != &mc_head; temp= temp->next){
		snode = ADDRESS(mc, temp,link);
		switch(freeze){
			case 2: strncpy(snode->data, abuffer+2, 2);
				freeze--;
				break;
			case 1: strncpy(snode->data, abuffer, 2);
				freeze--;
				break;
		}
		fprintf(pl_file, "%-12s%-8s\n", snode->address ,snode->data);
		if(strcmp(snode->symbol, "-")){
			node = check_sym(snode->symbol);
			strcpy(abuffer, node->defined_at);
			freeze = 2;
			fprintf(st_file, "%-12s%-8s\n", snode->address, snode->symbol);
		}
	}
}

void process_macro(){
/*
	This is used for macro processing in the first stage of processing.
*/
	char *line = NULL, *symbol;
	int semi_colon = -1;
	while(file_end == 0){
		line = read_line(input_file);
		line_no++;
		if(no_of_fields > 3){
			fprintf(stderr, "%d: Illegal number of fields detected\n", line_no);
		}
		else{
			if(no_of_fields != 0){
				int length = strlen(line);
				if(strchr(line, ':') != NULL){
					semi_colon = 1;
				}
				else{
					semi_colon = 0;
				}
				symbol = (char *)calloc(strlen(line), sizeof(char));
				strcpy(symbol, line);
				line+=length+1;
				if(no_of_fields >= 2 && !strcmp("MACRO", line)){
					if(no_of_fields == 3){
						switch (semi_colon) {
							case 1:
				                symbol[length - 1] = '\0';
								expand_macro(symbol, line + strlen(line)+1, no_of_operands + 1);
							break;
							case -1:
							case 0:
								add_macro(symbol, line + strlen(line)+1, no_of_operands + 1);
							break;
						}
					}
					else{
						fprintf(stderr, "%d: Illegal Syntax for macro '%s'\n", line_no, symbol);
					}
				}
				else{
					switch (no_of_fields) {
						case 1:
							fprintf(asm_file, "%s\n", symbol);
						break;
						case 2:
							fprintf(asm_file, "%s %s\n", symbol, line);
						break;
						case 3:
							fprintf(asm_file, "%s %s %s\n", symbol, line, line + strlen(line)+1);
						break;
					}
				}
			}
		}
	}
	free(line);
}

void process_tokens(char *line){
/*
	A support function that checks the individual tokens for their
	type and accordingly branches. It checks  the legality of the tokens
	too.
*/
	struct Instruction *instruction = check_if_mnemonic(line);
	int directive = check_if_directive(line);
	int semi_colon = -1;
	int length = strlen(line);
    char *symbol = NULL;
	if(instruction == NULL && !directive){
		if(strchr(line, ':') != NULL){
			if(line[length - 1] == ':'){
                line[length - 1] = '\0';
			}
			semi_colon = 1;
		}
		else{
			semi_colon == 0;
		}
        if(length < 7){
            symbol = (char *)calloc(strlen(line), sizeof(char));
            strcpy(symbol, line);
        }
        else{
            fprintf(stderr, "%d: Symbol name exceeds character limit of 6.\n", line_no);
        }
        line = line+length+1;
	    instruction = check_if_mnemonic(line);
	    directive = check_if_directive(line);
		no_of_fields--;
	}
	if(instruction != NULL){
		length = strlen(line);
		if(semi_colon == 0){
			fprintf(stderr, "%d: ':' missing from label\n", line_no);
		}
        else if(semi_colon == 1 && symbol != NULL){
            st_add(symbol, 'L', "-", dth(program_size, 4), program_size);
        }
		if((no_of_operands + 1) == instruction->no_of_operands && no_of_fields == 2){
			if(!process_instructions(instruction, line+length+1)){
				fprintf(stderr, "%d: Operands do not match specification of %s instruction.\n", line_no, instruction->name);
			}
		}
		else{
			fprintf(stderr, "%d: Number of operands differ from specification of %s.\n", line_no, instruction->name);
		}
	}
	if(directive){
		if(no_of_fields == 1 && !strcmp("END", line)){
			file_end = 1;
		}
		else if(no_of_fields == 2 && !strcmp("ORG", line)){
			line = line+strlen(line)+1;
			if(strlen(line)>5){
				fprintf(stderr, "%d: Incorrect address specified for 'ORG'\n", line_no);
				return;
			}
			int buffer = 0, len = line[strlen(line) - 1] == 'H' ? strlen(line) - 2 :  strlen(line) - 1, current = 0, i;
			for(i = len; i >= 0; i--){
				if(line[i] <= 70 && line[i] >= 65){
					current = (int)line[i] - 55;
				}
				if(line[i] <= 57 && line[i] >= 48){
					current = (int)line[i] - 48;
				}
				buffer = buffer + current * pow(16, len - i);
			}
			program_size = buffer;
		}
		else{
			if(no_of_operands <= 8){
				process_directives(symbol, line, semi_colon);
			}
			else{
				fprintf(stderr, "%d: Too many arguments specified.\n", line_no);
			}
		}
	}
	return;
}

char *evaluate_operands(char *string){
/*
	Individual processed tokens are passed to this function for type
	checking.
*/
	int len = strlen(string);
	char *machine_defined = "ABCDEHLM", *expr = "+-/*)($",
		*buffer = (char *)calloc(len - 2, sizeof(char)), *op;
	if(string[0] == '\'' && string[strlen(string) - 1] == '\''){
		//Ascii to Hex Conversion:
		if(strlen(string) == 3){
			return dth((int)string[1], 2);
		}
	}
    else{
        switch(len){
            case 1:
                if(strstr(machine_defined, string) != NULL){
                    return "-";
                }
                return NULL;
            case 2:
                if(strcmp(string, "SP") == 0)
                    return "-";
            case 3:
            case 4:
                if(string[len - 1] == 'D'){
                    strncpy(buffer, string, len - 1);
                    return vacin('D', "0123456789", buffer, 2);
                }
                if(string[len - 1] == 'Q'){
                    strncpy(buffer, string, len - 1);
                    return vacin('Q', "01234567", buffer, 2);
                }
            case 5:
				if(string[len -1] == 'H'){
					if(len < 4){
						strncpy(buffer, string, len - 1);
						return vacin('H', "0123456789ABCDEF", buffer, 2);
					}
					strncpy(buffer, string, len - 1);
					return vacin('H', "0123456789ABCDEF", buffer, 4);
				}
            case 6:
            case 7:
            case 8:
            case 9:
                if(string[len - 1] == 'B'){
					strncpy(buffer, string, len - 1);
                    return vacin('B', "01", buffer, 2);
                }
			default:
				if(check_sym(string) == NULL){
					st_add(string, '-', "-", "-", 0);
				}
				return "???";
        }
    }
}

char *vacin(char type, char *va_str, char* str, int length){
/*
	Validate And Convert If Neccesary
*/
	int var;
	char *data;
	if(va_str != NULL){
		for(var = strlen(str) - 1; var >=0; var--){
			if(!strchr(va_str, str[var]))
				return NULL;
		}
	}
	int number = atoi(str);
	int dec = 0, i = 0, base;
	switch(type){
		case 'B':
			base = 2;
			break;
		case 'Q':
			base = 8;
			break;
		case 'D':
			if(va_str == NULL)
				dec = (int)str[0];
			else
				dec = atoi(str);
			break;
		case 'H':
			data = (char *)calloc(length, sizeof(char));
			memset(data, 48, length - strlen(str));
			strcat(data, str);
			return data;
		default:
			return NULL;
	}
	if(dec == 0){
		while(number != 0){
			dec += (number%10)*pow(base, i);
			number = number/10;
			i++;
		}
		return dth(dec, length);
	}
	else if(dec<255){
		return dth(dec, length);
	}
	return NULL;
}

char *dth(int dec, int length){
	char *buffer = (char *)calloc(length, sizeof(char));
	char *hex_list = "0123456789ABCDEF";
    int i;
	for(i= length - 1; i>=0; i--){
		buffer[i] = hex_list[dec%16];
		dec /= 16;
	}
    return buffer;
}
