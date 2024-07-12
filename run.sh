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
qemu-system-i386 -m 256M -soundhw pcspk -debugcon stdio -drive file=bin/Nova.qcow2,format=qcow2