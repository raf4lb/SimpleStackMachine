#include "instructions.h"
#include "serial.h"

int main(void)
{
    void (**instructions)() = instructions_create();

    CPU *cpu = cpu_create(16, 16, instructions);

    uint8_t program[] = {
        1,
        0,
        1,
        1,
        0,
        2,
        1,
        0,
        3,
        4,
        3,
        4,
        3,
        4,
        3,
        0,
    };

    cpu_load_program(cpu, program, 16);
    serial_setup();
    serial_printf("Running program\n");

    cpu_run(cpu);

    cpu_free(cpu);

    return 0;
}
