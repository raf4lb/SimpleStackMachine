#include <stdint.h>
#include <stdlib.h>

void *vmmalloc(uint16_t size);

void vmfree(void *ptr);

uint16_t get_memory_usage();

int16_t freeMemory();
int free_memory();