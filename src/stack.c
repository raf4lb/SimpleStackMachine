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
    stack->bp = 0;
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
    vmprintf("BP: %d\n", stack->bp);
    vmprintf("[");
    for (int i = 0; i < stack->size; i++)
    {
        if (i > 0)
        {
            vmprintf(", ");
        }
        if (i == stack->sp)
        {
            vmprintf("SP->");
        }
        if (i == stack->bp)
        {
            vmprintf("BP->");
        }
        vmprintf("0x%02X", stack->data[i]);
    }
    vmprintf("]\n");
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

void stack_push_message(Stack *stack, Message *message)
{
    stack_push_data(stack, &message->task_dst, sizeof(message->task_dst));
    stack_push_data(stack, &message->task_src, sizeof(message->task_src));
    stack_push_data(stack, &message->vm_src, sizeof(message->vm_src));
    stack_push_data(stack, &message->vm_dst, sizeof(message->vm_dst));
    stack_push_data(stack, &message->payload_size, sizeof(message->payload_size));
    memcpy(stack->data + stack->sp, message->payload, message->payload_size);
    stack->sp += message->payload_size;
}
