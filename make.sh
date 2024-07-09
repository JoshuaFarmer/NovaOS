#!/bin/bash

# Find all .c and .h files and count lines
find . \( -name '*.c' -or -name '*.h' \) -print0 | xargs -0 wc -l

# Compile assembly and C source files
clang -m32 -c "src/asm/boot.s" -o "bin/boot.o" -march=i386
nasm  -f elf32 "src/asm/interrupts.asm" -o "bin/interrupts.o"
# kernel
clang -m32 -c -ffreestanding "src/C/kernel/kernel32.c" -o "bin/krnl32.o" -Wall -Wextra -Werror -march=i386 -I src/C/include

# FS
#clang -m32 -c -ffreestanding "src/C/kernel/diskio.c" -o "bin/diskio.o" -Wall -Wextra -march=i386 -I src/C/include
#clang -m32 -c -ffreestanding "src/C/kernel/ff.c" -o "bin/ff.o" -Wall -Wextra -march=i386 -I=src/C/include
#clang -m32 -c -ffreestanding "src/C/kernel/ffsystem.c" -o "bin/ffsystem.o" -Wall -Wextra -march=i386 -I src/C/include
#clang -m32 -c -ffreestanding "src/C/kernel/ffunicode.c" -o "bin/ffunicode.o" -Wall -Wextra -march=i386 -I src/C/include

# Link the object files to create the binary
ld -m elf_i386 -T linker.ld -o bin/Nova.bin -O2 -nostdlib "bin/boot.o" "bin/interrupts.o" "bin/krnl32.o"

# Check if the binary is multiboot compliant
if grub-file --is-x86-multiboot bin/Nova.bin; then
	echo "Multiboot confirmed"
else
	echo "The file is not multiboot"
fi
