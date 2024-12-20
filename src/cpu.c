#include "cpu.h"
#include "stack.h"
#include "io.h"
#include "sys.h"
#include "task.h"
#include "delay.h"
#include "tasktree.h"

uint16_t MAX_TASKS = 1;
uint16_t TASK_STACK_SIZE = 32;
uint16_t TASK_CALLSTACK_SIZE = 32;
uint16_t CONTEXT_MAX_CYCLES = 20;

void cpu_init(CPU *cpu, uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, InstructionPtr *instructions, uint8_t port_bank)
{
    if (memory_size > 0)
    {
        cpu->memory = memory_create(memory_size);
    }
    cpu->port_bank = port_bank_create(port_bank);
    cpu->instructions = instructions;
    cpu->ip = 0;
    cpu->user_memory = 0;
    cpu->data_address = 0;
}

void cpu_create_task(CPU *cpu, uint16_t address)
{
    uint8_t id = 0;
    Task *task = task_create(id, address, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE);
    task_tree_add_child(cpu->task_tree_current_node, task);
}

void cpu_delete_task(CPU *cpu, TaskTreeNode *node)
{
    vmprintf("deleting task\n");
    task_tree_remove_child(node);
}

void cpu_load_program(CPU *cpu, uint8_t *program, uint16_t program_size, uint16_t data_address)
{
    cpu->program = program;
    cpu->program_size = program_size;
    cpu->data_address = data_address;
    Task *main_task = task_create(0, 0, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE);
    cpu->current_task = main_task;
    cpu->task_tree_root = task_tree_create_node(main_task);
}

void cpu_free(CPU *cpu)
{
    stack_free(cpu->stack);
    stack_free(cpu->callstack);
    memory_free(cpu->memory);
    port_bank_free(cpu->port_bank);
    task_tree_free(cpu->task_tree_root);
    free(cpu);
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
    // vmprintf("inst %d\n", opcode);
    cpu->instructions[opcode](cpu);
    // cpu_print(cpu);
}

void cpu_set_new_context(CPU *cpu, TaskTreeNode *node)
{
    node->task->active = true;
    cpu->ip = node->task->ip;
    cpu->stack = node->task->stack;
    cpu->callstack = node->task->callstack;
    cpu->current_task = node->task;
    cpu->task_tree_current_node = node;
}

void cpu_context_switch(CPU *cpu, TaskTreeNode *node)
{
    cpu->current_task->active = false;
    cpu->current_task->ip = cpu->ip;
    cpu_set_new_context(cpu, node);
}

void cpu_run_cycle(CPU *cpu, TaskTreeNode *node)
{
    cpu_context_switch(cpu, node);
    uint8_t cycles = CONTEXT_MAX_CYCLES;
    while (cycles > 0)
    {
        uint8_t opcode = cpu_fetch_8b(cpu);
        cpu_execute(cpu, opcode);
        if (opcode == 57)
        {
            break;
        }
        cycles--;
    }
}

void cpu_run(CPU *cpu)
{

    while (true)
    {
        task_tree_traverse_dfs(cpu, cpu->task_tree_root, cpu_run_cycle);
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