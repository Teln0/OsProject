#ifndef MEM_H
#define MEM_H

#include "../multiboot.h"

#include <stdint.h>

#define PAGE_SIZE 4096

uint64_t kernel_start_address;
uint64_t kernel_end_address;
uint64_t multiboot_start_address;
uint64_t multiboot_end_address;
uint8_t* memory_map_start;
uint8_t* memory_map_end;
uint64_t memory_map_size;

void init_memory(
    uint64_t kernel_start_address_a,
    uint64_t kernel_end_address_a,
    uint64_t multiboot_start_address_a,
    uint64_t multiboot_end_address_a,
    multiboot_info_t* mbinfo
); // Initialize memory.
uint8_t* allocate_frame(uint64_t allocation_size); // Allocates a new frame and returns it's index.

#endif
