#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "stack.h"
#include "messaging.h"

#define MAX_TASKS 4

typedef struct
{
    Stack *stack;
    Stack *callstack;
    Stack *localstack;
    MessageQueue *inbox;

    uint16_t id;
    uint16_t ip;
    uint16_t message_handler_address;

    uint8_t active; // usar uint8_t em vez de bool para controle exato
} Task;

Task *task_create(uint8_t id, uint16_t ip, uint16_t stack_size, uint16_t callstack_size, uint16_t localstack_size, uint16_t message_handler_address);

void task_free(Task *task);

#endif