; testing, only for SOME bios's
mouse_start:
	mov ax, 0
	int 0x33

	mov ax, 1
	int 0x33

	mov bx, 0
	ret
read_mouse:
	mov ax, 3	   
	int 0x33

	mov [mousey], dx
	mov [mousex], cx
	ret

mousex dw 0
mousey dw 0