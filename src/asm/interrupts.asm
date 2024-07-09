global SYSIN80
global default_exception_handler

extern SYS80
extern add_window
extern create_text_element

section .text

SYSIN80:
	cli
	pusha

	cmp eax, 0x0 ; default
	je  .none
	cmp eax, 0x1 ; create window
	je  .createwindow
	cmp eax, 0x2 ; create text element
	je .createtext
.exit:
	popa
.exit2:
	sti
	iret
.none:
	call SYS80
	jmp .exit
.createwindow:
	mov edx, ebx
	call add_window
	jmp .exit
.createtext:
	push edx
	push esi
	push cx
	push bx
	call create_text_element
	add esp, 16
	jmp .exit


default_exception_handler:
	cli
	hlt
	jmp $
	iret
