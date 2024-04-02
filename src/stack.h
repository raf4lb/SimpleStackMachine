#ifndef STACK_H
#define STACK_H

#include "memory.h"

typedef struct
{
    uint16_t *data;
    uint16_t size;
    uint16_t sp;
} Stack;

Stack *stack_create(uint16_t size);

void stack_free(Stack *stack);

uint16_t stack_pop(Stack *stack);

void stack_push(Stack *stack, uint16_t value);

void stack_print(Stack *stack);

#endif