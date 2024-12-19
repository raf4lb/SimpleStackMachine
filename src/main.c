#include <string.h>
#include "instructions.h"
#include "sys.h"
#include "delay.h"
#include "cpu.h"

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
    &halt,
    &push_literal,
    &push,
    &pop,
    &pop_address,
    &top,
    &delay,
    &jump,
    &pop_jump_if_false,
    &compare_equal,
    &compare_less,
    &compare_greater,
    &compare_less_equal,
    &compare_greater_equal,
    &add,
    &subtract,
    &multiply,
    &divide,
    &bitwise_and,
    &bitwise_or,
    &bitwise_xor,
    &bitwise_not,
    &bitwise_left_shift,
    &bitwise_right_shift,
    &call,
    &ret,
    &syscall,
    &addf,
    &push_literal_U16,
    &pop_U16,
    &top_U16,
    &add_U16,
    &push_literal_I16,
    &pop_I16,
    &top_I16,
    &add_I16,
    &subtract_I16,
    &multiply_I16,
    &divide_I16,
    &push_literal_F32,
    &pop_F32,
    &top_F32,
    &add_F32,
    &subtract_F32,
    &multiply_F32,
    &divide_F32,
    &load_I16,
    &store_I16,
    &var_I16,
    &del_I16,
    &subtract_U16,
    &multiply_U16,
    &divide_U16,
    &pop_address_U16,
    &push_U16,
    &push_millis,
};

uint8_t program[] = PROGRAM;
uint16_t program_size = PROGRAM_SIZE;
uint16_t data_address = DATA_ADDRESS;
uint16_t memory_size = 0;
uint16_t stack_size = 32;
uint16_t callstack_size = 16;
uint8_t port_banks = 3;

int main(void)
{
#ifdef ARDUINO
    timer0_setup();
#ifdef SERIAL_ENABLED
    serial_setup(9600);
#endif
#endif
    cpu_init(memory_size, stack_size, callstack_size, instructions_set, port_banks);
    cpu_load_program(&cpu, program, program_size, data_address);
    vmprintf("Memory consumption: %d bytes\n", get_memory_usage());
    vmprintf(" - VM consumption: %d bytes\n", get_memory_usage() - memory_size);
    vmprintf(" - User memory: %d bytes\n", memory_size);
    vmprintf("Running program\n");
    cpu_run(&cpu);
    cpu_free(&cpu);
    return 0;
}