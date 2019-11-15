#include "mem.h"

#include "../vga/vga.h"

void set_bit(uint8_t* A, uint64_t k)
{
    A[k/8] |= 1 << (k%8);
}
void clear_bit(uint8_t* A, uint64_t k)
{
    A[k/8] &= ~(1 << (k%8));
}
uint8_t get_bit(uint8_t* A, uint64_t k)
{
    return (A[k/8] & (1 << (k%8)));
}

uint64_t kernel_start_address = 0;
uint64_t kernel_end_address = 0;
uint64_t multiboot_start_address = 0;
uint64_t multiboot_end_address = 0;

void init_memory(
    uint64_t kernel_start_address_a,
    uint64_t kernel_end_address_a,
    uint64_t multiboot_start_address_a,
    uint64_t multiboot_end_address_a,
    multiboot_info_t* mbinfo)
{
    kernel_start_address = kernel_start_address_a;
    kernel_end_address = kernel_end_address_a;
    multiboot_start_address = multiboot_end_address_a;
    multiboot_end_address = multiboot_end_address_a;
    memory_map_start = kernel_end_address + 8;

    uint64_t highest_available_address = 0;

    // Iterate through the memory map a first time to get the maximum amount of memory
    multiboot_memory_map_t* mmap_entry = (void*) mbinfo->mmap_addr;
    while(mmap_entry < (void*)(mbinfo->mmap_addr + mbinfo->mmap_length))
    {
        // This code iterates through the memory map provided by grub
        // mmap_entry is the pointer to the current entry

        if (mmap_entry->type == 1)
        {
            if (mmap_entry->addr + mmap_entry->len > highest_available_address)
            {
                highest_available_address = mmap_entry->addr + mmap_entry->len;
            }
        }

        mmap_entry = (multiboot_memory_map_t *)((void*)mmap_entry + mmap_entry->size + 4);
    }

    memory_map_size = highest_available_address / PAGE_SIZE;
    memory_map_end = memory_map_start + memory_map_size / 8;

    for (uint64_t i = 0; i < memory_map_size / 8 + 1; i++)
    {
        memory_map_start[i] = 0;
    }

    // Iterate through the memory map a second time to get the unaccessible regions of memory
    mmap_entry = (void*) mbinfo->mmap_addr;
    while(mmap_entry < (void*)(mbinfo->mmap_addr + mbinfo->mmap_length))
    {
        // This code iterates through the memory map provided by grub
        // mmap_entry is the pointer to the current entry

        if (mmap_entry->type != 1)
        {
            uint64_t address = mmap_entry->addr;
            uint64_t length = mmap_entry ->len;

            for (uint64_t i = 0; i < length / PAGE_SIZE + 1; i++)
            {
                uint64_t frame = address / PAGE_SIZE + i;
                set_bit((void*)memory_map_start, frame);
            }
        }

        mmap_entry = (multiboot_memory_map_t *)((void*)mmap_entry + mmap_entry->size + 4);
    }

    // Mark regions used by the kernel, the multiboot info structure and the memory map as used
    for (uint64_t i = kernel_start_address; i < memory_map_end + 1; i += PAGE_SIZE) // Kernel and memory map are right next to each other.
    {
        uint64_t frame = i / PAGE_SIZE;
        set_bit((void*)memory_map_start, frame);
    }
    for (uint64_t i = multiboot_start_address; i < multiboot_end_address + 1; i += PAGE_SIZE)
    {
        uint64_t frame = i / PAGE_SIZE;
        set_bit((void*)memory_map_start, frame);
    }
}


uint8_t* allocate_frame(uint64_t allocation_size) // The allocation size is in frames. Returns the address of a free frame.
{
    for (uint64_t i = 0; i < memory_map_size; i++)
    {
        int is_space_unallocated = 1;
        for (uint64_t frame = 0; frame < allocation_size; frame ++)
        {
            if (get_bit((void*)memory_map_start, i + frame) != 0)
            {
                is_space_unallocated = 0;
            }
        }
        if (is_space_unallocated == 1)
        {
            for (uint64_t frame = 0; frame < allocation_size; frame ++)
            {
                char str[64] = {0};
                set_bit((void*)memory_map_start, i + frame);
            }
            return (uint8_t*) (i * PAGE_SIZE);
        }
    }
    return 0;
}
