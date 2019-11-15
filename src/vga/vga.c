#include "vga.h"

unsigned int vga_screenX = 80, vga_screenY = 25;
unsigned int vga_cursorX = 0, vga_cursorY = 0;

char vga_color_code(char fg, char bg)
{
    return ((fg) << 4 | (bg));
}

void vga_print_str(const char* str, char color_code)
{
    for (int i = 0; str[i] != 0; i++)
    {
        unsigned char* vga_buffer = (unsigned char*) VGA_BUFFER_ADDRESS;

        vga_buffer[(i + vga_cursorX + vga_cursorY * vga_screenX) * 2] = str[i];
        vga_buffer[(i + vga_cursorX + vga_cursorY * vga_screenX) * 2 + 1] = color_code;
    }

    vga_cursorY++;
}

void vga_set_cursor(unsigned int x, unsigned int y)
{
        vga_cursorX = x;
        vga_cursorY = y;
}
