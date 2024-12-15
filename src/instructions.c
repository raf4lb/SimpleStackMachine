#include "sys.h"
#include "instructions.h"
#include "delay.h"
#include "builtin.h"
#include "datatypes.h"

void halt(CPU *cpu)
{
    // stack_print(cpu->callstack);
    // stack_print(cpu->stack);
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
    stack_push_16b(cpu->stack, value);
}

void push_literal(CPU *cpu)
{
    uint8_t data_type;
    cpu_fetch_data(cpu, &data_type, sizeof(data_type));
    uint8_t size = get_data_type_size(data_type);
    if (size == WORD)
    {
        uint16_t value;
        cpu_fetch_data(cpu, &value, sizeof(value));
        stack_push_lend_data(cpu->stack, &value, sizeof(value));
    }
    if (size == DWORD)
    {
        uint32_t value;
        cpu_fetch_data(cpu, &value, sizeof(value));
        if (data_type == TYPE_F32)
        {
            stack_push_bend_data(cpu->stack, &value, sizeof(value));
        }
        else
        {
            stack_push_lend_data(cpu->stack, &value, sizeof(value));
        }
    }
}

void push_literal_U16(CPU *cpu)
{
    uint16_t value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void pop_U16(CPU *cpu)
{
    uint16_t value;
    stack_pop_bend_data(cpu->stack, &value, sizeof(value));
}

void push_U16(CPU *cpu)
{
    // TODO: optimize reading. We do not need to copy date to value variable, we can push it directly from the address.
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    if (address < cpu->port_bank->size)
    {
        value = port_bank_get_address(cpu->port_bank, address);
    }
    else
    {
        value = (uint16_t)cpu->program[cpu->data_memory + address - cpu->port_bank->size];
    }
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void pop_address_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_pop_bend_data(cpu->stack, &value, sizeof(value));
    if (address < cpu->port_bank->size)
    {
        port_bank_set_address(cpu->port_bank, address, (uint8_t)value);
    }
    else
    {
        address = cpu->user_memory + address - cpu->port_bank->size;
        memory_set_address_16b(cpu->memory, address, value);
    }
}

void load_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_read_bend_data(cpu->callstack, address, &value, sizeof(value));
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void store_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_pop_bend_data(cpu->stack, &value, sizeof(value));
    stack_write_bend_data(cpu->callstack, address, &value, sizeof(value));
}

void var_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->callstack->sp += sizeof(uint16_t);
}

void del_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->callstack->sp -= sizeof(uint16_t);
}

void top_U16(CPU *cpu)
{
    uint16_t top = stack_read_U16(cpu->stack, cpu->stack->sp - sizeof(uint16_t));
    vmprintf("%hu\n", top);
}

void add_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    uint16_t result = a + b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void subtract_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    uint16_t result = a - b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void multiply_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    uint16_t result = a * b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void divide_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    uint16_t result = a / b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void push_literal_I16(CPU *cpu)
{
    int16_t value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void pop_I16(CPU *cpu)
{
    cpu->stack->sp -= sizeof(int16_t);
}

void load_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    int16_t value;
    stack_read_bend_data(cpu->callstack, address, &value, sizeof(value));
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void store_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    int16_t value;
    stack_pop_bend_data(cpu->stack, &value, sizeof(value));
    stack_write_bend_data(cpu->callstack, address, &value, sizeof(value));
}

void var_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->callstack->sp += sizeof(int16_t);
}

void del_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->callstack->sp -= sizeof(int16_t);
}

void top_I16(CPU *cpu)
{
    int16_t top = stack_read_I16(cpu->stack, cpu->stack->sp - sizeof(int16_t));
    vmprintf("%hi\n", top);
}

void add_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    int16_t result = a + b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void subtract_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    int16_t result = a - b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void multiply_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    int16_t result = a * b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void divide_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    int16_t result = a / b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void push_literal_F32(CPU *cpu)
{
    float value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->stack, &value, sizeof(value));
}

void pop_F32(CPU *cpu)
{
    float value;
    stack_pop_bend_data(cpu->stack, &value, sizeof(value));
}

void top_F32(CPU *cpu)
{
    float top = stack_read_F32(cpu->stack, cpu->stack->sp - sizeof(float));
    char floatString[30];
    dtostrf(top, 10, 6, floatString);
    vmprintf("%s\n", floatString);
}

void add_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    float result = a + b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void subtract_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    float result = a - b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void multiply_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    float result = a * b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void divide_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->stack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->stack, &a, sizeof(a));
    float result = a / b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
}

void pop(CPU *cpu)
{
    // TODO: Pop value according the size of type stored
    uint8_t data_type;
    cpu_fetch_data(cpu, &data_type, sizeof(data_type));
    uint8_t size = get_data_type_size(data_type);
    if (size == WORD) // 2 bytes
    {
        uint16_t value;
        stack_pop_data(cpu->stack, &value, sizeof(value));
    }
    else if (size == DWORD) // 4 bytes
    {
        uint32_t value;
        stack_pop_data(cpu->stack, &value, sizeof(value));
    }
}

void pop_address(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    // TODO: Pop value according the size of type stored
    uint16_t value = stack_pop_16b(cpu->stack);
    if (address < cpu->port_bank->size)
    {
        port_bank_set_address(cpu->port_bank, address, (uint8_t)value);
    }
    else
    {
        address = cpu->user_memory + address - cpu->port_bank->size;
        memory_set_address_16b(cpu->memory, address, value);
    }
}

void top(CPU *cpu)
{
    uint8_t data_type;
    cpu_fetch_data(cpu, &data_type, sizeof(data_type));
    uint8_t size = get_data_type_size(data_type);
    if (size == WORD) // 2 bytes
    {
        uint16_t top;
        // stack_read_data(cpu->stack, &top, sizeof(top));
        if (data_type == TYPE_U16)
        {
            vmprintf("%u\n", top);
        }
        else if (data_type == TYPE_I16)
        {
            vmprintf("%i\n", top);
        }
    }
    else if (size == DWORD) // 4 bytes
    {
        if (data_type == TYPE_U32)
        {
            uint32_t top;
            // stack_read_data(cpu->stack, &top, sizeof(top));
            vmprintf("%lu\n", top);
        }
        else if (data_type == TYPE_I32)
        {
            uint32_t top;
            // stack_read_data(cpu->stack, &top, sizeof(top));
            vmprintf("%li\n", top);
        }
        else if (data_type == TYPE_F32)
        {
            float top;
            // stack_read_data(cpu->stack, &top, sizeof(top));
            char floatString[30];
            dtostrf(top, 5, 2, floatString);
            vmprintf("%s\n", floatString);
        }
    }
    else
    {
        vmprintf("TopError: unknown datatype %d\n", data_type);
    }
}
void delay(CPU *cpu)
{
    uint16_t milliseconds;
    cpu_fetch_data(cpu, &milliseconds, sizeof(milliseconds));
    delay_ms(milliseconds);
}

void jump(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, 2);
    cpu->ip = address;
}

void pop_jump_if_false(CPU *cpu)
{
    uint16_t address = cpu_fetch_16b(cpu);
    // TODO: Pop value according the size of type stored
    // Here result is a bool so it should be okay
    uint16_t result = stack_pop_16b(cpu->stack);
    if (!result)
        cpu->ip = address;
}

void compare_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a == b);
}

void compare_less(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a < b);
}

void compare_greater(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a > b);
}

void compare_less_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a <= b);
}

void compare_greater_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a >= b);
}

void add(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->stack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->stack, &a, sizeof(a));
    uint16_t result = a + b;
    stack_push_lend_data(cpu->stack, &result, sizeof(result));
    stack_print(cpu->stack);
}

void subtract(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a - b);
}

void multiply(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a * b);
}

void divide(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a / b);
}

void bitwise_and(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a & b);
}

void bitwise_or(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a | b);
}

void bitwise_xor(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a ^ b);
}

void bitwise_not(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, ~a);
}

void bitwise_left_shift(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a << b);
}

void bitwise_right_shift(CPU *cpu)
{
    // TODO: Pop value according the integer size
    uint16_t b = stack_pop_16b(cpu->stack);
    uint16_t a = stack_pop_16b(cpu->stack);
    stack_push_16b(cpu->stack, a >> b);
}

void call(CPU *cpu)
{
    // TODO: Push 16bit size (address size)
    // uint16_t address = cpu_fetch_16b(cpu);
    // stack_push_16b(cpu->callstack, cpu->ip);
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(uint16_t));
    stack_push_bend_data(cpu->callstack, &cpu->ip, sizeof(uint16_t));
    cpu->ip = address;
}

void ret(CPU *cpu)
{
    // TODO: Pop 16bit size (address size)
    // uint16_t address = stack_pop_16b(cpu->callstack);
    uint16_t address;
    stack_pop_bend_data(cpu->callstack, &address, sizeof(uint16_t));
    cpu->ip = address;
}

void syscall(CPU *cpu)
{
    uint16_t func_id;
    cpu_fetch_data(cpu, &func_id, sizeof(func_id));
    switch (func_id)
    {
    case BUILTIN_PRINT:
        builtin_print(cpu);
        break;
    case BUILTIN_TOGGLE_LED:
        builtin_toggle_led(cpu, 500);
        break;

    default:
        break;
    }
}

void addf(CPU *cpu)
{
    float a, b;
    stack_pop_data(cpu->stack, &b, sizeof(b));
    stack_pop_data(cpu->stack, &a, sizeof(a));
    float result = a + b;
    stack_push_bend_data(cpu->stack, &result, sizeof(result));
    stack_print(cpu->stack);
}
