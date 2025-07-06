#include <stdlib.h>
#include "stack.h"
#include "sys.h"
#include "io.h"

void stack_error_print(char *message)
{
    vmprintf("StackError: %s\n", message);
}

Stack *stack_create(uint16_t size)
{
    Stack *stack = (Stack *)vmmalloc(sizeof(Stack));
    if (stack == NULL)
    {
        stack_error_print("mem_alloc_failed for Stack\n");
        exit(EXIT_FAILURE);
    }
    stack->size = size;
    uint8_t *data = (uint8_t *)vmmalloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        stack_error_print("mem_alloc_failed for Stack->data\n");
        exit(EXIT_FAILURE);
    }
    // Initialize stack
    for (int i = 0; i < size; i++)
    {
        data[i] = 0;
    }
    stack->data = data;
    stack->sp = 0;
    return stack;
}

void stack_free(Stack *stack)
{
    vmfree(stack->data, sizeof(uint8_t) * stack->size);
    vmfree(stack, sizeof(Stack));
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
    vmprintf("SP: %d\n", stack->sp);
    vmprintf("[");
    for (int i = 0; i < stack->size; i++)
    {
        if (i > 0)
        {
            vmprintf(", ");
        }
        if (i == stack->sp)
        {
            vmprintf("->");
        }
        vmprintf("0x%02X", stack->data[i]);
    }
    vmprintf("]\n");
}

void stack_pop_bytes(Stack *stack, uint8_t *destination, uint16_t data_size)
{
    for (int i = 0; i < data_size; i++)
    {
        destination[i] = stack_pop(stack);
    }
}

void stack_push_16b(Stack *stack, uint16_t value)
{
    stack_push(stack, value >> 8);
    stack_push(stack, value & 255);
}

uint16_t stack_pop_16b(Stack *stack)
{
    uint8_t buffer[2];
    stack_pop_bytes(stack, buffer, 2); // pop 2 bytes
    uint16_t data = buffer[1] << 8 | buffer[0];
    return data;
}

void stack_push_data(Stack *stack, void *value, uint16_t size)
{
    stack_push_bend_data(stack, value, size);
}

void stack_push_bend_data(Stack *stack, void *value, uint16_t size)
{
    stack_write_bend_data_at(stack, value, size, stack->sp);
}

void stack_write_bend_data_at(Stack *stack, void *value, uint16_t size, uint16_t address)
{
    uint16_t final_address = address + size;
    if (final_address > stack->size)
    {
        vmprintf("stack_write_bend_data_at: stackoverflow\n");
        stack_print(stack);
        exit(EXIT_FAILURE);
    }

    for (uint16_t i = 0; i < size; i++)
    {
        stack->data[address + i] = *((uint8_t *)value + size - 1 - i);
    }

    if (final_address > stack->sp)
        stack->sp = final_address;
}

void stack_pop_data(Stack *stack, void *value, uint16_t size)
{
    stack_pop_bend_data(stack, value, size);
}

void stack_pop_bend_data(Stack *stack, void *value, uint16_t size)
{
    if (size > stack->sp)
    {
        vmprintf("stack_pop_bend_data: stack underflow\n");
        stack_print(stack);
        exit(EXIT_FAILURE);
    }

    stack->sp -= size;

    // Adjust pointer to start from the end of the extracted data
    uint8_t *data_ptr = stack->data + stack->sp + size - 1;

    // Iterate over the bytes in reverse order and copy them to the value buffer
    for (int i = 0; i < size; i++)
    {
        ((uint8_t *)value)[i] = *data_ptr;
        data_ptr--;
    }
}

void stack_read_data_at(Stack *stack, void *value, uint16_t size, uint16_t address)
{
    stack_read_bend_data_at(stack, value, size, address);
}

void stack_read_bend_data_at(Stack *stack, void *value, uint16_t size, uint16_t address)
{
    if (size > stack->sp)
    {
        vmprintf("stack_read_bend_data_at: stack underflow\n");
        stack_print(stack);
        exit(EXIT_FAILURE);
    }
    // Copia os bytes da stack para o buffer value, invertendo a ordem dos bytes
    for (int i = 0; i < size; i++)
    {
        ((uint8_t *)value)[i] = stack->data[address + (size - i - 1)];
    }
}

uint16_t stack_read_U16(Stack *stack, uint16_t address)
{
    uint16_t value;
    uint8_t size = sizeof(value);
    stack_read_bend_data_at(stack, &value, size, address);
    return value;
}

int16_t stack_read_I16(Stack *stack, uint16_t address)
{
    int16_t value;
    uint8_t size = sizeof(value);
    stack_read_bend_data_at(stack, &value, size, address);
    return value;
}

float stack_read_F32(Stack *stack, uint16_t address)
{
    float value;
    uint8_t size = sizeof(value);
    stack_read_bend_data_at(stack, &value, size, address);
    return value;
}