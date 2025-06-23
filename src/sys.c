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

uint16_t get_local_vm_id(){
    return 1;
}