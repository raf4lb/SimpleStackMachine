#include <string.h>
#include <stdlib.h>
#include "instructions.h"
#include "serial.h"

#ifndef PROGRAM
#define PROGRAM "{}"
#endif

#ifndef PROGRAM_SIZE
#define PROGRAM_SIZE 0
#endif

uint8_t *convert_str_to_long_int(char str[], uint16_t size)
{
    uint8_t *array = malloc(size * sizeof(uint8_t)); // Dynamically allocate memory for the array

    // Check if memory allocation is successful
    if (array == NULL)
    {
        serial_printf("Memory allocation failed.\n");
        exit(1);
    }

    // Remove curly braces and split the string into individual elements
    char *token = strtok(str, "{,}");
    uint16_t i = 0;

    // Convert each element to long integer
    while (token != NULL)
    {
        array[i++] = (uint8_t)strtoul(token, NULL, 10);
        token = strtok(NULL, "{,}");
    }

    return array;
}

int main(void)
{
    char program_string[] = PROGRAM;
    uint16_t program_size = PROGRAM_SIZE;
    uint8_t *program = convert_str_to_long_int(program_string, program_size);

    void (**instructions)() = instructions_create();
    uint16_t memory_size = 128;
    uint16_t stack_size = 16;
    uint16_t callstack_size = 16;
    uint8_t port_banks = 3;
    CPU *cpu = cpu_create(memory_size, stack_size, callstack_size, instructions, port_banks);

    cpu_load_program(cpu, program, program_size);
    serial_setup();
    serial_printf("Running program\n");

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
//             serial_printf("%c\n", receivedData[i]);
//         }
//     }
// }