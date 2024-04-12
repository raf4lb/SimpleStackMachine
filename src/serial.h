#ifdef ARDUINO
#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BUAD 9600
#define BUAD_RATE_CALC ((F_CPU / 16 / BUAD) - 1)

void serial_setup(void);

void serial_send(char *sendString);

uint8_t USART_receive();

void USART_transmit(uint8_t data);

int serial_printf(const char *format, ...);

#endif
#endif