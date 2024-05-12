qemu-img convert -f raw -O qcow2 bin/Nova.iso bin/Nova.qcow2
qemu-system-x86_64 -m -soundhw pcspk 256m -debugcon stdio -drive file=bin/Nova.qcow2,format=qcow2