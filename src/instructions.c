#include "sys.h"
#include "instructions.h"
#include "delay.h"
#include "builtin.h"

void halt(CPU *cpu)
{
    vmprintf("Exit\n");
    cpu_free(cpu);
    exit(0);
}

void push(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    uint16_t value;
    if (address < cpu->port_bank->size)
    {
        value = port_bank_get_address(cpu->port_bank, address);
    }
    else
    {
        address = address + cpu->user_memory - cpu->port_bank->size;
        value = memory_get_address_16b(cpu->memory, address);
    }
    // TODO: Pop value according the size of type stored
    stack_push_uint16_t(cpu->stack, value);
}

void push_literal(CPU *cpu)
{
    uint16_t value = cpu_fetch_16b(cpu);
    // TODO: Push value according the size of type stored
    stack_push_uint16_t(cpu->stack, value);
}

void pop(CPU *cpu)
{
    // TODO: Pop value according the size of type stored
    stack_pop_uint16_t(cpu->stack);
}

void pop_address(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    // TODO: Pop value according the size of type stored
    uint16_t value = stack_pop_uint16_t(cpu->stack);
    if (address < cpu->port_bank->size)
    {
        port_bank_set_address(cpu->port_bank, address, value);
    }
    else
    {
        address = cpu->user_memory + address - cpu->port_bank->size;
        memory_set_address_16b(cpu->memory, address, value);
    }
}

void top(CPU *cpu)
{
    uint8_t h = cpu->stack->data[cpu->stack->sp - 2];
    uint8_t l = cpu->stack->data[cpu->stack->sp - 1];
    uint16_t top = h << 8 | l;
    vmprintf("%d\n", top);
}

void delay(CPU *cpu)
{
    uint16_t milliseconds = cpu_fetch_16b(cpu);
    delay_ms(milliseconds);
}

void jump(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    cpu->ip = address;
}

void pop_jump_if_false(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    // TODO: Pop value according the size of type stored
    // Here result is a bool so it should be okay
    uint16_t result = stack_pop_uint16_t(cpu->stack);
    if (!result)
        cpu->ip = address;
}

void compare_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a == b);
}

void compare_less(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a < b);
}

void compare_greater(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a > b);
}

void compare_less_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a <= b);
}

void compare_greater_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a >= b);
}

void add(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a + b);
}

void subtract(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a - b);
}

void multiply(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a * b);
}

void divide(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a / b);
}

void bitwise_and(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a & b);
}

void bitwise_or(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a | b);
}

void bitwise_xor(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a ^ b);
}

void bitwise_not(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, ~a);
}

void bitwise_left_shift(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a << b);
}

void bitwise_right_shift(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_uint16_t(cpu->stack);
    uint16_t a = stack_pop_uint16_t(cpu->stack);
    stack_push_uint16_t(cpu->stack, a >> b);
}

void call(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    // TODO: Push 16bit size (address size)
    stack_push_uint16_t(cpu->callstack, cpu->ip);
    cpu->ip = address;
}

void ret(CPU *cpu)
{
    // TODO: Pop 16bit size (address size)
    uint16_t address = stack_pop_uint16_t(cpu->callstack);
    cpu->ip = address;
}

void syscall(CPU *cpu)
{
    uint8_t func = cpu_fetch_16b(cpu);
    switch (func)
    {
    case 0:
        print(cpu);
        break;

    default:
        break;
    }
}
