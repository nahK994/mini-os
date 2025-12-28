#include "memory.h"

extern char _kernel_end;

static uint32_t free_memory_address;

void init_memory() {
    free_memory_address = (uint32_t)&_kernel_end;
}

void* kmalloc(uint32_t size) {
    uint32_t allocated_address = free_memory_address;
    free_memory_address += size;
    return (void*)allocated_address;
}
