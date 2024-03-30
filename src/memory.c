#include "memory.h"
#include <stdlib.h>
#include <stdint.h>

void memory_error_print(char *message)
{
    printf("MemoryError: %s", message);
}

Memory *memory_create(int size)
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
    printf("[");
    int i;
    for (i = 0; i < memory->size; i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printf("%u", memory->data[i]);
    }
    printf("]\n");
}

uint8_t memory_get_address(Memory *memory, uint16_t address)
{
    if (0 <= address && address < memory->size)
    {
        return memory->data[address];
    }
    memory_error_print("memory_get_address: Memory address %d out of the range\n");
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
