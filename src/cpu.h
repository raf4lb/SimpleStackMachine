#ifndef CPU_H
#define CPU_H

#include "stack.h"
#include "io.h"
#include <stdint.h>

typedef struct CPU CPU;

typedef void (*InstructionPtr)(CPU *cpu);

struct CPU
{
    Memory *memory;
    uint8_t *program;
    uint16_t program_size;
    Stack *stack;
    Stack *callstack;
    uint16_t ip;
    InstructionPtr *instructions;
    PortBank *port_bank;
    uint16_t user_memory;
    uint16_t data_memory;
};

CPU *cpu_create(uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, InstructionPtr *instruction_set, uint8_t port_banks);

void cpu_free(CPU *cpu);

uint8_t cpu_fetch_8b(CPU *cpu);

uint16_t cpu_fetch_16b(CPU *cpu);

void cpu_execute(CPU *cpu, uint8_t opcode);

void cpu_load_program(CPU *cpu, uint8_t *program, uint16_t program_size, uint16_t data_address);

void cpu_run(CPU *cpu);

void cpu_print_user_memory(CPU *cpu);

#endif
