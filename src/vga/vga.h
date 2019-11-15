#ifndef VGA_H
#define VGA_H

#define VGA_BUFFER_ADDRESS 0xb8000

char vga_color_code(char fg, char bg);

void vga_print_str(const char* str, char color_code);
void vga_set_cursor(unsigned int x, unsigned int y);

#endif
