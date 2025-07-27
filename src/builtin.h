#ifndef BUILTIN_H
#define BUILTIN_H
#include "cpu.h"

#define BUILTIN_PRINT 0
#define BUILTIN_TOGGLE_LED 1
#define BUILTIN_PRINT_OPSTACK 2
#define BUILTIN_SEND_MESSAGE 3
#define BUILTIN_GET_MEMORY_USAGE 4
#define BUILTIN_PRINT_STACK 5

void builtin_print(CPU *cpu);
void builtin_toggle_led(CPU *cpu, uint16_t microseconds);
void builtin_print_opstack(CPU *cpu);
void builtin_syscall(CPU *cpu);
void builtin_send_message(CPU *cpu);
void builtin_get_memory_usage(CPU *cpu);
void builtin_print_stack(CPU *cpu);
#endif