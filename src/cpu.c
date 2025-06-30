#include "cpu.h"
#include "stack.h"
#include "sys.h"
#include "task.h"
#include "delay.h"
#include "tasktree.h"
#include "string.h"

uint16_t TASK_STACK_SIZE = 16;
uint16_t TASK_CALLSTACK_SIZE = 16;
uint16_t TASK_LOCALSTACK_SIZE = 16;
uint16_t CONTEXT_MAX_CYCLES = 200;

void *cpu_create(uint16_t memory_size, uint16_t stack_size, uint16_t callstack_size, const InstructionPtr *instructions, uint8_t port_bank)
{
    CPU *cpu = (CPU *)vmmalloc(sizeof(CPU));
    cpu->memory = memory_create(memory_size);
    cpu->port_bank = port_bank_create(port_bank);
    cpu->instructions = instructions;
    cpu->ip = 0;
    cpu->user_memory = 0;
    cpu->data_address = 0;
    cpu->tasks_number = 0;
    cpu->message_queues = NULL;
    return cpu;
}

void cpu_create_task(CPU *cpu, uint16_t address)
{
    if (cpu->tasks_number == MAX_TASKS)
    {
        vmprintf("reached max tasks\n");
        return;
    }

    cpu->tasks_number++;
    uint8_t id = cpu->tasks_number;

    uint16_t message_handler_address;
    stack_pop_data(cpu->stack, &message_handler_address, sizeof(uint16_t));

    Task *task = task_create(id, address, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE, TASK_LOCALSTACK_SIZE, message_handler_address);
    task_tree_add_child(cpu->task_tree_current_node, task);
    cpu_create_task_inbox(cpu, task);
}

void cpu_create_task_inbox(CPU *cpu, Task *task)
{
    MessageQueue *new_queue = message_queue_create(task->id);
    task->inbox = new_queue;
    MessageQueue *queue = cpu->message_queues;
    while (queue->next != NULL)
    {
        queue = queue->next;
    }
    queue->next = new_queue;
}

void cpu_delete_task(CPU *cpu, TaskTreeNode *node)
{
    task_tree_remove_child(node);
}

void cpu_load_program(CPU *cpu, const uint8_t *program, uint16_t program_size, uint16_t data_address)
{
    cpu->program = program;
    cpu->program_size = program_size;
    cpu->data_address = data_address;
    Task *main_task = task_create(0, 0, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE, TASK_LOCALSTACK_SIZE, 0);
    cpu->task_tree_root = task_tree_create_node(main_task);
    cpu->task_tree_current_node = cpu->task_tree_root;
    MessageQueue *queue = message_queue_create(0);
    main_task->inbox = queue;
    cpu->message_queues = queue;
}

void cpu_free(CPU *cpu)
{
    memory_free(cpu->memory);
    port_bank_free(cpu->port_bank);
    task_tree_free(cpu->task_tree_root);
    vmfree(cpu, sizeof(CPU));
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
    cpu->instructions[opcode](cpu);
}

void cpu_set_task_node(CPU *cpu, TaskTreeNode *node)
{
    node->task->active = true;
    cpu->ip = node->task->ip;
    cpu->stack = node->task->stack;
    cpu->callstack = node->task->callstack;
    cpu->localstack = node->task->localstack;
    cpu->task_tree_current_node = node;
}

void cpu_context_switch(CPU *cpu, TaskTreeNode *node)
{
    if (cpu->task_tree_current_node->task != NULL)
    {
        cpu->task_tree_current_node->task->active = false;
        cpu->task_tree_current_node->task->ip = cpu->ip;
    }
    cpu_set_task_node(cpu, node);
}

void cpu_run_cycle(CPU *cpu, TaskTreeNode *node)
{
    cpu_context_switch(cpu, node);
    cpu_process_context_inbox(cpu);
    uint8_t cycles = CONTEXT_MAX_CYCLES;
    while (cycles > 0)
    {
        uint8_t opcode = cpu_fetch_8b(cpu);
        cpu_execute(cpu, opcode);
        if (opcode == 57)
        {
            break; // async return
        }
        cycles--;
    }
}

void cpu_process_context_inbox(CPU *cpu)
{
    MessageQueue *inbox_queue = cpu->task_tree_current_node->task->inbox;

    while (inbox_queue->count > 0)
    {
        Message *message = inbox_queue->head;
        if (cpu->task_tree_current_node->task->message_handler_address > 0)
        {
            stack_push_data(cpu->callstack, &cpu->ip, sizeof(uint16_t));
            cpu->ip = cpu->task_tree_current_node->task->message_handler_address;
        }
        inbox_queue->head = message->next;
        inbox_queue->count--;
        message_free(message);
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
    vmprintf("LocalStack:\n");
    stack_print(cpu->localstack);
}