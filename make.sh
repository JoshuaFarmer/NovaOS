#!/bin/bash

# Find all .c and .h files and count lines
find . \( -name '*.c' -or -name '*.h' -or -name '*.s' -or -name '*.asm' \) -print0 | xargs -0 wc -l

# Compile assembly and C source files
clang -m32 -c "src/asm/boot.s" -o "bin/boot.o" -march=i386
nasm  -f elf32 "src/asm/interrupts.asm" -o "bin/interrupts.o"

# Kernel
clang -m32 -c -ffreestanding "src/C/kernel32.c" -o "bin/krnl32.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/io.c" -o "bin/io.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/ata.c" -o "bin/ata.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/malloc.c" -o "bin/malloc.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/math.c" -o "bin/math.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/string.c" -o "bin/string.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/sound.c" -o "bin/sound.o" -Wall -Wextra -march=i386 -I src/C/include

# FatFS
clang -m32 -c -ffreestanding "src/C/FatFS/diskio.c" -o "bin/diskio.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/FatFS/ff.c" -o "bin/ff.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/FatFS/ffsystem.c" -o "bin/ffsystem.o" -Wall -Wextra -march=i386 -I src/C/include
clang -m32 -c -ffreestanding "src/C/FatFS/ffunicode.c" -o "bin/ffunicode.o" -Wall -Wextra -march=i386 -I src/C/include

# Link the object files to create the binary
ld -m elf_i386 -T linker.ld -o bin/Nova.bin -O2 -nostdlib "bin/boot.o" "bin/interrupts.o" "bin/krnl32.o" "bin/io.o" "bin/ata.o" "bin/malloc.o" "bin/math.o" "bin/string.o" "bin/sound.o" "bin/diskio.o" "bin/ff.o" "bin/ffsystem.o" "bin/ffunicode.o"

# Check if the binary is multiboot compliant
if grub-file --is-x86-multiboot bin/Nova.bin; then
	echo "Multiboot confirmed"
	sh run.sh
	exit 0
else
	echo "The file is not multiboot"
	exit 1
fi