#include "serial.h"
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdarg.h>
#include <stdlib.h>

void serial_setup()
{
    // Register settings
    // High and low bits
    UBRR0H = (BUAD_RATE_CALC >> 8);
    UBRR0L = BUAD_RATE_CALC;
    // transimit and recieve enable
    UCSR0B = (1 << TXEN0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit data format
}

void serial_send(char *sendString)
{
    int i = 0;
    for (i = 0; i < strlen(sendString); i++)
    {
        while ((UCSR0A & (1 << UDRE0)) == 0)
        {
        };
        UDR0 = sendString[i];
    }
}

void serial_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Determine the size needed for the buffer
    int size = vsnprintf(NULL, 0, format, args);

    // Allocate memory for the buffer
    char *buffer = (char *)malloc(sizeof(char) + 1); // +1 for null terminator

    // Reset va_list for actual usage
    va_end(args);
    va_start(args, format);

    // Write formatted data into the buffer
    vsnprintf(buffer, size + 1, format, args);

    va_end(args);

    // Now you can use the buffer as needed, for example, printing it
    serial_send(buffer);
    free(buffer);
}