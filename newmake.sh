#!/bin/bash

# Find all .c and .h files and count lines
find . \( -name '*.c' -or -name '*.h' \) -print0 | xargs -0 wc -l

# compile bootloader (not yet)

# Compile assembly and C source files
clang -m32 -c "src/asm/boot.s" -o "bin/boot.o" -march=i386
nasm  -f elf32 "src/asm/interrupts.asm" -o "bin/interrupts.o"
# kernel
clang -m32 -c -ffreestanding "src/C/kernel32.c" -o "bin/krnl32.o" -Wall -Wextra -march=i386 -I src/C/include

# Link the object files to create the flat binary
ld -m elf_i386 -T linker.ld -o bin/Nova.elf -O2 -nostdlib "bin/boot.o" "bin/interrupts.o" "bin/krnl32.o"

# Convert the ELF file to a flat binary
objcopy -O binary bin/Nova.elf bin/Nova.bin
