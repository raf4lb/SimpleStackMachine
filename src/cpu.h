#include "stack.h"
#include "io.h"
#include <stdint.h>

typedef struct CPU CPU;

struct CPU
{
    Memory *memory;
    Stack *stack;
    int ip;
    void (**instructions)(CPU *cpu);
    PortBank *port_bank;
    uint16_t user_memory;
};

CPU *cpu_create(int memory_size, int stack_size, void (**instructions)(CPU *cpu), uint8_t port_banks);

void cpu_free(CPU *cpu);

uint8_t cpu_fetch_8b(CPU *cpu);

uint16_t cpu_fetch_16b(CPU *cpu);

void cpu_execute(CPU *cpu, uint8_t opcode);

void cpu_load_program(CPU *cpu, uint8_t *program, int program_size);

void cpu_run(CPU *cpu);
