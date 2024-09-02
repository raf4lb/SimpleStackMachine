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
        stack_error_print("Memory allocation failed for Stack\n");
        exit(EXIT_FAILURE);
    }
    stack->size = size;
    uint8_t *data = (uint8_t *)vmmalloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        stack_error_print("Memory allocation failed for Stack->data\n");
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
    vmfree(stack->data);
    vmfree(stack);
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
    vmprintf("Stack Pointer: %d\n", stack->sp);
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

void stack_push_bytes(Stack *stack, uint8_t *source, uint16_t data_size)
{
    for (int i = 0; i < data_size; i++)
    {
        stack_push(stack, source[i]);
    }
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

void stack_push_32b(Stack *stack, uint32_t value)
{
    stack_push(stack, (uint8_t)((value >> 24) & 0xFF));
    stack_push(stack, (uint8_t)((value >> 16) & 0xFF));
    stack_push(stack, (uint8_t)((value >> 8) & 0xFF));
    stack_push(stack, (uint8_t)(value & 0xFF));
}

uint16_t stack_pop_16b(Stack *stack)
{
    uint8_t buffer[2];
    stack_pop_bytes(stack, buffer, 2); // pop 2 bytes
    uint16_t data = buffer[1] << 8 | buffer[0];
    return data;
}

uint32_t stack_pop_32b(Stack *stack)
{
    uint8_t buffer[4];
    stack_pop_bytes(stack, buffer, 4); // pop 4 bytes
    uint32_t data = ((uint32_t)buffer[3] << 24) | ((uint32_t)buffer[2] << 16) | ((uint32_t)buffer[1] << 8) | buffer[0];
    return data;
}

uint64_t stack_read_bytes(Stack *stack, uint16_t size)
{
    uint64_t result = 0;
    // Ensure that size is within valid range (1, 2, 4, or 8)
    if (size != 1 && size != 2 && size != 4 && size != 8)
    {
        vmprintf("stack_read_bytes: invalid size");
        exit(EXIT_FAILURE);
    }
    for (uint16_t i = 0; i < size; i++)
    {
        result = (result << 8) | stack->data[stack->sp - size + i];
    }
    return result;
}

void stack_push_data(Stack *stack, void *value, uint16_t size)
{
    stack_push_lend_data(stack, value, size);
}

void stack_push_lend_data(Stack *stack, void *value, uint16_t size)
{
    if (stack->sp + size <= stack->size)
    {
        memcpy(stack->data + stack->sp, value, size);
        stack->sp += size;
    }
    else
    {
        vmprintf("stack_push_data: stack overflow\n");
    }
}

void stack_push_bend_data(Stack *stack, void *value, uint16_t size)
{
    if (stack->sp + size <= stack->size)
    {
        uint8_t temp[size];
        for (uint16_t i = 0; i < size; i++)
        {
            temp[i] = *((uint8_t *)value + size - 1 - i);
        }
        memcpy(stack->data + stack->sp, temp, size);
        stack->sp += size;
    }
    else
    {
        vmprintf("stack_push_data: stack overflow\n");
    }
}

void stack_pop_data(Stack *stack, void *value, uint16_t size)
{
    if (stack->sp >= size)
    {
        stack->sp -= size;
        memcpy(value, stack->data + stack->sp, size);
    }
    else
    {
        vmprintf("stack_pop_data: stack underflow\n");
    }
}

void stack_pop_bend_data(Stack *stack, void *value, uint16_t size)
{
    if (stack->sp >= size)
    {
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
    else
    {
        vmprintf("stack_pop_bend_data: stack underflow\n");
    }
}

void stack_read_data(Stack *stack, uint16_t address, void *value, uint16_t size)
{
    stack_read_bend_data(stack, address, value, size);
}

void stack_read_bend_data(Stack *stack, uint16_t address, void *value, uint16_t size)
{
    if (stack->sp >= size)
    {
        // Copia os bytes da stack para o buffer value, invertendo a ordem dos bytes
        for (int i = 0; i < size; i++)
        {
            ((uint8_t *)value)[i] = stack->data[address + (size - i - 1)];
        }
    }
    else
    {
        vmprintf("stack_read_data: stack underflow\n");
    }
}

uint16_t stack_read_U16(Stack *stack, uint16_t address)
{
    uint16_t value;
    uint8_t size = sizeof(value);
    stack_read_bend_data(stack, address, &value, size);
    return value;
}

int16_t stack_read_I16(Stack *stack, uint16_t address)
{
    int16_t value;
    uint8_t size = sizeof(value);
    stack_read_bend_data(stack, address, &value, size);
    return value;
}

float stack_read_F32(Stack *stack, uint16_t address)
{
    float value;
    uint8_t size = sizeof(value);
    stack_read_bend_data(stack, address, &value, size);
    return value;
}

void stack_read_lend_data(Stack *stack, void *value, uint16_t size)
{
    if (stack->sp >= size)
    {
        memcpy(value, stack->data + stack->sp - size, size);
    }
    else
    {
        vmprintf("stack_read_data: stack underflow\n");
    }
}

void stack_write_bend_data(Stack *stack, uint16_t address, void *value, uint16_t size)
{
    if (address + size < stack->size)
    {
        uint8_t temp[size];
        for (uint16_t i = 0; i < size; i++)
        {
            temp[i] = *((uint8_t *)value + size - 1 - i);
        }
        memcpy(stack->data + address, temp, size);
    }
    else
    {
        vmprintf("stack_write_bend_data: stack out of space\n");
    }
}