wfse_diskinfo_buffer = 0x8000
wfse_ptr_buffer      = 0x81ff
wfse_buffer          = 0x83ff

; es:di = KeyboardBuffer (name)
wfse_get_file:
	push ds
	push es

	push wfse_ptr_buffer
	pop  ds

	; do shit here
	mov  bx, 1
	.loop:
		push 0x1000
		pop  es
		call .fetch_name
		inc  bx
		call .cmp
		cmp  ax, 1
		jne  .loop
	mov  di, 1
	push wfse_ptr_buffer
	pop  es
	call puts

	; restore shit
	pop  es
	pop  ds
ret
	.nvm:
		pop es
		pop ds
		mov di, wfse_file_not_found
		call puts
	ret

	.fetch_name:
		call _wfse_get_file_meta
		cmp  ax, 1
		je   .nvm

		push wfse_buffer
		pop  es

		mov  si, 1
	ret

	.cmp:
	; Compare strings in a loop
		.cmploop:
			; Load characters from each string
		mov al, [ds:SI]   ; Load byte at ES:SI into AL,
		mov bl, [es:DI]
		inc si
		inc di
		; Check for null terminator
		cmp al, 0
		je .cmpequal   ; Jump if null terminator is reached
		
		; Compare characters
		cmp al, bl
		jne .cmpnot_equal   ; Jump if characters are not equal
		
		; Continue loop
		jmp .cmploop
	.cmpnot_equal:
		; Strings are not equal
		mov ax, 0
	ret
	.cmpequal:
		; Strings are equal
		mov ax, 1
ret

init_wfse:
    ; Load FS meta into memory
    mov ah, 2 ; read disk sectors function
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [es:wfse_fs_meta] ; sector number
    mov dh, 0 ; head number
    mov dl, 0 ; drive number
    mov bx, wfse_diskinfo_buffer ; buffer to read data into
	mov es, bx
	xor bx, bx
    int 0x13 ; BIOS interrupt
	
    ; Parse FS start
    mov si, wfse_diskinfo_buffer
	add si, 7
	mov al, [es:si]
	push 0x1000
	pop  es
    mov [es:wfse_C], al ; load C from buffer
	sub si, 7
	
wfse_get_ptrs:
    ; Read file pointers
    mov ah, 0x02 ; read disk sectors function
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [es:wfse_fs_start] ; sector number (start from next sector after FS start)
    mov dh, 0 ; head number
    mov dl, 0 ; drive number
    mov bx, wfse_ptr_buffer ; buffer to read data into
	mov es, bx
	xor bx, bx
    int 0x13 ; BIOS interrupt

	push 0x1000
	pop  es
    ret

wfse_ls:
	mov bx, 0
	.loop:
		inc bx
		call wfse_print_name
		cmp  ax, 1x
		jne .loop
	ret

; get file data from pointer [bx]
wfse_print_name:
	call _wfse_get_file_meta
	cmp  ax, 1
	jmp  .nvm
	push wfse_buffer
	pop  es

	mov  si, 0
	mov  al, [es:si]
	push ax

	mov  di, 1
	call puts

	pop  ax

	push 0x1000
	pop  es
	
	mov al, 13
	call putc
	mov al, 10
	call putc

	mov  [wfse_File_Len], al
	ret
.nvm:
	mov ax, 1
	ret

; get meta data from pointer [bx]
_wfse_get_file_meta:
		call _wfse_get_pointer
		cmp [wfse_Sp], 0
		je  .nvm
		; Read file pointers
		mov ah, 2 ; read disk sectors function
		mov al, 1 ; number of sectors to read
		mov ch, [es:wfse_Cl]    ; cylinder number
		mov cl, [es:wfse_Sp]    ; sector number (start from next sector after FS start)
		mov dh, [es:wfse_Hd]    ; head number
		mov dl, [es:wfse_Drive] ; drive number
		
		mov bx, wfse_buffer ; buffer to read data into
		mov es, bx
		xor bx, bx
		int 0x13 ; BIOS interrupt

		mov  ax, 0
		push 0x1000
		pop  es
		ret
	.nvm:
		mov bx, 0x1000
		mov es, bx
		mov  ax, 1
	ret
; Parse file pointer at index [bx]
_wfse_get_pointer:
		push wfse_ptr_buffer
		pop  es
		
		; get the index of the pointer
		mov ax, bx
		mov bx, 4
		mul bx

		sub bx, 3 ; goto start pos

		mov si, bx

		push si

		mov al, [es:si]
		push ax
		inc si

		mov al, [es:si]
		push ax
		inc si
		
		mov al, [es:si]
		push ax
		inc si
		
		mov al, [es:si]
		push ax

		push 0x1000
		pop  es

		pop  si
		mov [es:wfse_ptr], si ; save ram location
		pop  ax
		mov [es:wfse_Sp],  al ; load Sp from pointer
		pop  ax
		mov [es:wfse_Cl],  al ; load Cl from pointer
		pop  ax
		mov [es:wfse_Hd],  al ; load Hd from pointer
		pop  ax
		mov [es:wfse_Dr],  al ; load Dr from pointer
		
		ret

wfse_meta:
	push wfse_diskinfo_buffer
	pop  es
	
	mov  di, 0
	
	mov  al, [es:di]
	inc  di
	cmp  al, 'W'
	jne  .error

	mov  al, [es:di]
	inc  di
	cmp  al, 'F'
	jne  .error

	mov  al, [es:di]
	inc  di
	cmp  al, 'S'
	jne  .error

	mov  al, [es:di]
	inc  di
	cmp  al, 'E'
	jne  .error

	push 0x1000
	pop  es

	mov di, wfse_start_msg
	call puts
ret
.error:
	mov di, wfse_error_msg	
	call puts
ret

wfse_Sp    db 0 ; meta sector pointer            (8 bit)
wfse_Cl    db 0 ; cylinder/track                 (8 bit)
wfse_Hd    db 0 ; head                           (8 bit)
wfse_Dr    db 0 ; drive                          (8 bit)
wfse_Drive db 0 ; drive                          (8 bit)

wfse_File_Len    db 0
wfse_File_Name:  times 24 db 0

wfse_C   db 0 ; number of file pointer sectors
wfse_ptr dw 0 ; file pointer                   (16 bit)
wfse_fs_meta  db 6
wfse_fs_start db 7

wfse_start_msg db "WFSE has been initialized", 13, 10, 0
wfse_error_msg db "WFSE Error 0x0: Failed to initialize WFSE", 13, 10, 0
wfse_file_not_found db "WFSE Error 0x1: File not found", 13, 10, 0

times 512*4-($-$$) db 0
db "WFSE V1", 0, 1, "COSA Drive", 0
times 512*5-($-$$) db 0 

db 1, 0, 0, 8

times 512*6-($-$$) db 0

_WFSECOSA:
	db 1, "COSA.sys", 0
	times 512*7-($-$$) db 0
	db "COSA is not actually accessible by WFSE, as it is located on sector 1", 0
	times 512*8-($-$$) db 0
