#include "memory.h"

extern char _kernel_end;

static uint32_t free_memory_address;

#define ALIGNMENT 4

static uint32_t align_up(uint32_t addr) {
    if (addr % ALIGNMENT == 0)
        return addr;

    return addr + (ALIGNMENT - (addr % ALIGNMENT));
}


void init_memory() {
    free_memory_address = (uint32_t)&_kernel_end;
}

void* kmalloc(uint32_t size) {
    uint32_t aligned_address = align_up(free_memory_address);
    free_memory_address = aligned_address + size;
    return (void*)aligned_address;
}
