#include "stack.h"
#include <stdlib.h>
#include "serial.h"

void stack_error_print(char *message)
{
    serial_printf("StackError: %s\n", message);
}

Stack *stack_create(int size)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL)
    {
        stack_error_print("Memory allocation failed for Stack\n");
        exit(EXIT_FAILURE);
    }
    stack->size = size;
    uint16_t *data = (uint16_t *)malloc(size * sizeof(uint16_t));
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
    return stack;
}

void stack_free(Stack *stack)
{
    free(stack->data);
    free(stack);
}

uint16_t stack_pop(Stack *stack)
{
    if (stack->sp == 0)
    {
        stack_error_print("stackunderflow");
        exit(EXIT_FAILURE);
    }
    stack->sp--;
    return stack->data[stack->sp];
}

void stack_push(Stack *stack, uint16_t value)
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
    serial_printf("[");
    int i;
    for (i = 0; i < stack->size; i++)
    {
        if (i > 0)
        {
            serial_printf(", ");
        }
        if (i == stack->sp)
        {
            serial_printf("->");
        }
        serial_printf("%u", stack->data[i]);
    }
    serial_printf("]\n");
}