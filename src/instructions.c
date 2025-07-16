#include <stdlib.h>
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

void push_literal_U16(CPU *cpu)
{
    uint16_t value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void pop_U16(CPU *cpu)
{
    uint16_t value;
    stack_pop_bend_data(cpu->opstack, &value, sizeof(value));
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
        value = (uint16_t)cpu->program[cpu->data_address + address - cpu->port_bank->size];
    }
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void pop_address_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_pop_bend_data(cpu->opstack, &value, sizeof(value));
    if (address < cpu->port_bank->size)
    {
        port_bank_set_address(cpu->port_bank, address, (uint8_t)value);
    }
    else
    {
        vmprintf("ERROR: invalid cpu address\n");
        exit(EXIT_FAILURE);
    }
}

void load_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_read_bend_data_at(cpu->stack, &value, sizeof(value), address);
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void store_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    uint16_t value;
    stack_pop_bend_data(cpu->opstack, &value, sizeof(value));
    stack_write_bend_data_at(cpu->stack, &value, sizeof(value), address);
}

void var_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->stack->sp += sizeof(uint16_t);
}

void del_U16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->stack->sp -= sizeof(uint16_t);
}

void top_U16(CPU *cpu)
{
    uint16_t top = stack_read_U16(cpu->opstack, cpu->opstack->sp - sizeof(uint16_t));
    vmprintf("%hu\n", top);
}

void add_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a + b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void subtract_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a - b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void multiply_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a * b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void divide_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a / b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void push_literal_I16(CPU *cpu)
{
    int16_t value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void pop_I16(CPU *cpu)
{
    cpu->opstack->sp -= sizeof(int16_t);
}

void load_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    int16_t value;
    stack_read_bend_data_at(cpu->stack, &value, sizeof(value), address);
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void store_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    int16_t value;
    stack_pop_bend_data(cpu->opstack, &value, sizeof(value));
    stack_write_bend_data_at(cpu->stack, &value, sizeof(value), address);
}

void var_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->stack->sp += sizeof(int16_t);
}

void del_I16(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    cpu->stack->sp -= sizeof(int16_t);
}

void top_I16(CPU *cpu)
{
    int16_t top = stack_read_I16(cpu->opstack, cpu->opstack->sp - sizeof(int16_t));
    vmprintf("%hi\n", top);
}

void add_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    int16_t result = a + b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void subtract_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    int16_t result = a - b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void multiply_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    int16_t result = a * b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void divide_I16(CPU *cpu)
{
    int16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    int16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    int16_t result = a / b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void push_literal_F32(CPU *cpu)
{
    float value;
    cpu_fetch_data(cpu, &value, sizeof(value));
    stack_push_bend_data(cpu->opstack, &value, sizeof(value));
}

void pop_F32(CPU *cpu)
{
    float value;
    stack_pop_bend_data(cpu->opstack, &value, sizeof(value));
}

void top_F32(CPU *cpu)
{
    float top = stack_read_F32(cpu->opstack, cpu->opstack->sp - sizeof(float));
#ifdef ARDUINO
    char floatString[30];
    dtostrf(top, 10, 2, floatString);
    vmprintf("%s\n", floatString);
#elif MACOSX
    vmprintf("%.2f\n", top);
#endif
}

void add_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    float result = a + b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void subtract_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    float result = a - b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void multiply_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    float result = a * b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void divide_F32(CPU *cpu)
{
    float b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    float a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    float result = a / b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(result));
}

void delay(CPU *cpu)
{
    uint16_t milliseconds;
    cpu_fetch_data(cpu, &milliseconds, sizeof(milliseconds));
    delay_ms(cpu, milliseconds);
}

void jump(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, 2);
    cpu->ip = address;
}

void pop_jump_if_false(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    // TODO: Pop value according the size of type stored
    // Here result is a bool so it should be okay
    bool result;
    stack_pop_bend_data(cpu->opstack, &result, sizeof(result));
    if (!result)
        cpu->ip = address;
}

void compare_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    bool result = a == b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(bool));
}

void compare_less(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->opstack);
    uint16_t a = stack_pop_16b(cpu->opstack);
    stack_push_16b(cpu->opstack, a < b);
}

void compare_greater(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b;
    stack_pop_bend_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_bend_data(cpu->opstack, &a, sizeof(a));
    bool result = a > b;
    stack_push_bend_data(cpu->opstack, &result, sizeof(bool));
}

void compare_less_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->opstack);
    uint16_t a = stack_pop_16b(cpu->opstack);
    stack_push_16b(cpu->opstack, a <= b);
}

void compare_greater_equal(CPU *cpu)
{
    // TODO: Pop value according the size of type stored (int, float, etc)
    uint16_t b = stack_pop_16b(cpu->opstack);
    uint16_t a = stack_pop_16b(cpu->opstack);
    stack_push_16b(cpu->opstack, a >= b);
}

void bitwise_and_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a & b;
    stack_push_data(cpu->opstack, &result, sizeof(result));
}

void bitwise_or_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a | b;
    stack_push_data(cpu->opstack, &result, sizeof(result));
}

void bitwise_xor_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a ^ b;
    stack_push_data(cpu->opstack, &result, sizeof(result));
}

void bitwise_not_U16(CPU *cpu)
{
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    a = ~a;
    stack_push_data(cpu->opstack, &a, sizeof(a));
}

void bitwise_left_shift_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a << b;
    stack_push_data(cpu->opstack, &result, sizeof(result));
}

void bitwise_right_shift_U16(CPU *cpu)
{
    uint16_t b;
    stack_pop_data(cpu->opstack, &b, sizeof(b));
    uint16_t a;
    stack_pop_data(cpu->opstack, &a, sizeof(a));
    uint16_t result = a >> b;
    stack_push_data(cpu->opstack, &result, sizeof(result));
}

void call(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    call_address(cpu, address);
}

void call_address(CPU *cpu, uint16_t address)
{
    stack_push_data(cpu->stack, &cpu->ip, sizeof(cpu->ip));
    cpu->ip = address;
    stack_push_data(cpu->stack, &cpu->stack->bp, sizeof(cpu->stack->bp));
    cpu->stack->bp = cpu->stack->sp;
}

void ret(CPU *cpu)
{
    cpu->stack->sp = cpu->stack->bp;
    stack_pop_data(cpu->stack, &cpu->stack->bp, sizeof(cpu->stack->bp));
    uint16_t address;
    stack_pop_data(cpu->stack, &address, sizeof(address));
    cpu->ip = address;
}

void async_call(CPU *cpu)
{
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(uint16_t));
    cpu_create_task(cpu, address);
}

void async_ret(CPU *cpu)
{
    cpu_delete_task(cpu, cpu->task_tree_current_node);
}

void syscall(CPU *cpu)
{
    builtin_syscall(cpu);
}

void push_millis(CPU *cpu)
{
    uint16_t time = millis();
    stack_push_bend_data(cpu->opstack, &time, sizeof(time));
}

void allocate_local(CPU *cpu)
{
    uint16_t size;
    cpu_fetch_data(cpu, &size, sizeof(size));
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    // do nothing with the address?
    cpu->stack->sp += size;
}

void push_local(CPU *cpu)
{
    uint16_t size;
    cpu_fetch_data(cpu, &size, sizeof(size));
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    void *value;
    stack_read_data_at(cpu->stack, &value, sizeof(value), cpu->stack->bp + address);
    stack_push_bend_data(cpu->opstack, &value, size);
}

void pop_local(CPU *cpu)
{
    uint16_t size;
    cpu_fetch_data(cpu, &size, sizeof(size));
    uint16_t address;
    cpu_fetch_data(cpu, &address, sizeof(address));
    void *value;
    stack_pop_bend_data(cpu->opstack, &value, size);
    stack_write_bend_data_at(cpu->stack, &value, sizeof(value), cpu->stack->bp + address);
}
