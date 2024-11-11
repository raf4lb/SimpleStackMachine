#ifdef ARDUINO
#include <string.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdarg.h>
#include "sys.h"
#include "serial.h"

void serial_setup(uint16_t baud)
{
    uint16_t ubrr_value = F_CPU / 16 / baud - 1;
    // Set baud rate
    UBRR0H = (ubrr_value >> 8);
    UBRR0L = ubrr_value;
    // Enable transmitter
    UCSR0B = (1 << TXEN0);
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void serial_write(const char *str)
{
    while (*str)
    {
        // Wait until the transmit buffer is empty
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        // Load the next character into the buffer
        UDR0 = *str;
        str++;
    }
}

int serial_printf(const char *format, ...)
{
#ifdef SERIAL_ENABLED
    va_list args;
    va_start(args, format);

    // Determine the size needed for the buffer
    int size = vsnprintf(NULL, 0, format, args);

    // Allocate memory for the buffer
    char *buffer = (char *)vmmalloc(sizeof(char) + 1); // +1 for null terminator

    // Reset va_list for actual usage
    va_end(args);
    va_start(args, format);

    // Write formatted data into the buffer
    vsnprintf(buffer, size + 1, format, args);

    va_end(args);

    // Now you can use the buffer as needed, for example, printing it
    serial_write(buffer);
    vmfree(buffer);
#endif
    return 0;
}
#endif