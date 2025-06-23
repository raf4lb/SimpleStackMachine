#include "cpu.h"
#include "stack.h"
#include "sys.h"
#include "task.h"
#include "delay.h"
#include "tasktree.h"
#include "string.h"

uint16_t TASK_STACK_SIZE = 32;
uint16_t TASK_CALLSTACK_SIZE = 32;
uint16_t TASK_LOCALSTACK_SIZE = 32;
uint16_t CONTEXT_MAX_CYCLES = 20;

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

void default_message_handler(Message *message){
    vmprintf("Task %d received message %s\n", message->task_dst, message->payload);
}

void cpu_create_task(CPU *cpu, uint16_t address)
{   
    cpu->tasks_number++;
    uint8_t id = cpu->tasks_number;
    
    Task *task = task_create(id, address, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE, TASK_LOCALSTACK_SIZE, default_message_handler);
    task_tree_add_child(cpu->task_tree_current_node, task);
    cpu_create_task_inbox(cpu, task);
}

void cpu_create_task_inbox(CPU *cpu, Task *task){
    MessageQueue *new_queue = message_queue_create(task->id);
    task->inbox = new_queue;
    MessageQueue *queue = cpu->message_queues;
    while (queue->next != NULL){
        queue = queue->next;
    }
    queue->next = new_queue;

    Message *test_message = (Message *)vmmalloc(sizeof(Message));
    test_message->frag_id = 0;
    test_message->frag_total = 0;
    test_message->seq = 0;
    test_message->vm_dst = get_local_vm_id();
    test_message->vm_src = get_local_vm_id();
    test_message->task_dst = 1;
    test_message->len = 4;
    test_message->payload = (uint8_t *)vmmalloc(4*sizeof(uint8_t));
    test_message->payload[0] = 'O';
    test_message->payload[1] = 'l';
    test_message->payload[2] = 'a';
    test_message->payload[3] = '\0';
    test_message->crc = compute_crc(test_message);
    send_message(cpu->message_queues, test_message);

    Message *test_message2 = (Message *)vmmalloc(sizeof(Message));
    test_message2->frag_id = 0;
    test_message2->frag_total = 0;
    test_message2->seq = 0;
    test_message2->vm_dst = get_local_vm_id();
    test_message2->vm_src = get_local_vm_id();
    test_message2->task_dst = 1;
    test_message2->len = 6;
    test_message2->payload = (uint8_t *)vmmalloc(6*sizeof(uint8_t));
    test_message2->payload[0] = 'm';
    test_message2->payload[1] = 'u';
    test_message2->payload[2] = 'n';
    test_message2->payload[3] = 'd';
    test_message2->payload[4] = 'o';
    test_message2->payload[5] = '\0';
    test_message2->crc = compute_crc(test_message2);
    send_message(cpu->message_queues, test_message2);
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
    Task *main_task = task_create(0, 0, TASK_STACK_SIZE, TASK_CALLSTACK_SIZE, TASK_LOCALSTACK_SIZE, default_message_handler);
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
    uint8_t cycles = CONTEXT_MAX_CYCLES;
    task_process_inbox(node->task);
    while (cycles > 0)
    {
        uint8_t opcode = cpu_fetch_8b(cpu);
        cpu_execute(cpu, opcode);
        if (opcode == 57)
        {
            break; // asyn return
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
    vmprintf("LocalStack:\n");
    stack_print(cpu->localstack);
}