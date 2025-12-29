#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void init_memory();
void* kmalloc(uint32_t size);
void print_memory_stats();

#endif
