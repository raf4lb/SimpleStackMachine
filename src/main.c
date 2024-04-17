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

InstructionPtr instructions_set[] = {
    halt,
    push_literal,
    push,
    pop,
    pop_address,
    top,
    delay,
    jump,
    pop_jump_if_false,
    compare_equal,
    compare_less,
    compare_greater,
    compare_less_equal,
    compare_greater_equal,
    add,
    subtract,
    multiply,
    divide,
    bitwise_and,
    bitwise_or,
    bitwise_xor,
    bitwise_not,
    bitwise_left_shift,
    bitwise_right_shift,
    call,
    ret,
    syscall,
};

uint8_t program[] = PROGRAM;
uint16_t program_size = PROGRAM_SIZE;
uint16_t data_address = DATA_ADDRESS;
uint16_t memory_size = 32;
uint16_t stack_size = 16;
uint16_t callstack_size = 16;
uint8_t port_banks = 3;

int main(void)
{

    CPU *cpu = cpu_create(memory_size, stack_size, callstack_size, instructions_set, port_banks);

    cpu_load_program(cpu, program, program_size, data_address);
#ifdef ARDUINO
    serial_setup();
#endif
    vmprintf("Memory consumption: %d bytes\n", get_memory_usage());
    vmprintf(" - VM consumption: %d bytes\n", get_memory_usage() - memory_size);
    vmprintf(" - User memory: %d bytes\n", memory_size);
    vmprintf("Running program\n");
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