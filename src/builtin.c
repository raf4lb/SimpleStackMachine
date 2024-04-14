#include "builtin.h"
#include <stdint.h>

void print(CPU *cpu)
{
    uint16_t buffer_address = stack_pop_uint16_t(cpu->stack); // pop 2 bytes
    // vmprintf("address %d = %d\n", buffer_address, cpu->memory->data[cpu->data_memory + buffer_address]);
    const char *buffer = (const char *)&cpu->memory->data[cpu->data_memory + buffer_address];

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