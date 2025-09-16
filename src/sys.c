#include <stdlib.h>

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

void *vmcalloc(uint16_t count, uint16_t size)
{
    void *ptr = calloc(count, size);
    if (ptr != NULL)
    {
        dynamic_memory_usage += size;
    }
    return ptr;
}

void vmfree(void *ptr, uint16_t size)
{
    dynamic_memory_usage -= size;
    free(ptr);
}

uint16_t get_memory_usage()
{
    return dynamic_memory_usage;
}

uint16_t get_local_vm_id()
{
    return 1;
}