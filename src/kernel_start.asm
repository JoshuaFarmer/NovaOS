_kernel_start:
	pop dx
	call clear_scr
	push 0
	pop ds
	push 0x1000
	pop es

	mov ax, 0
	mov bx, 0
	mov cx, 0
	mov dx, 0
	
	KeyBoardBuffer = 0xe820

	mov ah, 09h
	mov cx, 0x1000
	mov al, 20h
	mov bl, 17h
	int 10h

	mov ah, 0x02
	int 10h

	jmp dat_t

clear_scr:
	pusha
	mov ah, 0x00
	mov al, 0x03  ; text mode 80x25 16 colours
	int 0x10
	popa
	ret