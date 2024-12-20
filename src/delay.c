#include "delay.h"
#include <unistd.h>
#include <stdbool.h>
#ifdef ARDUINO

volatile uint16_t millis_counter = 0; // Milliseconds counter
volatile uint16_t context_switch_conter = 0;

// Initialize Timer0 to generate an interrupt every 1 millisecond
void timer0_setup()
{
    // Set Timer0 to CTC mode
    TCCR0A = (1 << WGM01);

    // Set the Timer0 compare value for 1ms (16 MHz / 64 prescaler / 1000 Hz = 250)
    OCR0A = 249;

    // Set the prescaler to 64
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Enable Timer0 Compare Match A Interrupt
    TIMSK0 = (1 << OCIE0A);

    // Enable global interrupts
    sei();
}

// Timer0 overflow ISR to increment the millisecond counter
ISR(TIMER0_COMPA_vect)
{
    millis_counter++; // Increment every millisecond
    context_switch_conter++;
    // if (context_switch_conter > CONTEXT_SWITCH_MS)
    // {
    //     cpu_context_switch(&cpu);
    //     context_switch_conter = 0;
    // }
}

uint16_t millis()
{
    uint16_t millis;
    // Disable interrupts temporarily to read the shared variable
    cli();
    millis = millis_counter;
    sei();
    return millis;
}

void delay_ms(CPU *cpu, uint16_t milliseconds)
{
    uint16_t started_at = millis();

    // Check if the interval has passed
    while (((millis() - started_at) < milliseconds))
        ;
}

volatile uint32_t micros_counter = 0;

void timer1_setup()
{
    // Set Timer1 to CTC (Clear Timer on Compare Match) mode
    TCCR1B |= (1 << WGM12);
    // Set prescaler to 8 (for a 16 MHz clock, 1 µs interval)
    TCCR1B |= (1 << CS11);
    // Set OCR1A for 1 µs interval (16 MHz / 8 = 2 MHz, so 1 tick = 0.5 µs)
    OCR1A = 2; // 1 tick at prescaler 8 = 0.5 µs, so 2 ticks = 1 µs
    // Enable Timer1 compare match interrupt
    TIMSK1 |= (1 << OCIE1A);
    // Enable global interrupts
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    // Increment the microsecond counter every 1 µs
    micros_counter++;
}

uint32_t micros()
{
    uint32_t micros;
    // Disable interrupts temporarily to read the shared variable
    cli();
    micros = micros_counter;
    sei();
    return micros;
}

void delay_us(uint32_t microseconds)
{
    uint32_t started_at = micros();
    uint32_t elapsed_time = 0;

    // Check if the interval has passed
    while ((micros() - started_at) < microseconds)
        ;
}

#elif MACOSX
#include <sys/time.h>
#include <unistd.h>
uint16_t millis()
{
    vmprintf("time");
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t milliseconds = (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
    return (uint16_t)(milliseconds % 65536); // Wrap the time to fit into 16 bits
    vmprintf("time");
}

void delay_ms(CPU *cpu, uint16_t milliseconds)
{
    usleep(milliseconds * 1000);
}
#endif