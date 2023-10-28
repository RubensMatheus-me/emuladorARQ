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

int index = 1;

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
uint16_t pc;
uint16_t instruction;

uint16_t getMemory(int i) {
    i++;
    return memory[i];
}

Instructions decode(uint16_t codedInstruction) {
    Instructions decoded;
    decoded.format = extract_bits(codedInstruction, 15, 1);

    switch (decoded.format) {
        case 0: // formato R
            decoded.opcode = extract_bits(codedInstruction, 9, 6);
            decoded.destiny = extract_bits(codedInstruction, 8, 3);
            decoded.op1 = extract_bits(codedInstruction, 5, 3);
            decoded.op2 = extract_bits(codedInstruction, 2, 3);
            break;
        case 1: // formato I
            decoded.imediate = extract_bits(codedInstruction, 10, 2);
            decoded.opcode = extract_bits(codedInstruction, 9, 6);
            decoded.destiny = extract_bits(codedInstruction, 8, 3);
            break;
    }

    return decoded;
}

void syscall() {
    switch (registers[0]) {
        case SYSCALLEXIT:
            exit(0);
            break;
    }
}

void execute(Instructions instructions) {
    switch (instructions.format) {
        case 0: // formato R
            switch (instructions.opcode) {
                case 0:
                    registers[instructions.destiny] = registers[instructions.op1] + registers[instructions.op2];
                    printf("Valor do opcode: %u, soma: %u\n", instructions.opcode, registers[instructions.destiny]);
                    break;
                case 1:
                    registers[instructions.destiny] = registers[instructions.op1] - registers[instructions.op2];
                    printf("Valor do opcode: %u, subtracao: %u\n", instructions.opcode, registers[instructions.destiny]);
                    break;
                case 2:
                    registers[instructions.destiny] = registers[instructions.op1] * registers[instructions.op2];
                    printf("Valor do opcode: %u, multiplicacao: %u\n", instructions.opcode, registers[instructions.destiny]);
                    break;
                case 3:
                    registers[instructions.destiny] = registers[instructions.op1] / registers[instructions.op2];
                    printf("Valor do opcode: %u, divisao: %u\n", instructions.opcode, registers[instructions.destiny]);
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
                    printf("Movido %u para %s\n", instructions.imediate, get_reg_name_str(instructions.destiny));
                    break;
            }
            break;
    }
}

int main(int argc, char **argv) {

    while (1) {
        instruction = getMemory(pc);
        Instructions decoded = decode(instruction);
        if (decoded.opcode == SYSCALL) {
            syscall();
            break;
        }
        execute(decoded);
        pc++;
    }

    if (argc != 2) {
        printf("usage: %s [bin_name]\n", argv[0]);
        exit(1);
    }

    return 0;
}
