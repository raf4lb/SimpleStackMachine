#include "delay.h"
#include <util/delay.h> // Delay functions

void delay_ms(uint16_t milliseconds)
{
    while (milliseconds--)
    {
        _delay_ms(1); // Delay 1 millisecond using built-in _delay_ms function
    }
}
