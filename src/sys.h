#ifndef SYS_H
#define SYS_H

#include <stdint.h>
#include <stdlib.h>

void *vmmalloc(uint16_t size);

void vmfree(void *ptr);

uint16_t get_memory_usage();

int free_memory();

#endif