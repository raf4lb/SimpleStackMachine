#ifndef BUILTIN_H
#define BUILTIN_H
#include "cpu.h"

#define BUILTIN_PRINT 0
#define BUILTIN_TOGGLE_LED 1
#define BUILTIN_PRINT_STACK 2

void builtin_print(CPU *cpu);
void builtin_toggle_led(CPU *cpu, uint16_t microseconds);
void builtin_print_stack(CPU *cpu);
void builtin_syscall(CPU *cpu);
#endif