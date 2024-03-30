#ifndef STACK_H
#define STACK_H

#include "memory.h"

typedef struct
{
    uint16_t *data;
    int size;
    int sp;
} Stack;

Stack *stack_create(int size);

void stack_free(Stack *stack);

uint16_t stack_pop(Stack *stack);

void stack_push(Stack *stack, uint16_t value);

void stack_print(Stack *stack);

#endif