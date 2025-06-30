#ifndef CPU_H
#define CPU_H

#include "stack.h"
#include "io.h"
#include <stdint.h>
#include <string.h>
#include "task.h"
#include "tasktree.h"

typedef struct CPU CPU;

typedef void (*InstructionPtr)(CPU *cpu);

struct CPU
{
    Memory *memory;
    TaskTreeNode *task_tree_root;
    TaskTreeNode *task_tree_current_node;
    const uint8_t *program;
    Stack *stack;
    Stack *callstack;
    Stack *localstack;
    const InstructionPtr *instructions;
    PortBank *port_bank;
    MessageQueue *message_queues;

    uint16_t program_size;
    uint16_t ip;
    uint16_t user_memory;
    uint16_t data_address;
    uint16_t tasks_number;
};

void *cpu_create(uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, const InstructionPtr *instruction_set, uint8_t port_banks);

void cpu_free(CPU *cpu);

uint8_t cpu_fetch_8b(CPU *cpu);

uint16_t cpu_fetch_16b(CPU *cpu);

void cpu_fetch_data(CPU *cpu, void *value, uint16_t size);

void cpu_execute(CPU *cpu, uint8_t opcode);

void cpu_load_program(CPU *cpu, const uint8_t *program, uint16_t program_size, uint16_t data_address);

void cpu_run(CPU *cpu);

void cpu_print_user_memory(CPU *cpu);

void cpu_print(CPU *cpu);

typedef struct TaskTreeNode TaskTreeNode;

void cpu_context_switch(CPU *cpu, TaskTreeNode *node);
void cpu_set_task_node(CPU *cpu, TaskTreeNode *node);

void cpu_create_task(CPU *cpu, uint16_t address);
void cpu_delete_task(CPU *cpu, TaskTreeNode *node);

void cpu_create_task_inbox(CPU *cpu, Task *task);
void cpu_process_context_inbox(CPU *cpu);
#endif
