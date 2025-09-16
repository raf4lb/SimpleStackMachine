#ifndef SYS_H
#define SYS_H

#include <stdint.h>

void *vmmalloc(uint16_t size);

void *vmcalloc(uint16_t count, uint16_t size);

void vmfree(void *ptr, uint16_t size);

uint16_t get_memory_usage();

uint16_t get_local_vm_id();

#endif