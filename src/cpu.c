#include <stdlib.h>
#include "cpu.h"
#include "instructions.h"
#include "stack.h"
#include "sys.h"
#include "task.h"
#include "delay.h"
#include "tasktree.h"

void *cpu_create(uint8_t port_bank)
{
    CPU *cpu = (CPU *)vmmalloc(sizeof(CPU));
    cpu->port_bank = port_bank_create(port_bank);
#ifdef MACOSX
    cpu->server = server_create(SERVER_PORT);
#endif
    cpu->ip = 0;
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
    stack_pop_data(cpu->opstack, &message_handler_address, sizeof(uint16_t));

    Task *task = task_create(id, address, TASK_OPSTACK_SIZE, TASK_STACK_SIZE, message_handler_address);
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
    Task *main_task = task_create(0, 0, TASK_OPSTACK_SIZE, TASK_STACK_SIZE, 0);
    cpu->task_tree_root = task_tree_create_node(main_task);
    cpu->task_tree_current_node = cpu->task_tree_root;
    MessageQueue *queue = message_queue_create(0);
    main_task->inbox = queue;
    cpu->message_queues = queue;
}

void cpu_free(CPU *cpu)
{
    port_bank_free(cpu->port_bank);
    task_tree_free(cpu->task_tree_root);
    vmfree(cpu, sizeof(CPU));
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
    switch (opcode)
    {
    case OP_HALT:
        halt(cpu);
        break;

    // Control flow
    case OP_DELAY:
        delay(cpu);
        break;
    case OP_JUMP:
        jump(cpu);
        break;
    case OP_POP_JUMP_IF_FALSE:
        pop_jump_if_false(cpu);
        break;

    // Comparisons
    case OP_COMPARE_EQ:
        compare_equal(cpu);
        break;
    case OP_COMPARE_LT:
        compare_less(cpu);
        break;
    case OP_COMPARE_GT:
        compare_greater(cpu);
        break;
    case OP_COMPARE_LE:
        compare_less_equal(cpu);
        break;
    case OP_COMPARE_GE:
        compare_greater_equal(cpu);
        break;

    // Calls and syscalls
    case OP_CALL:
        call(cpu);
        break;
    case OP_RETURN:
        ret(cpu);
        break;
    case OP_SYSCALL:
        syscall(cpu);
        break;
    case OP_ASYNC_CALL:
        async_call(cpu);
        break;
    case OP_ASYNC_RETURN:
        async_ret(cpu);
        break;

    // U16 operations
    case OP_PUSHL_U16:
        push_literal_U16(cpu);
        break;
    case OP_POP_U16:
        pop_U16(cpu);
        break;
    case OP_TOP_U16:
        top_U16(cpu);
        break;
    case OP_ADD_U16:
        add_U16(cpu);
        break;
    case OP_SUB_U16:
        subtract_U16(cpu);
        break;
    case OP_MUL_U16:
        multiply_U16(cpu);
        break;
    case OP_DIV_U16:
        divide_U16(cpu);
        break;
    case OP_POPA_U16:
        pop_address_U16(cpu);
        break;

    // U16 opstack/local
    case OP_PUSH_U16:
        push_U16(cpu);
        break;
    case OP_ALLOC_LOCAL:
        allocate_local(cpu);
        break;
    case OP_PUSH_LOCAL:
        push_local(cpu);
        break;
    case OP_POP_LOCAL:
        pop_local(cpu);
        break;

    // I16 operations and memory
    case OP_PUSHL_I16:
        push_literal_I16(cpu);
        break;
    case OP_POP_I16:
        pop_I16(cpu);
        break;
    case OP_TOP_I16:
        top_I16(cpu);
        break;
    case OP_ADD_I16:
        add_I16(cpu);
        break;
    case OP_SUB_I16:
        subtract_I16(cpu);
        break;
    case OP_MUL_I16:
        multiply_I16(cpu);
        break;
    case OP_DIV_I16:
        divide_I16(cpu);
        break;
    case OP_READ_I16:
        load_I16(cpu);
        break;
    case OP_WRITE_I16:
        store_I16(cpu);
        break;
    case OP_ALLOC_I16:
        var_I16(cpu);
        break;
    case OP_FREE_I16:
        del_I16(cpu);
        break;

    // F32 operations
    case OP_PUSHL_F32:
        push_literal_F32(cpu);
        break;
    case OP_POP_F32:
        pop_F32(cpu);
        break;
    case OP_TOP_F32:
        top_F32(cpu);
        break;
    case OP_ADD_F32:
        add_F32(cpu);
        break;
    case OP_SUB_F32:
        subtract_F32(cpu);
        break;
    case OP_MUL_F32:
        multiply_F32(cpu);
        break;
    case OP_DIV_F32:
        divide_F32(cpu);
        break;

    // Time
    case OP_PUSH_MILLIS:
        push_millis(cpu);
        break;

    // Bitwise operations
    case OP_LSHIFT_U16:
        bitwise_left_shift_U16(cpu);
        break;
    case OP_RSHIFT_U16:
        bitwise_right_shift_U16(cpu);
        break;
    case OP_OR_U16:
        bitwise_or_U16(cpu);
        break;
    case OP_XOR_U16:
        bitwise_xor_U16(cpu);
        break;
    case OP_AND_U16:
        bitwise_and_U16(cpu);
        break;
    case OP_NOT_U16:
        bitwise_not_U16(cpu);
        break;

    default:
        vmprintf("unknown opcode %d\n", opcode);
        break;
    }
}

void cpu_set_task_node(CPU *cpu, TaskTreeNode *node)
{
    node->task->active = true;
    cpu->ip = node->task->ip;
    cpu->opstack = node->task->opstack;
    cpu->stack = node->task->stack;
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
    cpu_check_incoming_messages(cpu);
    cpu_context_switch(cpu, node);
    cpu_process_context_inbox(cpu);
    uint8_t cycles = CONTEXT_MAX_CYCLES;
    while (cycles > 0)
    {
        uint8_t opcode;
        cpu_fetch_data(cpu, &opcode, sizeof(opcode));
        cpu_execute(cpu, opcode);
        if (opcode == OP_ASYNC_RETURN)
        {
            break; // async return
        }
        cycles--;
    }
}

void cpu_check_incoming_messages(CPU *cpu)
{
#ifdef MACOSX
    Message *message = server_receive_message(cpu->server);
    if (message)
    {
        // printf("Mensagem recebida:\n");
        // printf("  vm_src: %u\n", msg->vm_src);
        // printf("  vm_dst: %u\n", msg->vm_dst);
        // printf("  task_src: %u\n", msg->task_src);
        // printf("  task_dst: %u\n", msg->task_dst);
        // printf("  seq: %u\n", msg->seq);
        // printf("  payload_size: %u\n", msg->payload_size);
        // printf("  frag_id: %u\n", msg->frag_id);
        // printf("  frag_total: %u\n", msg->frag_total);

        // if (msg->payload && msg->payload_size > 0)
        // {
        //     printf("  payload: ");
        //     fwrite(msg->payload, 1, msg->payload_size, stdout);
        //     printf("\n");
        // }

        deliver_local(cpu->message_queues, message);
    }
#endif
}

void cpu_process_context_inbox(CPU *cpu)
{
    MessageQueue *inbox_queue = cpu->task_tree_current_node->task->inbox;

    while (inbox_queue->count > 0)
    {
        Message *message = inbox_queue->head;
        if (cpu->task_tree_current_node->task->message_handler_address > 0)
        {
            call_address(cpu, cpu->task_tree_current_node->task->message_handler_address);
            stack_push_message(cpu->stack, message);
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

void cpu_print(CPU *cpu)
{
    vmprintf("IP: %d\n", cpu->ip);
    vmprintf("OpStack:\n");
    stack_print(cpu->opstack);
    vmprintf("Stack:\n");
    stack_print(cpu->stack);
}