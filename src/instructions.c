#include <stdlib.h>
#include "instructions.h"
#include "delay.h"
#include "builtin.h"
#include "io.h"

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
        value = (uint16_t)port_bank_get_address(cpu->port_bank, address);
    }
    else
    {
        address = address + cpu->user_memory - cpu->port_bank->size;
        value = (uint16_t)memory_get_address_16b(cpu->memory, address);
    }
    // TODO: Pop value according the size of type stored
    Object *object = Object_create(OBJECT_TYPE_I16, value);
    ObjectStack_push(cpu->stack, object);
}

void push_literal(CPU *cpu)
{
    uint16_t value = cpu_fetch_16b(cpu);
    // TODO: Push value according the size of type stored
    Object *object = Object_create(OBJECT_TYPE_I16, value);
    ObjectStack_push(cpu->stack, object);
}

void pop(CPU *cpu)
{
    // TODO: Pop value according the size of type stored
    Object *object = ObjectStack_pop(cpu->stack);
    Object_free(object);
}

void pop_address(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    Object *object = ObjectStack_pop(cpu->stack);
    uint16_t value = object->value.from.i16;
    if (address < cpu->port_bank->size)
    {
        port_bank_set_address(cpu->port_bank, address, (uint8_t)value);
    }
    else
    {
        address = cpu->user_memory + address - cpu->port_bank->size;
        memory_set_address_16b(cpu->memory, address, value);
    }
    Object_free(object);
}

void top(CPU *cpu)
{
    Object *object = cpu->stack->data[cpu->stack->sp - 1];
    switch (object->type)
    {
    case OBJECT_TYPE_I16:
        vmprintf("%d\n", object->value.from.i16);
        break;

    default:
        vmprintf("TopInstructionError: Unknown object type\n");
        break;
    }
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
    // Here result is a bool so it should be okay
    Object *object = ObjectStack_pop(cpu->stack);
    uint8_t result = (uint8_t)object->value.from.i16;
    Object_free(object);
    if (!result)
        cpu->ip = address;
}

void compare_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_equal(a, b));
}

void compare_less(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_less(a, b));
}

void compare_greater(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_greater(a, b));
}

void compare_less_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_less_equal(a, b));
}

void compare_greater_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_greater_equal(a, b));
}

void add(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    Object *c = Object_add(a, b);
    ObjectStack_push(cpu->stack, c);
}

void subtract(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_sub(a, b));
}

void multiply(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_mul(a, b));
}

void divide(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_div(a, b));
}

void bitwise_and(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_and(a, b));
}

void bitwise_or(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_or(a, b));
}

void bitwise_xor(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_xor(a, b));
}

void bitwise_not(CPU *cpu)
{
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_not(a));
}

void bitwise_left_shift(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_left_shift(a, b));
}

void bitwise_right_shift(CPU *cpu)
{
    Object *b = ObjectStack_pop(cpu->stack);
    Object *a = ObjectStack_pop(cpu->stack);
    ObjectStack_push(cpu->stack, Object_bitwise_right_shift(a, b));
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

void (**instructions_create())()
{
    void (**instructions)() = malloc(INSTRUCTIONS * sizeof(void (*)()));
    if (instructions == NULL)
    {
        vmprintf("Memory allocation failed for instructions");
        return NULL;
    }
    instructions[0] = halt;
    instructions[1] = push_literal;
    instructions[2] = push;
    instructions[3] = pop;
    instructions[4] = pop_address;
    instructions[5] = top;
    instructions[6] = delay;
    instructions[7] = jump;
    instructions[8] = pop_jump_if_false;
    instructions[9] = compare_equal;
    instructions[10] = compare_less;
    instructions[11] = compare_greater;
    instructions[12] = compare_less_equal;
    instructions[13] = compare_greater_equal;
    instructions[14] = add;
    instructions[15] = subtract;
    instructions[16] = multiply;
    instructions[17] = divide;
    instructions[18] = bitwise_and;
    instructions[19] = bitwise_or;
    instructions[20] = bitwise_xor;
    instructions[21] = bitwise_not;
    instructions[22] = bitwise_left_shift;
    instructions[23] = bitwise_right_shift;
    instructions[24] = call;
    instructions[25] = ret;
    instructions[26] = syscall;

    return instructions;
}