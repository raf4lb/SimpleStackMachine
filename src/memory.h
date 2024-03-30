#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stdio.h>

typedef struct
{
    uint8_t *data; // array of 8 bits
    int size;
} Memory;

Memory *memory_create(int size);

void memory_free(Memory *memory);

void memory_print(Memory *memory);

uint8_t memory_get_address(Memory *memory, uint16_t address);

void memory_set_address(Memory *memory, uint16_t address, uint8_t value);

#endif