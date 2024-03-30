#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "cpu.h"

void halt(CPU *cpu);

void push(CPU *cpu);

void push_literal(CPU *cpu);

void pop(CPU *cpu);

void top(CPU *cpu);

#define INSTRUCTIONS 5

void (**instructions_create())();

#endif