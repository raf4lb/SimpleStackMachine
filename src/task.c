#include <stdint.h>
#include <stdbool.h>
#include "task.h"
#include "sys.h"
#include "io.h"

Task *task_create(uint8_t id, uint16_t ip, uint16_t stack_size, uint16_t callstack_size, uint16_t localstack_size, void (*message_handler)(Message *message))
{
    Task *task = (Task *)vmmalloc(sizeof(Task));
    if (task == NULL)
    {
        vmprintf("Memory allocation for task failed\n");
        exit(EXIT_FAILURE);
    }
    task->id = id;
    task->active = false;
    task->ip = ip;
    task->stack = stack_create(stack_size);
    task->callstack = stack_create(callstack_size);
    task->localstack = stack_create(localstack_size);
    task->message_handler = message_handler;
    return task;
}

void task_free(Task *task)
{
    stack_free(task->stack);
    stack_free(task->callstack);
    stack_free(task->localstack);
    vmfree(task);
}

void task_process_inbox(Task *task)
{
    MessageQueue *inbox_queue = task->inbox;

    while (inbox_queue->count > 0)
    {
        Message *message = inbox_queue->head;

        task->message_handler(message);

        inbox_queue->head = message->next;
        inbox_queue->count--;
        message_free(message);
    }
}