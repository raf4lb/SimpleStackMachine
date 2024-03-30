#include "instructions.h"
#include <stdlib.h>

void halt(CPU *cpu)
{
    printf("Exit\n");
    exit(0);
}

void push(CPU *cpu)
{
    // push a 16 bit value from a 16bit address to stack
    uint16_t address = cpu_fetch_16b(cpu);
    uint8_t h = memory_get_address(cpu->memory, address) << 8;
    uint8_t l = memory_get_address(cpu->memory, address + 1);
    uint16_t value = h | l;
    stack_push(cpu->stack, value);
}

void push_literal(CPU *cpu)
{
    // push a 16 bit value to stack
    uint16_t value = cpu_fetch_16b(cpu);
    stack_push(cpu->stack, value);
}

void pop(CPU *cpu)
{
    stack_pop(cpu->stack);
}

void top(CPU *cpu)
{
    printf("%d\n", cpu->stack->data[cpu->stack->sp - 1]);
}

void (**instructions_create())()
{
    void (**instructions)() = malloc(INSTRUCTIONS * sizeof(void (*)()));
    if (instructions == NULL)
    {
        printf("Memory allocation failed for instructions");
        return NULL;
    }
    instructions[0] = halt;
    instructions[1] = push_literal;
    instructions[2] = push;
    instructions[3] = pop;
    instructions[4] = top;
    return instructions;
}