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

#define program_size PROGRAM_SIZE
#define data_address DATA_ADDRESS
#define memory_size 0
#define stack_size 32
#define callstack_size 16
#define port_banks 3

const uint8_t program[] = PROGRAM;

int main(void)
{

    timer_init();
#ifdef SERIAL_ENABLED
    serial_setup(9600);
#endif
    vmprintf("Starting VM\n");
    CPU *cpu = cpu_create(memory_size, stack_size, callstack_size, port_banks);
    cpu_load_program(cpu, program, program_size, data_address);
    cpu_run(cpu);
    cpu_free(cpu);
    return 0;
}