#include "stack.h"

typedef struct CPU CPU;

struct CPU
{
    Memory *memory;
    Stack *stack;
    int ip;
    void (**instructions)(CPU *cpu);
};

CPU *cpu_create(int memory_size, int stack_size, void (**instructions)(CPU *cpu));

void cpu_free(CPU *cpu);

uint8_t cpu_fetch_8b(CPU *cpu);

uint16_t cpu_fetch_16b(CPU *cpu);

void cpu_execute(CPU *cpu, uint8_t opcode);

void cpu_load_program(CPU *cpu, uint8_t *program, int program_size);

void cpu_run(CPU *cpu);
