global system_interrupt80_handler
global system_interrupt0_handler
global keyboard_interrupt_handler
global default_exception_handler

extern _syscall
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

; yes
system_interrupt80_handler:
	call _syscall
	iret

default_exception_handler:
	cli
	pusha
	call default_exception
	popa
	sti
	iret
	