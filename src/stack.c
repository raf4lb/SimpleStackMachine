#include "stack.h"
#include <stdlib.h>
#include "io.h"

void stack_error_print(char *message)
{
    vmprintf("StackError: %s\n", message);
}

Stack *stack_create(uint16_t size)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        stack_error_print("Memory allocation failed for Stack\n");
        exit(EXIT_FAILURE);
    }
    stack->size = size;
    uint8_t *data = (uint8_t *)malloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        stack_error_print("Memory allocation failed for Stack->data\n");
        exit(EXIT_FAILURE);
    }
    // Initialize stack
    int i;
    for (i = 0; i < size; i++)
    {
        data[i] = 0;
    }
    stack->data = data;
    stack->sp = 0;
    return stack;
}

void stack_free(Stack *stack)
{
    free(stack->data);
    free(stack);
}

uint8_t stack_pop(Stack *stack)
{
    if (stack->sp == 0)
    {
        stack_error_print("stackunderflow");
        exit(EXIT_FAILURE);
    }
    stack->sp--;
    return stack->data[stack->sp];
}

void stack_push(Stack *stack, uint8_t value)
{
    if (stack->sp == stack->size)
    {
        stack_error_print("stackoverflow");
        exit(EXIT_FAILURE);
    }
    stack->data[stack->sp++] = value;
}

void stack_print(Stack *stack)
{
    vmprintf("[");
    int i;
    for (i = 0; i < stack->size; i++)
    {
        if (i > 0)
        {
            vmprintf(", ");
        }
        if (i == stack->sp)
        {
            vmprintf("->");
        }
        vmprintf("%u", stack->data[i]);
    }
    vmprintf("]\n");
}

void stack_push_bytes(Stack *stack, uint8_t *source, uint16_t data_size)
{
    int i;
    for (i = 0; i < data_size; i++)
    {
        stack_push(stack, source[i]);
    }
}

void stack_pop_bytes(Stack *stack, uint8_t *destination, uint16_t data_size)
{
    int i;
    for (i = 0; i < data_size; i++)
    {
        destination[i] = stack_pop(stack);
    }
}

void stack_push_uint16_t(Stack *stack, uint16_t value)
{
    stack_push(stack, value >> 8);
    stack_push(stack, value & 255);
}

uint16_t stack_pop_uint16_t(Stack *stack)
{
    uint8_t buffer[2];
    stack_pop_bytes(stack, buffer, 2); // pop 2 bytes
    uint16_t data = buffer[1] << 8 | buffer[0];
    return data;
}