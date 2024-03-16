use16
nop
_start:
	mov di, msg
	mov ah, 0xE
	.loop:
		mov al, [es:di]
		inc di
		int 10h
		test al, al
		jne .loop
.end:
mov ax, 0x69
retf

.data:
	msg db "Hello, The World Of Cosa!", 13, 10, 0