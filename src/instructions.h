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

void bitwise_or(CPU *cpu);

void bitwise_xor(CPU *cpu);

void bitwise_not(CPU *cpu);

void bitwise_left_shift(CPU *cpu);

void bitwise_right_shift(CPU *cpu);

void call(CPU *cpu);

void ret(CPU *cpu);

#define INSTRUCTIONS 26

void (**instructions_create())();

#endif