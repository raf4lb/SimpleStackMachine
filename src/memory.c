#include "memory.h"
#include "sys.h"
#include "io.h"

void memory_error_print(char *message)
{
    vmprintf("MemoryError: %s", message);
}

Memory *memory_create(uint16_t size)
{
    Memory *memory = (Memory *)vmmalloc(sizeof(Memory));
    if (memory == NULL)
    {
        memory_error_print("Memory allocation failed for Memory\n");
        exit(EXIT_FAILURE);
    }
    memory->size = size;
    uint8_t *data = (uint8_t *)vmmalloc(size * sizeof(uint8_t));
    if (data == NULL)
    {
        memory_error_print("Memory allocation failed for Memory->data\n");
        exit(EXIT_FAILURE);
    }
    // Initialize memory
    for (int i = 0; i < size; i++)
    {
        data[i] = 0;
    }
    memory->data = data;
    return memory;
}

void memory_free(Memory *memory)
{
    vmfree(memory->data);
    vmfree(memory);
}

void memory_print(Memory *memory)
{
    vmprintf("[");
    for (int i = 0; i < memory->size; i++)
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
