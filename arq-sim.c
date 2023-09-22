#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "lib.h"



const char* get_reg_name_str (uint16_t reg)
{
	static const char *str[] = {
		"r0",
		"r1",
		"r2",
		"r3",
		"r4",
		"r5",
		"r6",
		"r7"
	};

	return str[reg];
}

typedef struct instruction {
	uint8_t format;
	uint8_t opcode;
	uint8_t destiny;
	uint8_t op1;
	uint8_t op2;

}Instruction;

uint16_t extract_bits (uint16_t v, uint8_t bstart, uint8_t blength);
uint16_t memory[64]; //memoria
int registers[8]; //8 registradores disponiveis


int main (int argc, char **argv) {	
	struct instruction instruction;
	
	memory[0] = 0b0000011101110001; //0 000000 101 110 001 add
	
	uint8_t formatM0 = extract_bits(memory[0], 15, 1); //formato
	uint8_t opcodeM0 = extract_bits(memory[0], 9, 6); //opcode
	
	memory[1] = 0b0000000101110001; // 0 000001 111 111 010 sub
	
	instruction.format = extract_bits(memory[0], 15, 1); //formato
	instruction.opcode = extract_bits(memory[1], 9, 6); //opcode
	instruction.destiny = extract_bits(memory[1], 8, 3); //registrador 1
	instruction.op1 = extract_bits(memory[1], 5, 3); //registrador 2
	instruction.op2 = extract_bits(memory[1], 2, 3); //registrador 3
	registers[instruction.destiny] = instruction.destiny;
	registers[instruction.op1] = instruction.op1;
	registers[instruction.op2] = instruction.op2;
	
	
	printf("formato: %u\n", instruction.format);
	printf("opcode: %u\n", instruction.opcode);
	printf("r1: %u\n", registers[instruction.destiny]);
	printf("r2: %u\n", registers[instruction.op1]);
	printf("r3: %u\n", registers[instruction.op2]);

	
	switch (instruction.format) {
		case 0:
			switch (instruction.opcode) {
				case 0:
					registers[instruction.destiny] = registers[instruction.op1] + registers[instruction.op2];
					printf("Valor do opcode: %u, soma: %u\n", instruction.opcode, registers[instruction.destiny]);
					break;
					
				case 1:
					registers[instruction.destiny] = registers[instruction.op1] - registers[instruction.op2];
					printf("Valor do opcode: %u, subtracao: %u\n", instruction.opcode, registers[instruction.destiny]);	
					break;
					
				case 2:
					registers[instruction.destiny] = registers[instruction.op1] * registers[instruction.op2];
					printf("Valor do opcode: %u, multiplicacao: %u\n", instruction.opcode, registers[instruction.destiny]);	
					break;
					
				case 3:
					registers[instruction.destiny] = registers[instruction.op1] / registers[instruction.op2];
					printf("Valor do opcode: %u, divisao: %u\n", instruction.opcode, registers[instruction.destiny]);	
					break;
					 	
				case 4:
					
					
			}	
	}	
	
	
	
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}

	return 0;
}
