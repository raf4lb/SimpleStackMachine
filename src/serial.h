#ifdef ARDUINO
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

int serial_printf(const char *format, ...);

void serial_setup(uint16_t baud);

void serial_write(const char *str);

#endif
#endif