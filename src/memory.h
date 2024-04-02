#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t *data; // array of 8 bits
    uint16_t size; // maximum size is 65536
} Memory;

Memory *memory_create(uint16_t size);

void memory_free(Memory *memory);

void memory_print(Memory *memory);

uint8_t memory_get_address(Memory *memory, uint16_t address);

void memory_set_address(Memory *memory, uint16_t address, uint8_t value);

uint16_t memory_get_address_16b(Memory *memory, uint16_t address);

void memory_set_address_16b(Memory *memory, uint16_t address, uint16_t value);

#endif