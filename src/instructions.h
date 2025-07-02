#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

typedef enum
{
    OP_HALT = 0,

    // Control flow
    OP_DELAY,
    OP_JUMP,
    OP_POP_JUMP_IF_FALSE,

    // Comparisons
    OP_COMPARE_EQ,
    OP_COMPARE_LT,
    OP_COMPARE_GT,
    OP_COMPARE_LE,
    OP_COMPARE_GE,

    // Calls and syscalls
    OP_CALL,
    OP_RETURN,
    OP_SYSCALL,
    OP_ASYNC_CALL,
    OP_ASYNC_RETURN,

    // U16 operations
    OP_PUSHL_U16,
    OP_POP_U16,
    OP_TOP_U16,
    OP_ADD_U16,
    OP_SUB_U16,
    OP_MUL_U16,
    OP_DIV_U16,
    OP_POPA_U16,

    // U16 stack/local
    OP_PUSH_U16,
    OP_PUSH_LOCAL_U16,
    OP_POP_LOCAL_U16,
    OP_PARENT_POP_LOCAL_U16,

    // I16 operations and memory
    OP_PUSHL_I16,
    OP_POP_I16,
    OP_TOP_I16,
    OP_ADD_I16,
    OP_SUB_I16,
    OP_MUL_I16,
    OP_DIV_I16,
    OP_READ_I16,
    OP_WRITE_I16,
    OP_ALLOC_I16,
    OP_FREE_I16,

    // F32 operations
    OP_PUSHL_F32,
    OP_POP_F32,
    OP_TOP_F32,
    OP_ADD_F32,
    OP_SUB_F32,
    OP_MUL_F32,
    OP_DIV_F32,

    // Time
    OP_PUSH_MILLIS,

    // Bitwise operations
    OP_LSHIFT_U16,
    OP_RSHIFT_U16,
    OP_OR_U16,
    OP_XOR_U16,
    OP_AND_U16,
    OP_NOT_U16

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

void push_local_U16(CPU *cpu);

void pop_local_U16(CPU *cpu);

void parent_pop_local_U16(CPU *cpu);

#endif