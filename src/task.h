#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "stack.h"

typedef struct
{
    uint16_t id;
    bool active;
    uint16_t ip;
    Stack *stack;
    Stack *callstack;
} Task;

Task *task_create(uint8_t id, uint16_t ip, uint16_t stack_size, uint16_t callstack_size);

void task_free(Task *task);

#endif