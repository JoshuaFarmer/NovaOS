org 0x8000
start:
	nop
	mov al, 'A'
	mov ah, 0xE
	int 10h
retf