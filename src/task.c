#include <stdint.h>
#include <stdbool.h>
#include "task.h"
#include "sys.h"
#include "io.h"

Task *task_create(uint8_t id, uint16_t ip, uint16_t stack_size, uint16_t callstack_size)
{
    Task *task = (Task *)vmmalloc(sizeof(Task));
    if (task == NULL)
    {
        vmprintf("Memory allocation for task failed");
        exit(1);
    }
    task->id = id;
    task->active = false;
    task->ip = ip;
    task->stack = stack_create(stack_size);
    task->callstack = stack_create(callstack_size);
    return task;
}

void task_free(Task *task)
{
    stack_free(task->stack);
    stack_free(task->callstack);
    vmfree(task);
}