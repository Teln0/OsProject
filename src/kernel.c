#include "multiboot.h"
#include "strutils.h"

#include "vga/vga.h"
#include "mem/mem.h"

extern multiboot_uint64_t lnk_end_kernel;

typedef struct {
	multiboot_uint32_t sh_name;
	multiboot_uint32_t sh_type;
	multiboot_uint64_t sh_flags;
	multiboot_uint64_t sh_addr;
	multiboot_uint64_t sh_offset;
	multiboot_uint64_t sh_size;
	multiboot_uint32_t sh_link;
	multiboot_uint32_t sh_info;
	multiboot_uint64_t sh_addralign;
	multiboot_uint64_t sh_entsize;
} Elf64_Shdr;

void kernel_main(const multiboot_info_t* mbinfo)
{
    vga_print_str((char*) mbinfo->boot_loader_name, vga_color_code(0, 15));
    char str[64] = {0};

    // Parsing elf sections table
    unsigned long kernel_start, kernel_end;
    unsigned long multiboot_start, multiboot_end;
    multiboot_start = mbinfo;
    multiboot_end = ((void*) mbinfo) + sizeof(multiboot_info_t);
    Elf64_Shdr* section_header = (void*) mbinfo->u.elf_sec.addr;
    for (int i = 0; i < mbinfo->u.elf_sec.num; i++)
    {
        if (i == 1)
        {
            kernel_start = section_header[i].sh_addr;
        }
        // This code iterates through the elf section headers (Elf64_Shdr)
        // section_header is an array and i in the index of the element in the array
    }

    kernel_end = &lnk_end_kernel;

    init_memory(kernel_start, kernel_end, multiboot_start, multiboot_end, mbinfo);

    while(1);
}
