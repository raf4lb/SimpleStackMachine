#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

void halt(CPU *cpu);

void push(CPU *cpu);

void push_literal(CPU *cpu);

void pop(CPU *cpu);

void pop_address(CPU *cpu);

void top(CPU *cpu);

void delay(CPU *cpu);

void jump(CPU *cpu);

void pop_jump_if_false(CPU *cpu);

void compare_equal(CPU *cpu);

void compare_less(CPU *cpu);

void compare_greater(CPU *cpu);

void compare_less_equal(CPU *cpu);

void compare_greater_equal(CPU *cpu);

void add(CPU *cpu);

void subtract(CPU *cpu);

void multiply(CPU *cpu);

void divide(CPU *cpu);

void bitwise_and(CPU *cpu);

void bitwise_and_U16(CPU *cpu);

void bitwise_or(CPU *cpu);

void bitwise_or_U16(CPU *cpu);

void bitwise_xor(CPU *cpu);

void bitwise_not(CPU *cpu);

void bitwise_not_U16(CPU *cpu);

void bitwise_left_shift(CPU *cpu);

void bitwise_left_shift_U16(CPU *cpu);

void bitwise_right_shift(CPU *cpu);

void call(CPU *cpu);

void ret(CPU *cpu);

void async_call(CPU *cpu);

void async_ret(CPU *cpu);

void syscall(CPU *cpu);

void addf(CPU *cpu);

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