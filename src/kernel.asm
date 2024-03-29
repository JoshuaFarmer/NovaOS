KeyboardBuffer = 0xe820

include "kernel_start.asm"
include "kernel_data.asm"
call beep

call init_wfse
call wfse_meta
call wfse_ls

call mouse_start
jmp kernel_mainloop

splash:
	mov di, SYS_desc
	call puts
ret

putc:
	pusha
	mov ah, 0xE
	mov bh, 0
	mov bl, [es:SYS_colour]
	int 10h
	popa
ret	

puts:
	mov al, [es:di]
	cmp al, 0
	jne .next
ret
	.next:
		call putc
		inc di
		jmp puts
	
getc:	
	mov ah, 0
	int 16h
ret

gets:
	call getc
	call putc
	cmp al, 8
	je .backspace
	cmp al, 13
	je .end
	mov [es:di], al
	inc di
	jmp gets
	.end:
		mov ah, 0xE
		mov al, 10
		int 10h
ret		
.backspace:
	mov al, 20h
	call putc
	mov al, 8	
	call putc
	
	dec di
	mov byte[es:di], 0
	jmp gets

; si = source
; di = destination
strCpy:
; Compare strings in a loop
	.loop:
		; Load characters from each string
	mov al, [es:si]   ; Load byte at ES:SI into AL,
	mov [es:di], al
	inc si
	inc di
	; Check for null terminator
	cmp al, 0
	je .done   ; Jump if null terminator is reached
	
	; Continue loop
	jmp .loop
.done:
	ret
	
strCmp:
; Compare strings in a loop
	.loop:
		; Load characters from each string
	mov al, [es:SI]   ; Load byte at ES:SI into AL,
	mov bl, [es:DI]
	inc si
	inc di
	; Check for null terminator
	cmp al, 0
	je .equal   ; Jump if null terminator is reached
	
	; Compare characters
	cmp al, bl
	jne .not_equal   ; Jump if characters are not equal
	
	; Continue loop
	jmp .loop
.not_equal:
	; Strings are not equal
	mov ax, 0
ret
.equal:
	; Strings are equal
	mov ax, 1
ret

clearBuffer:	
	mov di, KeyboardBuffer	
	.loop:
	mov byte[es:di], 0
	inc di
	cmp byte[es:di], 0
	jne .loop
ret

_INST_desc:
	call splash
jmp kernel_mainloop

_INST_help:
	mov di, SYS_help_text
	call puts
jmp kernel_mainloop

_INST_user:
	call clearBuffer
	mov di, DAT_user_prompt
	call puts	
	mov di, SYS_user		
	call gets	
	dec di
	mov byte[es:di], 20h
jmp kernel_mainloop

_INST_disk:	
	mov	 ah, 02h ; read function.
	mov	 al, 10  ; sectors to read.
	mov	 ch, 0   ; cylinder.
	mov	 cl, 1   ; sector.
	mov	 dh, 0   ; head.
	mov	 dl, 1   ; drive.
	
	; es:bx points to receiving
	;  data buffer:
	mov	 bx, 0x8000
	mov	 es, bx
	mov	 bx, 0
	
	; read!
	int	 13h

	mov	 bx, 0x1000
	mov	 es, bx
	mov	 bx, 0
jmp kernel_mainloop

_INST_run:
	mov al, 0x90
	push 0x8000
	pop es
	mov si, 0
	
	cmp [es:si], al
	je .run_success
	.run_error:
		push 0x1000
		pop es
		mov di, DAT_run_error
		call puts
	jmp kernel_mainloop
	.run_success:
		mov bx, 0x8000
		mov ds, bx
		mov es, bx
		call 0x8000:0x0000
	.run_end:
		push 0x1000
		pop  es
		push 0x0
		pop  ds
		
		mov [es:DAT_exit_code], ax
		mov di, DAT_exit_code_msg
		call puts

		mov ax, [es:DAT_exit_code]
		call hex_to_string
		push ax
		call putc
		pop ax
		mov  al, ah
		call putc

		mov di, DAT_new_line
		call puts
	
	jmp kernel_mainloop

_INST_cls:
	call clear_scr
	int 10h
jmp kernel_mainloop

kernel_mainloop:
	call clearBuffer
	call read_mouse

	mov di, SYS_user
	call puts
			
	mov di, SYS_prompt
	call puts
	
	; get user input
	mov di, KeyboardBuffer
	call gets	
		
	; help command
	mov si, INST_help
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_help
	
	; splash screen command
	mov si, INST_desc
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_desc
	
	; change user command
	mov si, INST_user
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_user
	
	; load disk
	mov si, INST_disk
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_disk
	
	; run
	mov si, INST_run
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_run

	; clear command
	mov si, INST_cls
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_cls

	; check if the user wants to edit
	mov si, INST_monitor
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1
	je _INST_monitor

	; check if the user wants to reboot
	mov si, INST_reboot
	mov di, KeyboardBuffer
	call strCmp
	cmp ax, 1	
	je reboot
	
	jmp kernel_mainloop
reboot:
	db 0x0ea
	dw 0x0000
	dw 0xffff

_INST_monitor:
	.init:
		push 0x8000
		pop  es
		mov  di, 0
	.loop:
		call getc

		cmp al, 'Q'
		je .exit
		cmp al, 'q'
		je .exit

		cmp al, 'I'
		je .insert
		cmp al, 'i'
		je .insert

		cmp al, 'V'
		je .view
		cmp al, 'v'
		je .view

		cmp al, 'C'
		je .clear
		cmp al, 'c'
		je .clear

		cmp al, 0x8 ; backspace
		je .backspace

		jmp .loop
	.clear:
		call clear_scr
		jmp .loop
	.view:
		mov cx, 128
		mov si, 0
		.viewloop:
			mov al, [es:si]
			inc si
			call hex_to_string

			push ax
			call putc
			pop ax
			mov  al, ah
			call putc
			mov al, ' '
			call putc

			loop .viewloop

		jmp .loop
	.backspace:
		mov cx, 3
		.backspace.loop:
			mov al, 8	; one backspace
			call putc
			mov al, 20h
			call putc
			mov al, 8	
			call putc
		loop .backspace.loop
		dec di
		jmp .loop
	.insert:
		mov  al, ' '
		call putc
		call get_hex_value
		mov [es:di], bl
		inc  di
		
		jmp .loop
	.exit:
		push 0x1000
		pop  es
		mov  di, DAT_new_line
		call puts
		jmp kernel_mainloop

	; Function to convert a hex character to its corresponding value
	hex_to_value:
		cmp al, '0'
		jl  .not_hex
		cmp al, '9'
		jle .hex_digit
		cmp al, 'A'
		jl  .not_hex
		cmp al, 'F'
		jg  .not_hex
		sub al, 'A'
		add al, 10
		ret
	.hex_digit:
		sub al, '0'
		ret
	.not_hex:
		mov al, 'e'
		ret

; Main function to get a hexadecimal value from the user
get_hex_value:
	.init:
		xor bx, bx
	.read_loop:
		call getc
		cmp  al, 0x0D
		je   .end_input
		push ax
		call hex_to_value
		cmp  al, 'e'
		pop  dx
		je   .read_loop
		mov  cl, 4
		shl  bl, cl
		or   bl, al
		mov  ax, dx
		call putc
		jmp .read_loop
	.end_input:
		ret

hex_to_string:
	mov ah, 0
	mov bl, al
	shr al, 4
	call .convert_to_ascii
	mov ah, al
	mov al, bl
	and al, 0x0F
	call .convert_to_ascii
	
	mov  dl, al
	mov  al, ah
	mov  ah, dl
	ret

	.convert_to_ascii:
		cmp al, 9
		jbe .less_than_9
		add al, 'A' - 10
		ret
	.less_than_9:
		add al, '0'
		ret

; al
_print_hex:
	push ax
	call putc
	pop ax
	mov  al, ah
	call putc
	mov al, ' '
	call putc
	call hex_to_string
ret

; sound
include "kernel_sound.asm"

; Mouse
include "kernel_mouse.asm"

; WFSE
include "wfse.asm"
