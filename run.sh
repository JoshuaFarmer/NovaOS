#!/bin/bash

# Check if ISODIR is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <ISODIR>"
  exit 1
fi

ISODIR=$1
IMGFILE="output.img"
ISOFILE="rescue.iso"
SIZE=100M  # Adjust size as needed

# Create an empty disk image
dd if=/dev/zero of=$IMGFILE bs=1M count=$(echo $SIZE | sed 's/M//')

# Create a loop device
LOOPDEV=$(losetup -f --show $IMGFILE)

# Create a FAT32 filesystem
mkfs.vfat -F 32 $LOOPDEV

# Mount the loop device
MOUNTDIR=$(mktemp -d)
mount ${LOOPDEV}p1 $MOUNTDIR

# Copy the contents of ISODIR to the image
cp -r $ISODIR/* $MOUNTDIR

# Unmount and detach the loop device
umount $MOUNTDIR
losetup -d $LOOPDEV

# Clean up
rmdir $MOUNTDIR

# Create an ISO image from the ISODIR
grub-mkrescue -o $ISOFILE $1

echo "Created $IMGFILE and $ISOFILE from $ISODIR"

# Optionally, you can test the ISO with QEMU
qemu-system-i386 -m 256M -cdrom $ISOFILE -drive file=$IMGFILE,format=raw -boot d
