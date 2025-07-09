#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "task.h"
#include "sys.h"
#include "io.h"

Task *task_create(uint8_t id, uint16_t ip, uint16_t opstack_size, uint16_t stack_size, uint16_t localstack_size, uint16_t message_handler_address)
{
    Task *task = (Task *)vmmalloc(sizeof(Task));
    if (task == NULL)
    {
        vmprintf("mem_alloc_failed: task\n");
        exit(EXIT_FAILURE);
    }
    task->id = id;
    task->active = false;
    task->ip = ip;
    task->opstack = stack_create(opstack_size);
    task->stack = stack_create(stack_size);
    task->localstack = stack_create(localstack_size);
    task->message_handler_address = message_handler_address;
    return task;
}

void task_free(Task *task)
{
    stack_free(task->opstack);
    stack_free(task->stack);
    stack_free(task->localstack);
    message_queue_free(task->inbox);
    vmfree(task, sizeof(Task));
}