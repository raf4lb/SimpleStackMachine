
#ifndef DELAY_H
#define DELAY_H
#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#ifdef ARDUINO
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

void timer0_setup();
void timer1_setup();
uint32_t micros();

#endif

uint16_t millis();
void delay_ms(CPU *cpu, uint16_t milliseconds);
void delay_us(uint32_t microseconds);

#endif