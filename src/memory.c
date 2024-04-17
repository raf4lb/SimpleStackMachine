#include "memory.h"
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

void memory_error_print(char *message)
{
    vmprintf("MemoryError: %s", message);
}

Memory *memory_create(uint16_t size)
{
    Memory *memory = (Memory *)malloc(sizeof(Memory));
    if (memory == NULL)
    {
        memory_error_print("Memory allocation failed for Memory\n");
        exit(EXIT_FAILURE);
    }
    memory->size = size;
    uint8_t *data = (uint8_t *)malloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        memory_error_print("Memory allocation failed for Memory->data\n");
        exit(EXIT_FAILURE);
    }
    // Initialize memory
    int i;
    for (i = 0; i < size; i++)
    {
        data[i] = 0;
    }
    memory->data = data;
    return memory;
}

void memory_free(Memory *memory)
{
    free(memory->data);
    free(memory);
}

void memory_print(Memory *memory)
{
    vmprintf("[");
    int i;
    for (i = 0; i < memory->size; i++)
    {
        if (i > 0)
        {
            vmprintf(", ");
        }
        vmprintf("%u", memory->data[i]);
    }
    vmprintf("]\n");
}

uint8_t memory_get_address(Memory *memory, uint16_t address)
{
    if (0 <= address && address < memory->size)
    {
        return memory->data[address];
    }
    memory_error_print("memory_get_address: Memory address out of the range\n");
    exit(EXIT_FAILURE);
}

void memory_set_address(Memory *memory, uint16_t address, uint8_t value)
{
    if (0 <= address && address < memory->size)
    {
        memory->data[address] = value;
    }
    else
    {
        memory_error_print("memory_get_address: Memory address out of the range\n");
        exit(EXIT_FAILURE);
    }
}

uint16_t memory_get_address_16b(Memory *memory, uint16_t address)
{
    uint8_t h = memory_get_address(memory, address) << 8;
    uint8_t l = memory_get_address(memory, address + 1);
    uint16_t value = h | l;
    return value;
}

void memory_set_address_16b(Memory *memory, uint16_t address, uint16_t value)
{
    uint8_t h = value >> 8;
    memory_set_address(memory, address, h);
    uint8_t l = value & 255;
    memory_set_address(memory, address + 1, l);
}

#ifdef ARDUINO
#include "Arduino.h"
// Function to get the free memory on Arduino
unsigned int getFreeMemory()
{
    // Start of the heap (as defined by the linker)
    extern unsigned int __heap_start;

    // Current position of the heap
    unsigned int current_sp = (unsigned int)&current_sp;

    // Free memory calculation
    unsigned int free_memory;
    if (__heap_start == 0)
    {
        // If the heap start address is not defined, return 0
        free_memory = 0;
    }
    else
    {
        // Calculate free memory
        free_memory = current_sp - (unsigned int)&__heap_start;
    }
    return free_memory;
}

#endif