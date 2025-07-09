#include <stdint.h>
#include "builtin.h"
#include "delay.h"
#include "stack.h"

void builtin_print(CPU *cpu)
{
    uint16_t buffer_address;
    stack_pop_bend_data(cpu->opstack, &buffer_address, sizeof(uint16_t)); // pop 2 bytes
    char *buffer = (char *)&cpu->program[cpu->data_address + buffer_address - cpu->port_bank->size];

    // stack_pop_bytes(cpu->opstack, address, 2); // pop 2 bytes
    // int16_t integer = *(int16_t *)address;

    // stack_pop_bytes(cpu->opstack, address, 2); // pop 2 bytes
    // int16_t integer2 = *(int16_t *)address;

    // uint8_t faddress[4];
    // stack_pop_bytes(cpu->opstack, faddress, 4); // pop 2 bytes
    // float floatValue = *(float *)faddress;

    // vmprintf(buffer, integer, integer2, floatValue);
    vmprintf(buffer);
}

void builtin_print_stack(CPU *cpu)
{
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

void builtin_get_memory_usage(CPU *cpu)
{
    uint16_t memory_usage = get_memory_usage();
    stack_push_data(cpu->opstack, &memory_usage, sizeof(memory_usage));
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
    case BUILTIN_SEND_MESSAGE:
        builtin_send_message(cpu);
        break;
    case BUILTIN_GET_MEMORY_USAGE:
        builtin_get_memory_usage(cpu);
        break;
    default:
        break;
    }
}