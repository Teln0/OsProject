#--compiling--

#-kernel-
#src/kernel.c
echo "BUILDING KERNEL"
x86_64-elf-gcc -m64 -c src/kernel.c -o src/kernel.o -ffreestanding -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -std=gnu99 -O2 -Wall -Wextra -g
x86_64-elf-gcc -m64 -c src/vga/vga.c -o src/vga/vga.o -ffreestanding -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -std=gnu99 -O2 -Wall -Wextra -g
x86_64-elf-gcc -m64 -c src/strutils.c -o src/strutils.o -ffreestanding -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -std=gnu99 -O2 -Wall -Wextra -g
x86_64-elf-gcc -m64 -c src/mem/mem.c -o src/mem/mem.o -ffreestanding -z max-page-size=0x1000 -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -std=gnu99 -O2 -Wall -Wextra -g

#-loader-
echo "BUILDING LOADER"
nasm -f elf64 boot/bootstrap.asm -o boot/bootstrap.o
nasm -f elf64 boot/long_mode_init.asm -o boot/long_mode_init.o

#--linking--

#-kernel-

#-loader-
echo "LINKING KERNEL AND LOADER"
ld -n -o loader.bin -T link.ld boot/bootstrap.o boot/long_mode_init.o src/kernel.o src/vga/vga.o src/strutils.o src/mem/mem.o
rm -f loader_build/boot/kernel.bin
cp loader.bin loader_build/boot/kernel.bin

#--creating iso--
echo "CREATING ISO"
grub-mkrescue -o os.iso loader_build/
