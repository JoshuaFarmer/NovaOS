global system_interrupt80_handler
global system_interrupt0_handler
global keyboard_interrupt_handler
global default_exception_handler

extern system_interrupt80
extern system_interrupt0
extern default_exception
extern getc

extern add_window
extern create_text_element

section .text

keyboard_interrupt_handler:
	cli
	pusha
	call getc
	popa
	sti
	iret

system_interrupt0_handler:
	cli
	pusha
	call system_interrupt0
	popa
	sti
	iret

system_interrupt80_handler:
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
	call system_interrupt80
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
	pusha
	call default_exception
	popa
	sti
	iret