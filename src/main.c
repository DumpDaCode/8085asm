/*
	The main point of entry to assembler.
	Date: 22-06-2021
*/
#include "assembler.h"

int main(char argc, char *argv[]){
	if(argc == 2){
		init(argv[1]);
		fprintf(stdout, "Processing input file for Macros. (All line numbers corresspond to input file)\n");
		process_macro();
		fprintf(stdout, "Processing intermediate file. (All line numbers corresspond to intermediate file)\n");
		assemble();
		fprintf(stdout, "Updating references and dumping code to respective files in output/\n");
		urad();
		fprintf(stdout, "Done. :)\n");
	}
	else{
		fprintf(stderr, "Unrecognizable usage format\n");
	}
}
