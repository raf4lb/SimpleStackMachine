#include <string.h>
#include "instructions.h"
#include "sys.h"

#ifndef PROGRAM
#define PROGRAM "{}"
#endif

#ifndef PROGRAM_SIZE
#define PROGRAM_SIZE 0
#endif

#ifndef DATA_ADDRESS
#define DATA_ADDRESS 0
#endif

uint8_t program[] = PROGRAM;
uint16_t program_size = PROGRAM_SIZE;
uint16_t data_address = DATA_ADDRESS;
uint16_t memory_size = 1024;
uint16_t stack_size = 16;
uint16_t callstack_size = 16;
uint8_t port_banks = 3;

int main(void)
{
    void (**instructions)() = instructions_create();
    CPU *cpu = cpu_create(memory_size, stack_size, callstack_size, instructions, port_banks);

    cpu_load_program(cpu, program, program_size, data_address);
#ifdef ARDUINO
    serial_setup();
#endif
    vmprintf("Running program\n");
    vmprintf("Memory usage: %d bytes\n", get_memory_usage());
    vmprintf("Free memory: %d bytes\n", free_memory());
    cpu_run(cpu);
    cpu_free(cpu);
    return 0;
}

// int main(void)
// {
//     int size = 5;
//     uint8_t receivedData[size]; // Array to store received data
//     int i;

//     serial_setup();

//     while (1)
//     {
//         i = 0;
//         for (i = 0; i < size; i++)
//         {
//             receivedData[i] = USART_receive();
//         }
//         for (i = 0; i < size; i++)
//         {
//             vmprintf("%c\n", receivedData[i]);
//         }
//     }
// }