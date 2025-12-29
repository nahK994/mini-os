#include "memory.h"
#include <stdint.h>

extern char _kernel_end;

#define ALIGNMENT 4
#define HEAP_SIZE (1024 * 1024)

static uint32_t align_up(uint32_t addr);

static uint32_t heap_start;
static uint32_t heap_end;
static uint32_t free_memory_address;

void init_memory(void) {
    heap_start = align_up((uint32_t)&_kernel_end);
    free_memory_address = heap_start;
    heap_end = heap_start + HEAP_SIZE;
}

static uint32_t align_up(uint32_t addr) {
    if (addr % ALIGNMENT == 0)
        return addr;

    return addr + (ALIGNMENT - (addr % ALIGNMENT));
}

void* kmalloc(uint32_t size) {
    uint32_t aligned_address = align_up(free_memory_address);
    uint32_t next_free = aligned_address + size;

    if (next_free > heap_end) {
        print("KERNEL PANIC: Out of memory!\n");
        while (1) { }
    }

    free_memory_address = next_free;
    return (void*)aligned_address;
}

void print_memory_stats() {
    uint32_t used = free_memory_address - heap_start;
    uint32_t total = heap_end - heap_start;
    uint32_t free = total - used;

    print("Heap start: ");
    print_hex(heap_start);
    print("\n");

    print("Heap end  : ");
    print_hex(heap_end);
    print("\n");

    print("Used      : ");
    print_hex(used);
    print(" bytes\n");

    print("Free      : ");
    print_hex(free);
    print(" bytes\n");
}
