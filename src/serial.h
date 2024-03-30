#ifndef SERIAL_H
#define SERIAL_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BUAD 9600
#define BUAD_RATE_CALC ((F_CPU / 16 / BUAD) - 1)

void serial_setup(void);

void serial_send(char *sendString);

void serial_printf(const char *format, ...);

#endif