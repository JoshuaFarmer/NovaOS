#!/bin/bash

# Find all .c and .h files and count lines
find . \( -name '*.c' -or -name '*.h' \) -print0 | xargs -0 wc -l

# Compile assembly and C source files
clang -m32 -c "src/asm/boot.s" -o "bin/boot.o" -march=i386
clang -m32 -c -ffreestanding "src/C/kernel32.c" -o "bin/krnl32.o" -Wall -Wextra -Werror -march=i386
clang -m32 -c -ffreestanding "src/C/diskio.c" -o "bin/diskio.o" -Wall -Wextra -march=i386
clang -m32 -c -ffreestanding "src/C/ffsystem.c" -o "bin/ffsystem.o" -Wall -Wextra -march=i386
clang -m32 -c -ffreestanding "src/C/ffunicode.c" -o "bin/ffunicode.o" -Wall -Wextra -march=i386

# Link the object files to create the binary
ld -m elf_i386 -T linker.ld -o bin/Nova.bin -O2 -nostdlib "bin/boot.o" "bin/krnl32.o" "bin/diskio.o" "bin/ffsystem.o" "bin/ffunicode.o"

# Check if the binary is multiboot compliant
if grub-file --is-x86-multiboot bin/Nova.bin; then
    echo "Multiboot confirmed"

    # Create directories for ISO contents
    mkdir -p isodir/boot/grub

    # Copy the binary and configuration files to the ISO directory
    cp bin/Nova.bin isodir/boot/Nova.bin
    cp grub.cfg isodir/boot/grub/grub.cfg
    cp a.txt isodir/a.txt

    # Create the ISO image
    grub-mkrescue -o bin/Nova.iso isodir

	qemu-img convert -O qcow2 bin/Nova.iso bin/Nova.qcow2

    # Optionally run the OS in QEMU
    qemu-system-x86_64 -m 256M -soundhw pcspk -debugcon stdio -drive file=bin/Nova.qcow2,index=0,if=ide,format=qcow2
else
    echo "The file is not multiboot"
fi
