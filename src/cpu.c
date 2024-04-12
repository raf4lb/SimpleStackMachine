#include <stdlib.h>
#include "cpu.h"
#include "stack.h"
#include "io.h"

CPU *cpu_create(uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, void (**instructions)(CPU *cpu), uint8_t port_bank)
{
    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    if (cpu == NULL)
    {
        vmprintf("Memory allocation to CPU failed");
        exit(EXIT_FAILURE);
    }
    cpu->memory = memory_create(memory_size);
    cpu->stack = stack_create(stack_size);
    cpu->callstack = stack_create(callstack_size);
    cpu->instructions = instructions;
    cpu->port_bank = port_bank_create(port_bank);
    cpu->user_memory = 0;
    return cpu;
}

void cpu_free(CPU *cpu)
{
    stack_free(cpu->stack);
    memory_free(cpu->memory);
    port_bank_free(cpu->port_bank);
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

void cpu_load_program(CPU *cpu, uint8_t *program, uint16_t program_size)
{
    int i;
    for (i = 0; i < program_size; i++)
    {
        memory_set_address(cpu->memory, i, program[i]);
    }
    cpu->user_memory = program_size;
}

void cpu_run(CPU *cpu)
{
    while (1)
    {
        uint8_t opcode = cpu_fetch_8b(cpu);
        cpu_execute(cpu, opcode);
    }
}

void cpu_print_user_memory(CPU *cpu)
{
    vmprintf("[");
    uint16_t i;
    for (i = cpu->user_memory; i < cpu->memory->size; i++)
    {
        if (i > cpu->user_memory)
        {
            vmprintf(", ");
        }
        vmprintf("%u", cpu->memory->data[i]);
    }
    vmprintf("]\n");
}