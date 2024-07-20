#!/bin/bash

set -e

# Define the disk image size and name
IMAGE_SIZE_MB=1024
IMAGE_NAME="disk_image.img"
MOUNT_DIR="mnt"
GRUB_CFG="grub.cfg"

if [ -e $IMAGE_NAME ]; then
	rm $IMAGE_NAME
fi

# Create a 512MB disk image
dd if=/dev/zero of=$IMAGE_NAME bs=1M count=$IMAGE_SIZE_MB

# Create a partition table
parted $IMAGE_NAME --script mklabel msdos
parted $IMAGE_NAME --script mkpart primary fat32 1MiB 100%

# Associate the image with a loop device
LOOP_DEVICE=$(losetup --show -fP $IMAGE_NAME)

# Create a FAT32 filesystem on the partition
mkfs.vfat -F 32 ${LOOP_DEVICE}p1

# Mount the partition
mkdir -p $MOUNT_DIR
mount ${LOOP_DEVICE}p1 $MOUNT_DIR

# Install GRUB
grub-install --target=i386-pc --boot-directory=$MOUNT_DIR/boot --no-floppy --modules="part_msdos" ${LOOP_DEVICE}

# Create a grub.cfg file
cat > $GRUB_CFG << EOF
set timeout=5
set default=0
set gfxmode=640x480

menuentry "Supernova OS" {
	multiboot /boot/Nova.bin
	boot
}
EOF

# Copy the grub.cfg to the boot directory
mkdir -p $MOUNT_DIR/boot/grub
cp $GRUB_CFG $MOUNT_DIR/boot/grub/grub.cfg

# Copy the Nova binary to the boot directory
cp "bin/Nova.bin" "$MOUNT_DIR/boot/Nova.bin"

# Unmount and detach the loop device
umount $MOUNT_DIR
losetup -d $LOOP_DEVICE

# Clean up
rmdir $MOUNT_DIR

echo "GRUB installed to $IMAGE_NAME successfully."

# Launch the disk image with QEMU
qemu-system-i386 -m 256M -drive file=$IMAGE_NAME,format=raw,if=ide,index=0,media=disk
