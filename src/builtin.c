#include "builtin.h"
#include "delay.h"
#include <stdint.h>
#include "stack.h"

void builtin_print(CPU *cpu)
{
    uint16_t buffer_address;
    stack_pop_bend_data(cpu->stack, &buffer_address, sizeof(uint16_t)); // pop 2 bytes
    // vmprintf("address %d = %d\n", buffer_address, cpu->memory->data[cpu->data_memory + buffer_address]);
    char *buffer = (char *)&cpu->program[cpu->data_address + buffer_address - cpu->port_bank->size];

    // stack_pop_bytes(cpu->stack, address, 2); // pop 2 bytes
    // int16_t integer = *(int16_t *)address;

    // stack_pop_bytes(cpu->stack, address, 2); // pop 2 bytes
    // int16_t integer2 = *(int16_t *)address;

    // uint8_t faddress[4];
    // stack_pop_bytes(cpu->stack, faddress, 4); // pop 2 bytes
    // float floatValue = *(float *)faddress;

    // vmprintf(buffer, integer, integer2, floatValue);
    vmprintf(buffer);
}

void builtin_print_stack(CPU *cpu)
{
    stack_print(cpu->stack);
}

void builtin_toggle_led(CPU *cpu, uint16_t milliseconds)
{
#ifdef ARDUINO

#include "avr/io.h"
#define LED_PIN PB5 // Define LED pin (Arduino digital pin 13 on ATmega328P)
                    // Set LED pin as output
    DDRB |= (1 << LED_PIN);
    // Toggle LED
    PORTB ^= (1 << LED_PIN);
    delay_ms(cpu, milliseconds);
#endif
}

void builtin_syscall(CPU *cpu)
{
    uint16_t func_id;
    cpu_fetch_data(cpu, &func_id, sizeof(func_id));
    switch (func_id)
    {
    case BUILTIN_PRINT:
        builtin_print(cpu);
        break;
    case BUILTIN_TOGGLE_LED:
        builtin_toggle_led(cpu, 500);
        break;
    case BUILTIN_PRINT_STACK:
        builtin_print_stack(cpu);
        break;
    default:
        break;
    }
}