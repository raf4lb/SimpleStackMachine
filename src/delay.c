#include "delay.h"
#ifdef ARDUINO
#include <util/delay.h> // Delay functions
#endif
#ifdef MACOSX
#include <unistd.h>
#endif
void delay_ms(uint16_t milliseconds)
{
    while (milliseconds--)
    {
#ifdef ARDUINO
        _delay_ms(1); // Delay 1 millisecond using built-in _delay_ms function
#elif MACOSX
        usleep(1000);
#endif
    }
}
