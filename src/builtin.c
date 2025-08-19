#include <stdint.h>
#include "builtin.h"
#include "delay.h"
#include "stack.h"

void builtin_print(CPU *cpu)
{
    uint8_t *format;
    stack_pop_data(cpu->opstack, &format, sizeof(format));

    uint16_t u16;
    int16_t i16;
    uint8_t u8;
    float f32;
    uint8_t *string;

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'u':
                stack_pop_data(cpu->opstack, &u16, sizeof(u16));
                vmprintf("%hu", u16);
                break;
            case 'i':
                stack_pop_data(cpu->opstack, &i16, sizeof(i16));
                vmprintf("%hi", i16);
                break;
            case 'f':
#ifdef ARDUINO
                // TODO: fix printing floats for arduino
                vmprintf("FLOAT UNSUPPORTED");
#elif MACOSX || WINDOWS
                stack_pop_data(cpu->opstack, &f32, sizeof(f32));
                vmprintf("%.2f", f32);
#endif
                break;
            case 'c':
                stack_pop_data(cpu->opstack, &u8, sizeof(u8));
                vmprintf("%c", u8);
                break;
            case 's':
                stack_pop_data(cpu->opstack, &string, sizeof(string));
                vmprintf("%s", string);
                break;
            default:
                vmprintf("%%%c", *format);
            }
        }
        else
        {
            vmprintf("%c", *format);
        }
        format++;
    }
}

void builtin_print_opstack(CPU *cpu)
{
    vmprintf("OPSTACK\n");
    stack_print(cpu->opstack);
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

void builtin_send_message(CPU *cpu)
{

    uint16_t payload_size;
    stack_pop_data(cpu->opstack, &payload_size, sizeof(payload_size));

    uint16_t payload_address;
    stack_pop_data(cpu->opstack, &payload_address, sizeof(payload_address));
    uint8_t *payload = (uint8_t *)&cpu->program[cpu->data_address + payload_address - cpu->port_bank->size];

    uint16_t task_dst_id;
    stack_pop_data(cpu->opstack, &task_dst_id, sizeof(task_dst_id));

    uint16_t task_src_id = cpu->task_tree_current_node->task->id;

    message_queue_send_message(cpu->message_queues, task_src_id, task_dst_id, payload, payload_size);
}

void builtin_push_memory_usage(CPU *cpu)
{
    uint16_t memory_usage = get_memory_usage();
    stack_push_data(cpu->opstack, &memory_usage, sizeof(memory_usage));
}

void builtin_print_stack(CPU *cpu)
{
    vmprintf("STACK\n");
    stack_print(cpu->stack);
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
    case BUILTIN_PRINT_OPSTACK:
        builtin_print_opstack(cpu);
        break;
    case BUILTIN_SEND_MESSAGE:
        builtin_send_message(cpu);
        break;
    case BUILTIN_PUSH_MEMORY_USAGE:
        builtin_push_memory_usage(cpu);
        break;
    case BUILTIN_PRINT_STACK:
        builtin_print_stack(cpu);
        break;
    default:
        break;
    }
}