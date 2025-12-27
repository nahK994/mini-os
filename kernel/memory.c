#include "memory.h"

extern char _kernel_end;

static uint32_t free_memory_address;

void init_memory() {
    free_memory_address = (uint32_t)&_kernel_end;
}
