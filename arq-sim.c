#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "lib.h"

typedef int bool;
#define MEM_SIZE 1024

#define SYSCALLEXIT 0
#define SYSCALL 1
#define SYSCALLINT 2
#define SYSCALLNEWLINE 3

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

typedef struct instructions {
	uint16_t format;
	uint16_t opcode;
	uint16_t destiny;
	uint16_t op1;
	uint16_t op2;
	uint16_t opI;

}Instructions;

uint16_t extract_bits (uint16_t v, uint8_t bstart, uint8_t blength);
uint16_t memory[MEM_SIZE]; //memoria
uint16_t registers[8]; //8 registradores disponiveis
uint16_t pc;
uint16_t instruction;


uint16_t getMemory(int i) {
	return memory[i];
}


Instructions decode(uint16_t codedInstruction) {
	Instructions decoded;
	
	switch (decoded.format) {
		case 1: //formato I
			decoded.opI = extract_bits(codedInstruction, 10, 2);	
	}
	decoded.format = extract_bits(codedInstruction, 15, 1); //formato
	decoded.opcode = extract_bits(codedInstruction, 9, 6); //opcode
	decoded.destiny = extract_bits(codedInstruction, 8, 3); //registrador 1
	decoded.op1 = extract_bits(codedInstruction, 5, 3); //registrador 2
	decoded.op2 = extract_bits(codedInstruction, 2, 3); //registrador 3
	
	registers[decoded.destiny] = decoded.destiny;
	registers[decoded.op1] = decoded.op1;
	registers[decoded.op2] = decoded.op2; 
	
	return decoded;

}


	
int main (int argc, char **argv) {
	
	memory[1] = 0b0000001111111010; // 0 000001 111 111 010 sub
	
	pc = getMemory(1);
	instruction = pc;
	
	
	decode(instruction);
	
	
	
	
	/*
	uint8_t formatM0 = extract_bits(memory[0], 15, 1); //formato
	uint8_t opcodeM0 = extract_bits(memory[0], 9, 6); //opcode
	
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
			}	
	}	
	*/
	
	
	
	if (argc != 2) {
		printf("usage: %s [bin_name]\n", argv[0]);
		exit(1);
	}
	

	return 0;
}
