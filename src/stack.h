#ifndef STACK_H
#define STACK_H

#include "memory.h"
#include <string.h>

typedef struct
{
    uint8_t *data;
    uint16_t size;
    uint16_t sp;
} Stack;

Stack *stack_create(uint16_t size);

void stack_free(Stack *stack);

uint8_t stack_pop(Stack *stack);

void stack_push(Stack *stack, uint8_t value);

void stack_print(Stack *stack);

void stack_push_bytes(Stack *stack, uint8_t *source, uint16_t data_size);

void stack_pop_bytes(Stack *stack, uint8_t *destination, uint16_t data_size);

void stack_push_16b(Stack *stack, uint16_t value);

void stack_push_32b(Stack *stack, uint32_t value);

uint16_t stack_pop_16b(Stack *stack);

uint32_t stack_pop_32b(Stack *stack);

uint64_t stack_read_bytes(Stack *stack, uint16_t size);

void stack_push_data(Stack *stack, void *value, uint16_t size);

void stack_push_lend_data(Stack *stack, void *value, uint16_t size);

void stack_push_bend_data(Stack *stack, void *value, uint16_t size);

void stack_pop_data(Stack *stack, void *value, uint16_t size);

void stack_pop_bend_data(Stack *stack, void *value, uint16_t size);

void stack_read_data(Stack *stack, uint16_t address, void *value, uint16_t size);

void stack_read_bend_data(Stack *stack, uint16_t address, void *value, uint16_t size);

void stack_read_lend_data(Stack *stack, void *value, uint16_t size);

uint16_t stack_read_U16(Stack *stack, uint16_t address);

int16_t stack_read_I16(Stack *stack, uint16_t address);

float stack_read_F32(Stack *stack, uint16_t address);

void stack_write_bend_data(Stack *stack, uint16_t address, void *value, uint16_t size);

#endif