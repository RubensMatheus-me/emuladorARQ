#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

#include "lib.h"

#define MEM_SIZE 1024

#define SYSCALLEXIT 0
#define SYSCALL 1
#define SYSCALLNEWLINE 2
#define SYSCALLINT 3

const char* get_reg_name_str(uint16_t reg) {
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
    uint16_t imediate;
} Instructions;

uint16_t extract_bits(uint16_t v, uint8_t bstart, uint8_t blength);
uint16_t memory[MEM_SIZE];
uint16_t registers[8];
uint16_t pc = 1;
uint16_t instruction;

uint16_t getMemory(int i) {
    return memory[i];
    pc++;
}

Instructions decode(uint16_t codedInstruction) {
    Instructions decoded;
    decoded.format = extract_bits(codedInstruction, 15, 1);

    switch (decoded.format) {
        case 0: // formato R
            decoded.opcode = extract_bits(codedInstruction, 9, 6);
            decoded.destiny = extract_bits(codedInstruction, 6, 3);
            decoded.op1 = extract_bits(codedInstruction, 3, 3);
            decoded.op2 = extract_bits(codedInstruction, 0, 3);
            break;
        case 1: // formato I
            decoded.imediate = extract_bits(codedInstruction, 13, 2);
            decoded.opcode = extract_bits(codedInstruction, 10, 3);
            decoded.destiny = extract_bits(codedInstruction, 0, 10);
            break;
    }

    return decoded;
}


void execute(Instructions instructions) {
    switch (instructions.format) {
        case 0: // formato R
            switch (instructions.opcode) {
                case 0: //add
			registers[instructions.destiny] = registers[instructions.op1] + registers[instructions.op2];
			break;
                case 1: //sub
			registers[instructions.destiny] = registers[instructions.op1] - registers[instructions.op2];
			break;
		case 2: //mult
			registers[instructions.destiny] = registers[instructions.op1] * registers[instructions.op2];
			break;
		case 3: //div
			registers[instructions.destiny] = registers[instructions.op1] / registers[instructions.op2];
			break;
                case 4: //cmp_equal
            		registers[instructions.destiny] = registers[instructions.op1] == registers[instructions.op2];
                	break;
		case 5: //cmp_neq
			registers[instructions.destiny] = registers[instructions.op1] != registers[instructions.op2];
                	break;
                case 6:
                	registers[instructions.destiny] = registers[instructions.op1] < registers[instructions.op2];
                	break;
                case 7:
                	registers[instructions.destiny] = registers[instructions.op1] > registers[instructions.op2];
                	break;
                case 8:
                	registers[instructions.destiny] = registers[instructions.op1] <= registers[instructions.op2];
                	break;
                case 9:
                	registers[instructions.destiny] = registers[instructions.op1] >= registers[instructions.op2];
                	break;
                case 10:
                	registers[instructions.destiny] = registers[instructions.op1] & registers[instructions.op2];
                	break;
                case 11:
              		registers[instructions.destiny] = registers[instructions.op1] | registers[instructions.op2];
              		break;
              	case 12:
              		registers[instructions.destiny] = registers[instructions.op1] ^ registers[instructions.op2];
              		break;
              	case 13:
              		registers[instructions.destiny] = registers[instructions.op1] >> registers[instructions.op2];
              		break;
              	case 14:
              		registers[instructions.destiny] = registers[instructions.op1] << registers[instructions.op2];
              		break;
              	case 15:
              		registers[instructions.destiny] = memory[registers[instructions.op1]];
              	//memory[MEM_SIZE]
              	case 16:
              		memory[registers[instructions.op1]] = registers[instructions.op2];
              		break;
                case 63:
                	switch (registers[0]) {
        			case SYSCALLEXIT:
            				exit(0);
            				break;
            			case SYSCALL:
            			uint16_t start = registers[1];
				while (memory[start] != '\0') { 
					printf("%c", memory[start]);
					start++;
				}
					break;	

				case SYSCALLNEWLINE:
					printf("\n");
					break;

				case SYSCALLINT:
					printf("%u\n", registers[1]);
					break;
    			}	
			break;
            }

            break;
        case 1: // formato I
            switch (instructions.opcode) {
                case 0: // jump
                    pc = instructions.imediate;
                    break;
                case 1: // jump condicional
                    if (registers[instructions.destiny]) {
                        pc = instructions.imediate;
                    }
                    break;
                case 3: // Mover valor imediato para o registrador destiny
                    registers[instructions.destiny] = instructions.imediate;
                    break;
            }
            break;
    }
}

int main(int argc, char **argv) {
	load_binary_to_memory (argv[1], memory, MEM_SIZE);

	while (1) {
    		uint16_t cInstruction =  getMemory(pc);
    		Instructions decoded = decode(cInstruction);
    		execute(decoded);
	}


    if (argc != 2) {
        printf("usage: %s [bin_name]\n", argv[0]);
        exit(1);
    }

    return 0;
}
