#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdbool.h>
#include "stack.h"
#include "messaging.h"

#define MAX_TASKS 16

typedef struct
{
    uint16_t id;
    bool active;
    uint16_t ip;
    Stack *stack;
    Stack *callstack;
    Stack *localstack;
    MessageQueue *inbox;
    void (*message_handler)(Message *message);
    // uint16_t lsbp;
} Task;

Task *task_create(uint8_t id, uint16_t ip, uint16_t stack_size, uint16_t callstack_size, uint16_t localstack_size, void (*message_handler)(Message *message));

void task_free(Task *task);

void task_process_inbox(Task *task);

#endif