#include "cpu.h"
#include "stack.h"
#include "io.h"
#include "sys.h"

CPU *cpu_create(uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, InstructionPtr *instructions, uint8_t port_bank)
{
    CPU *cpu = (CPU *)vmmalloc(sizeof(CPU));
    if (cpu == NULL)
    {
        vmprintf("Memory allocation to CPU failed");
        exit(1);
    }
    if (memory_size > 0)
    {
        cpu->memory = memory_create(memory_size);
    }
    cpu->stack = stack_create(stack_size);
    cpu->callstack = stack_create(callstack_size);
    cpu->port_bank = port_bank_create(port_bank);
    cpu->instructions = instructions;
    cpu->ip = 0;
    cpu->user_memory = 0;
    cpu->data_memory = 0;
    return cpu;
}

void cpu_free(CPU *cpu)
{
    stack_free(cpu->stack);
    memory_free(cpu->memory);
    port_bank_free(cpu->port_bank);
    vmfree(cpu);
}

uint8_t cpu_fetch_8b(CPU *cpu)
{

    if (0 <= cpu->ip && cpu->ip < cpu->program_size)
    {
        return cpu->program[cpu->ip++];
    }
    vmprintf("cpu_fetch_8b: out of program memory\n");
    cpu_print(cpu);
    exit(EXIT_FAILURE);
}

uint16_t cpu_fetch_16b(CPU *cpu)
{
    uint16_t data = cpu_fetch_8b(cpu);
    return data << 8 | cpu_fetch_8b(cpu);
}

void cpu_fetch_data(CPU *cpu, void *value, uint16_t size)
{
    if (0 <= cpu->ip && cpu->ip < cpu->program_size)
    {
        memcpy(value, cpu->program + cpu->ip, size);
        cpu->ip += size;
    }
    else
    {
        vmprintf("cpu_fetch_data: out of program memory\n");
    }
}

void cpu_execute(CPU *cpu, uint8_t opcode)
{
    // vmprintf("\n");
    cpu->instructions[opcode](cpu);
    // vmprintf("inst %d\n", opcode);
    // stack_print(cpu->stack);
    // stack_print(cpu->callstack);
    // cpu_print(cpu);
}

void cpu_load_program(CPU *cpu, uint8_t *program, uint16_t program_size, uint16_t data_address)
{
    cpu->program = program;
    cpu->program_size = program_size;
    cpu->ip = 0;
    cpu->user_memory = 0;
    cpu->data_memory = data_address;
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
    for (int i = cpu->user_memory; i < cpu->memory->size; i++)
    {
        if (i > cpu->user_memory)
        {
            vmprintf(", ");
        }
        vmprintf("%u", cpu->memory->data[i]);
    }
    vmprintf("]\n");
}

void cpu_print(CPU *cpu)
{
    vmprintf("Instruction Pointer: %d\n", cpu->ip);
    vmprintf("Stack:\n");
    stack_print(cpu->stack);
    vmprintf("CallStack:\n");
    stack_print(cpu->callstack);
}