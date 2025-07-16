#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "stack.h"
#include "messaging.h"

#define MAX_TASKS 4

typedef struct
{
    Stack *opstack;
    Stack *stack;
    MessageQueue *inbox;

    uint16_t id;
    uint16_t ip;
    uint16_t message_handler_address;

    uint8_t active;
} Task;

Task *task_create(uint8_t id, uint16_t ip, uint16_t opstack_size, uint16_t stack_size, uint16_t message_handler_address);

void task_free(Task *task);

#endif