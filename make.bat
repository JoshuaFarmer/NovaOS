cd src
nasm "Bootloader.asm" "../bin/Load.bin"
nasm "Kernel.asm" "../bin/Kernel.bin"
cd ../bin
del COSA.img
del COSA.bin
copy /b Load.bin + Kernel.bin COSA.img
copy COSA.img COSA.bin
