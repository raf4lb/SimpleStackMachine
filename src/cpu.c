#ifndef CPU_H
#define CPU_H

#include "cpu.h"
#include <stdlib.h>

CPU *cpu_create(int memory_size, int stack_size, void (**instructions)(CPU *cpu))
{
    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    if (cpu == NULL)
    {
        printf("Memory allocation to CPU failed");
        exit(EXIT_FAILURE);
    }
    Memory *memory = memory_create(memory_size);
    cpu->memory = memory;
    Stack *stack = stack_create(stack_size);
    cpu->stack = stack;
    cpu->instructions = instructions;
    return cpu;
}

void cpu_free(CPU *cpu)
{
    stack_free(cpu->stack);
    memory_free(cpu->memory);
    free(cpu->instructions);
    free(cpu);
}

uint8_t cpu_fetch_8b(CPU *cpu)
{
    return memory_get_address(cpu->memory, cpu->ip++);
}

uint16_t cpu_fetch_16b(CPU *cpu)
{
    return cpu_fetch_8b(cpu) << 8 | cpu_fetch_8b(cpu);
}

void cpu_execute(CPU *cpu, uint8_t opcode)
{
    cpu->instructions[opcode](cpu);
}

void cpu_load_program(CPU *cpu, uint8_t *program, int program_size)
{
    int i;
    for (i = 0; i < program_size; i++)
        memory_set_address(cpu->memory, i, program[i]);
}

void cpu_run(CPU *cpu)
{
    while (1)
    {
        uint8_t opcode = cpu_fetch_8b(cpu);
        cpu_execute(cpu, opcode);
    }
}

#endif