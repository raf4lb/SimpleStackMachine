#include "sys.h"

uint16_t dynamic_memory_usage = 0;

void *vmmalloc(uint16_t size)
{
    void *ptr = malloc(size);
    if (ptr != NULL)
    {
        dynamic_memory_usage += size;
    }
    return ptr;
}

void vmfree(void *ptr)
{
    if (ptr != NULL)
    {
        // Determine the size of the memory block before freeing it
        // and subtract it from dynamic_memory_usage
        dynamic_memory_usage -= sizeof(*ptr);
        free(ptr);
    }
}

uint16_t get_memory_usage()
{
    return dynamic_memory_usage;
}

extern unsigned int __heap_start;
extern void *__brkval;

int freeMemory()
{
    int free_memory;
    if (__brkval == 0)
    {
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    }
    else
    {
        free_memory = ((int)&free_memory) - ((int)__brkval);
    }
    return free_memory;
}

int biggestMemoryBlock(uint16_t min, uint16_t max)
{
    if (min == max - 1)
        return min;

    int size = max;
    int lastSize = size;
    uint8_t *buf;
    while ((buf = (uint8_t *)malloc(size)) == NULL)
    {
        lastSize = size;
        size -= (max - min) / 2;
    };

    free(buf);
    return biggestMemoryBlock(size, lastSize);
};

int free_memory()
{
    return biggestMemoryBlock(0, 2048);
}