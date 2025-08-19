#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum
{
    OP_HALT = 0,
    OP_DELAY = 1,
    OP_JUMP = 2,
    OP_POP_JUMP_IF_FALSE = 3,
    OP_COMPARE_EQ = 4,
    OP_COMPARE_LT = 5,
    OP_COMPARE_GT = 6,
    OP_COMPARE_LE = 7,
    OP_COMPARE_GE = 8,
    OP_CALL = 9,
    OP_RETURN = 10,
    OP_SYSCALL = 11,
    OP_ASYNC_CALL = 12,
    OP_ASYNC_RETURN = 13,
    OP_PUSHL_U16 = 14,
    OP_POP_U16 = 15,
    OP_TOP_U16 = 16,
    OP_ADD_U16 = 17,
    OP_SUB_U16 = 18,
    OP_MUL_U16 = 19,
    OP_DIV_U16 = 20,
    OP_POPA_U16 = 21,
    OP_PUSH_U16 = 22,
    OP_ALLOC_LOCAL = 23,
    OP_PUSH_LOCAL = 24,
    OP_POP_LOCAL = 25,
    OP_PUSHL_I16 = 26,
    OP_POP_I16 = 27,
    OP_TOP_I16 = 28,
    OP_ADD_I16 = 29,
    OP_SUB_I16 = 30,
    OP_MUL_I16 = 31,
    OP_DIV_I16 = 32,
    OP_READ_I16 = 33,
    OP_WRITE_I16 = 34,
    OP_ALLOC_I16 = 35,
    OP_FREE_I16 = 36,
    OP_PUSHL_F32 = 37,
    OP_POP_F32 = 38,
    OP_TOP_F32 = 39,
    OP_ADD_F32 = 40,
    OP_SUB_F32 = 41,
    OP_MUL_F32 = 42,
    OP_DIV_F32 = 43,
    OP_PUSH_MILLIS = 44,
    OP_LSHIFT_U16 = 45,
    OP_RSHIFT_U16 = 46,
    OP_OR_U16 = 47,
    OP_XOR_U16 = 48,
    OP_AND_U16 = 49,
    OP_NOT_U16 = 50,
    OP_PUSH_LOCAL_REF = 51,
    OP_PUSH_CONST_REF = 52,
    OP_PUSH_CONST = 53,
} OpCode;

void halt(CPU *cpu);

void delay(CPU *cpu);

void jump(CPU *cpu);

void pop_jump_if_false(CPU *cpu);

void compare_equal(CPU *cpu);

void compare_less(CPU *cpu);

void compare_greater(CPU *cpu);

void compare_less_equal(CPU *cpu);

void compare_greater_equal(CPU *cpu);

void bitwise_and_U16(CPU *cpu);

void bitwise_or_U16(CPU *cpu);

void bitwise_xor_U16(CPU *cpu);

void bitwise_not_U16(CPU *cpu);

void bitwise_left_shift_U16(CPU *cpu);

void bitwise_right_shift_U16(CPU *cpu);

void call(CPU *cpu);

void call_address(CPU *cpu, uint16_t address);

void ret(CPU *cpu);

void async_call(CPU *cpu);

void async_ret(CPU *cpu);

void syscall(CPU *cpu);

void push_literal_U16(CPU *cpu);

void pop_U16(CPU *cpu);

void push_U16(CPU *cpu);

void pop_address_U16(CPU *cpu);

void load_U16(CPU *cpu);

void store_U16(CPU *cpu);

void var_U16(CPU *cpu);

void del_U16(CPU *cpu);

void top_U16(CPU *cpu);

void add_U16(CPU *cpu);

void subtract_U16(CPU *cpu);

void multiply_U16(CPU *cpu);

void divide_U16(CPU *cpu);

void push_literal_I16(CPU *cpu);

void pop_I16(CPU *cpu);

void load_I16(CPU *cpu);

void store_I16(CPU *cpu);

void var_I16(CPU *cpu);

void del_I16(CPU *cpu);

void top_I16(CPU *cpu);

void add_I16(CPU *cpu);

void subtract_I16(CPU *cpu);

void multiply_I16(CPU *cpu);

void divide_I16(CPU *cpu);

void push_literal_F32(CPU *cpu);

void pop_F32(CPU *cpu);

void top_F32(CPU *cpu);

void add_F32(CPU *cpu);

void subtract_F32(CPU *cpu);

void multiply_F32(CPU *cpu);

void divide_F32(CPU *cpu);

void push_millis(CPU *cpu);

void allocate_local(CPU *cpu);

void push_local(CPU *cpu);

void pop_local(CPU *cpu);

void push_local_reference(CPU *cpu);

void push_constant_reference(CPU *cpu);

void push_constant(CPU *cpu);

#endif