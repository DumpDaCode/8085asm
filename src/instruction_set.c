/*
	This is instruction set of 8085 in a C struct along with few important
	functions to deal with instructions.
*/
#include "instruction_set.h"
#include "globals.h"
#include "assembler.h"
#include "file_structures.h"
#include <string.h>
#include <stdlib.h>

struct Opcode_list for_aci = {"CE","?"},
	 for_adi = {"C6","?"},
	 for_ani = {"E6","?"},
	 for_call = {"CD", "??"},
	 for_cc = {"DC", "??"},
	 for_cm = {"FC", "??"},
	 for_cma = {"2F", ""},
	 for_cmc = {"3F", ""},
	 for_cnc = {"D4", "??"},
	 for_cnz = {"C4", "??"},
	 for_cp = {"F4", "??"},
	 for_cpe = {"EC", "??"},
	 for_cpi = {"FE", "??"},
	 for_cpo = {"E4", "??"},
	 for_cz = {"CC", "??"},
	 for_daa = {"27", ""},
	 for_di = {"F3", ""},
	 for_ei = {"FB", ""},
	 for_halt = {"76", ""},
	 for_in = {"DB", "?"},
	 for_jc = {"DA", "??"},
	 for_jm = {"FA", "??"},
	 for_jmp = {"C3", "??"},
	 for_jnc = {"D2", "??"},
	 for_jnz = {"C2", "??"},
	 for_jp = {"F2", "??"},
	 for_jpe = {"EA", "??"},
	 for_jpo = {"E2", "??"},
	 for_jz = {"CA", "??"},
	 for_lda = {"3A", "??"},
	 for_lhld = {"2A", "??"},
	 for_nop = {"00", ""},
	 for_ori = {"F6", "??"},
	 for_out = {"D3", "??"},
	 for_pchl = {"E9", ""},
	 for_ral = {"17", ""},
	 for_rar = {"1F", ""},
	 for_rc = {"D8", ""},
	 for_ret = {"C9", ""},
	 for_rim = {"20", ""},
	 for_rlc = {"07", ""},
	 for_rm = {"F8", ""},
	 for_rnc = {"D0", ""},
	 for_rnz = {"C0", ""},
	 for_rp = {"F0", ""},
	 for_rpe = {"E8", ""},
	 for_rpo = {"E0", ""},
	 for_rrc = {"0F", ""},
	 for_sbi = {"DE", "?"},
	 for_shld = {"22", "???"},
	 for_sim = {"30", ""},
	 for_sphl = {"F9", ""},
	 for_sta = {"32", "??"},
	 for_stc = {"37", ""},
	 for_sui = {"D6", "?"},
	 for_xchg = {"EB", ""},
	 for_xri = {"EE", "?"},
	 for_xhtl = {"E3", ""};

struct Opcode_list for_adc[8] = {
	{"8F", "A"},
	{"88", "B"},
	{"89", "C"},
	{"8A", "D"},
	{"8B", "E"},
	{"8C", "H"},
	{"8D", "L"},
	{"8E", "M"}
};

struct Opcode_list for_add[8] = {
	{"87", "A"},
	{"80", "B"},
	{"81", "C"},
	{"82", "D"},
	{"83", "E"},
	{"84", "H"},
	{"85", "L"},
	{"86", "M"}
};

struct Opcode_list for_ana[8] = {
	{"A7","A"},
	{"A0","B"},
	{"A1","C"},
	{"A2","D"},
	{"A3","E"},
	{"A4","H"},
	{"A5","L"},
	{"A6","M"},
};

struct Opcode_list for_cmp[8] = {
	{"BF","A"},
	{"B8","B"},
	{"B9","C"},
	{"BA","D"},
	{"BB","E"},
	{"BC","H"},
	{"BD","L"},
	{"BE","M"},
};

struct Opcode_list for_dad[4] = {
	{"09","B"},
	{"19","D"},
	{"29","H"},
	{"39","SP"}
};

struct Opcode_list for_dcr[8] = {
	{"3D","A"},
	{"05","B"},
	{"0D","C"},
	{"15","D"},
	{"1D","E"},
	{"25","H"},
	{"2D","L"},
	{"35","M"},
};

struct Opcode_list for_dcx[4] = {
	{"0B","B"},
	{"1B","D"},
	{"2B","H"},
	{"3B","SP"},
};

struct Opcode_list for_inr[8] = {
	{"3C","A"},
	{"04","B"},
	{"0C","C"},
	{"14","D"},
	{"1C","E"},
	{"24","H"},
	{"2C","L"},
	{"34","M"},
};

struct Opcode_list for_inx[4] = {
	{"03","B"},
	{"13","D"},
	{"23","H"},
	{"33","SP"}
};

struct Opcode_list for_ldax[2] = {
	{"0A","B"},
	{"1A","D"}
};

struct Opcode_list for_lxi[4] = {
	{"01","B,??"},
	{"11","D,??"},
	{"21","H,??"},
	{"31","SP,??"}
};

struct Opcode_list for_mov[63] = {
	{"7F","A,A"},
	{"78","A,B"},
	{"79","A,C"},
	{"7A","A,D"},
	{"7B","A,E"},
	{"7C","A,H"},
	{"7D","A,L"},
	{"7E","A,M"},
	{"47","B,A"},
	{"40","B,B"},
	{"41","B,C"},
	{"42","B,D"},
	{"43","B,E"},
	{"44","B,H"},
	{"45","B,L"},
	{"46","B,M"},
	{"4F","C,A"},
	{"48","C,B"},
	{"49","C,C"},
	{"4A","C,D"},
	{"4B","C,E"},
	{"4C","C,H"},
	{"4D","C,L"},
	{"4E","C,M"},
	{"57","D,A"},
	{"50","D,B"},
	{"51","D,C"},
	{"52","D,D"},
	{"53","D,E"},
	{"54","D,H"},
	{"55","D,L"},
	{"56","D,M"},
	{"7F","E,A"},
	{"58","E,B"},
	{"59","E,C"},
	{"5A","E,D"},
	{"5B","E,E"},
	{"5C","E,H"},
	{"5D","E,L"},
	{"5E","E,M"},
	{"67","H,A"},
	{"60","H,B"},
	{"61","H,C"},
	{"62","H,D"},
	{"63","H,E"},
	{"64","H,H"},
	{"65","H,L"},
	{"66","H,M"},
	{"6F","L,A"},
	{"68","L,B"},
	{"69","L,C"},
	{"6A","L,D"},
	{"6B","L,E"},
	{"6C","L,H"},
	{"6D","L,L"},
	{"6E","L,M"},
	{"77","M,A"},
	{"70","M,B"},
	{"71","M,C"},
	{"72","M,D"},
	{"73","M,E"},
	{"74","M,H"},
	{"75","M,L"},
};

struct Opcode_list for_mvi[8] = {
	{"3E","A,?"},
	{"06","B,?"},
	{"0E","C,?"},
	{"16","D,?"},
	{"1E","E,?"},
	{"26","H,?"},
	{"2E","L,?"},
	{"36","M,?"},
};

struct Opcode_list for_pop[8] = {
	{"C1","B"},
	{"D1","D"},
	{"E1","H"},
	{"F1","PSW"},
};

struct Opcode_list for_push[8] = {
	{"C5","B"},
	{"D5","D"},
	{"E5","H"},
	{"F5","PSW"},
};

struct Opcode_list for_rst[8] = {
	{"C7","0"},
	{"CF","1"},
	{"D7","2"},
	{"DF","3"},
	{"E7","4"},
	{"EF","5"},
	{"F7","6"},
	{"FF","7"},
};

struct Opcode_list for_sbb[8] = {
	{"9F","A"},
	{"98","B"},
	{"99","C"},
	{"9A","D"},
	{"9B","E"},
	{"9C","H"},
	{"9D","L"},
	{"9E","M"},
};

struct Opcode_list for_stax[2] = {
	{"02","B"},
	{"12","D"}
};

struct Opcode_list for_sub[8] = {
	{"97","A"},
	{"90","B"},
	{"91","C"},
	{"92","D"},
	{"93","E"},
	{"94","H"},
	{"95","L"},
	{"96","M"}
};

struct Opcode_list for_xra[8] = {
	{"AF","A"},
	{"A8","B"},
	{"A9","C"},
	{"AA","D"},
	{"AB","E"},
	{"AC","H"},
	{"AD","L"},
	{"AE","M"}
};

struct Instruction instructions[NO_OF_INSTRUCTIONS] = {
	{"ACI", 1, 2, 1, &for_aci},
	{"ADC", 1, 1, 8, for_adc},
	{"ADD", 1, 1, 8, for_add},
	{"ADI", 1, 2, 1, &for_adi},
	{"ANA", 1, 1, 8, for_ana},
	{"ANI", 1, 2, 1, &for_ani},
	{"CALL", 1, 3, 1, &for_call},
	{"CC", 1, 3, 1, &for_cc},
	{"CM", 1, 3, 1, &for_cm},
	{"CMA", 0, 1, 1, &for_cma},
	{"CMC", 0, 1, 1, &for_cmc},
	{"CNC", 1, 3, 1, &for_cnc},
	{"CNZ", 1, 3, 1, &for_cnz},
	{"CP", 1, 3, 1, &for_cp},
	{"CPE", 1, 3, 1, &for_cpe},
	{"CPI", 1, 2, 1, &for_cpi},
	{"CPO", 1, 3, 1, &for_cpo},
	{"CZ", 1, 3, 1, &for_cz},
	{"DAA", 0, 1, 1, &for_daa},
	{"DAD", 1, 1, 1, for_dad},
	{"DCR", 1, 1, 1, for_dcr},
	{"DCX", 1, 1, 1, for_dcx},
	{"DI", 0, 1, 1, &for_di},
	{"EI", 0, 1, 1, &for_ei},
	{"HALT", 0, 1, 1, &for_halt},
	{"IN", 1, 1, 1, &for_in},
	{"INR", 1, 1, 8, for_inr},
	{"INX", 1, 1, 1, for_inx},
	{"JC", 1, 3, 1, &for_jc},
	{"JM", 1, 3, 1, &for_jm},
	{"JMP", 1, 3, 1, &for_jmp},
	{"JNC", 1, 3, 1, &for_jnc},
	{"JNZ", 1, 3, 1, &for_jnz},
	{"JP", 1, 3, 1, &for_jp},
	{"JPE", 1, 3, 1, &for_jpe},
	{"JPO", 1, 3, 1, &for_jpo},
	{"JZ", 1, 3, 1, &for_jz},
	{"LDA", 1, 3, 1, &for_lda},
	{"LHLD", 1, 3, 1, &for_lhld},
	{"LDAX", 1, 1, 2, for_ldax},
	{"LXI", 2, 3, 4, for_lxi},
	{"MOV", 2, 1, 63, for_mov},
	{"MVI", 2, 2, 8, for_mvi},
	{"NOP", 0, 1, 1, &for_nop},
	{"ORI", 1, 2, 1, &for_ori},
	{"OUT", 1, 2, 1, &for_out},
	{"PCHL", 0, 1, 1, &for_pchl},
	{"POP", 1, 1, 4, for_pop},
	{"PUSH", 1, 1, 4, for_push},
	{"RAL", 0, 1, 1, &for_ral},
	{"RAR", 0, 1, 1, &for_rar},
	{"RC", 0, 1, 1, &for_rc},
	{"RET", 0, 1, 1, &for_ret},
	{"RIM", 0, 1, 1, &for_rim},
	{"RLC", 0, 1, 1, &for_rlc},
	{"RM", 0, 1, 1, &for_rm},
	{"RNC", 0, 1, 1, &for_rnc},
	{"RNZ", 0, 1, 1, &for_rnz},
	{"RP", 0, 1, 1, &for_rp},
	{"RPE", 0, 1, 1, &for_rpe},
	{"RPO", 0, 1, 1, &for_rpo},
	{"RRC", 0, 1, 1, &for_rrc},
	{"RST", 1, 1, 1, for_rst},
	{"SBB", 1, 1, 1, for_sbb},
	{"SBI", 1, 2, 1, &for_sbi},
	{"SHLD", 1, 3, 1, &for_shld},
	{"SIM", 0, 1, 1, &for_sim},
	{"SPHL", 0, 1, 1, &for_sphl},
	{"STA", 1, 3, 1, &for_sta},
	{"STC", 0, 1, 1, &for_stc},
	{"STAX", 1, 1, 1, for_stax},
	{"SUB", 1, 1, 1, for_sub},
	{"SUI", 1, 2, 1, &for_sui},
	{"XCHG", 0, 1, 1, &for_xchg},
	{"XRI", 1, 2, 1, &for_xri},
	{"XHTL", 0, 1, 1, &for_xhtl},
	{"XRA", 1, 1, 1, for_xra},
};

struct Instruction *check_if_mnemonic(char *string){
/*
	A support function for checking and returning the corressponding
	instruction.
*/
	int i;
	for(i = 0; i < NO_OF_INSTRUCTIONS; i++){
		if(!strcmp(instructions[i].name,  string))
			return &instructions[i];
	}
	return NULL;
}

int process_instructions(struct Instruction *instruction, char *operands){
/*
	A support function for processing instructions.
*/
	char *buffer = (char *)calloc(5, sizeof(char)), *result = NULL, *symbol = NULL;
	strtok(operands, ",");
	while(operands != NULL){
		if(operands[0] == '0' && operands[strlen(operands) - 1] == 'H'){
			result = evaluate_operands(operands+1);
		}
		else{
			result = evaluate_operands(operands);
		}
		if(result != NULL){
			if(!strcmp("???", result)){
				strcat(buffer, "??");
				symbol = (char *)calloc(strlen(operands), sizeof(char));
				strcpy(symbol, operands);
			}
			else if(!strcmp("-", result)){
				strcat(buffer, operands);
			}
			else if(strlen(result)<=2){
				strcat(buffer, "?");
			}
			else if(strlen(result)<=4){
				strcat(buffer, "??");
			}
		}
		else{
			return 0;
		}
		if(no_of_operands-- != 0){
			strcat(buffer, ",");
		}
		operands = strtok(NULL, ",");
	}
	int i;
	for(i = 0; i < instruction->no_of_variants; i++){
		if(!strcmp(instruction->list[i].operand, buffer))
			break;
	}

	if(i != instruction->no_of_variants){
		char data[3];
		if(symbol != NULL)
			mc_add(dth(program_size++, 4), instruction->list[i].opcode, symbol);
		else
			mc_add(dth(program_size++, 4), instruction->list[i].opcode, "-");
		for(int length = 1; length < instruction->bytes; length++){
			if(symbol != NULL){
				mc_add(dth(program_size++, 4), "??", "-");
			}
			else{
                if(2*length <= strlen(result))
    				strncpy(data, result + strlen(result) - 2*length, 2);
                else
                    strcpy(data, "00");
				mc_add(dth(program_size++, 4), data, "-");
			}
		}
		return 1;
	}
	return 0;
}
