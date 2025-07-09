#ifndef CPU_H
#define CPU_H
#include "stack.h"
#include "io.h"
#include "task.h"
#include "tasktree.h"
#include "server.h"

#define TASK_OPSTACK_SIZE 16
#define TASK_STACK_SIZE 16
#define TASK_LOCALSTACK_SIZE 16
#define CONTEXT_MAX_CYCLES 255

typedef struct CPU CPU;

struct CPU
{
    TaskTreeNode *task_tree_root;
    TaskTreeNode *task_tree_current_node;
    const uint8_t *program;
    Stack *opstack;
    Stack *stack;
    Stack *localstack;
    PortBank *port_bank;
    MessageQueue *message_queues;
    Server *server;

    uint16_t program_size;
    uint16_t ip;
    uint16_t data_address;
    uint16_t tasks_number;
};

void *cpu_create(uint8_t port_banks);

void cpu_free(CPU *cpu);

uint8_t cpu_fetch_8b(CPU *cpu);

uint16_t cpu_fetch_16b(CPU *cpu);

void cpu_fetch_data(CPU *cpu, void *value, uint16_t size);

void cpu_execute(CPU *cpu, uint8_t opcode);

void cpu_load_program(CPU *cpu, const uint8_t *program, uint16_t program_size, uint16_t data_address);

void cpu_run(CPU *cpu);

void cpu_print(CPU *cpu);

typedef struct TaskTreeNode TaskTreeNode;

void cpu_context_switch(CPU *cpu, TaskTreeNode *node);
void cpu_set_task_node(CPU *cpu, TaskTreeNode *node);

void cpu_create_task(CPU *cpu, uint16_t address);
void cpu_delete_task(CPU *cpu, TaskTreeNode *node);

void cpu_create_task_inbox(CPU *cpu, Task *task);
void cpu_process_context_inbox(CPU *cpu);
void cpu_check_incoming_messages(CPU *cpu);
#endif
