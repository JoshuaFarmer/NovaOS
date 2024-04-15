cd src
nasm "Bootloader.asm" -o "../bin/Load.bin"
nasm "Kernel.asm" -o "../bin/Kernel.bin"
cd ../bin
del COSA.img
del COSA.bin
copy /b Load.bin + Kernel.bin COSA.img
copy COSA.img COSA.bin
