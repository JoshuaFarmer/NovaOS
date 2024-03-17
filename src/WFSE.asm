init_wfse:
    ; Load FS start into memory
    mov ah, 2 ; read disk sectors function
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [wfse_fs_meta] ; sector number
    mov dh, 0 ; head number
    mov dl, 0 ; drive number
    mov bx, wfse_buffer ; buffer to read data into
    int 0x13 ; BIOS interrupt

    ; Parse FS start
    mov si, wfse_buffer
	mov al, [si + 8]
    mov [wfse_C], al ; load C from buffer

    ; Read file pointers
    mov ah, 0x02 ; read disk sectors function
    mov al, [wfse_C] ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [wfse_fs_start] ; sector number (start from next sector after FS start)
    mov dh, 0 ; head number
    mov dl, 0 ; drive number
    mov bx, wfse_buffer ; buffer to read data into
    int 0x13 ; BIOS interrupt

    ret

; get file data from pointer [bx]
wfse_fetch:
	call .wfse_get_file_meta
	ret
	; get meta data from pointer [bx]
	.wfse_get_file_meta:
		call .wfse_get_pointer
		; Read file pointers
		mov ah, 2 ; read disk sectors function
		mov al, 1 ; number of sectors to read
		mov ch, [wfse_Cl] ; cylinder number
		mov cl, [wfse_Sp] ; sector number (start from next sector after FS start)
		mov dh, [wfse_Hd] ; head number
		mov dl, [wfse_Dr] ; drive number
		mov bx, wfse_buffer ; buffer to read data into
		int 0x13 ; BIOS interrupt
		ret
		; Parse file pointer at index [bx]
	.wfse_get_pointer:
		mov si, wfse_buffer

		; get the index of the pointer
		shl bx, 2 ; multiply by 4
		sub bx, 3 ; goto start pos
		
		add si, bx

		mov [wfse_ptr], si ; save ram location

		mov al, [si]
		mov [wfse_Dr],  al ; load Hd from pointer
		inc si

		mov al, [si]
		mov [wfse_Hd],  al ; load Hd from pointer
		inc si
		
		mov al, [si]
		mov [wfse_Cl],  al ; load Cl from pointer
		inc si
		
		mov al, [si]
		mov [wfse_Sp],  al ; load Sp from pointer

		ret

wfse_Sp  db 0 ; meta sector pointer            (8 bit)
wfse_Cl  db 0 ; cylinder/track                 (8 bit)
wfse_Hd  db 0 ; head                           (8 bit)
wfse_Dr  db 0 ; drive                          (8 bit)

wfse_C   db 0 ; number of file pointer sectors
wfse_ptr dw 0 ; file pointer                   (16 bit)
wfse_fs_meta  db 17
wfse_fs_start db 18

; 4kb per program sector
wfse_buffer_size = 8
wfse_buffer:
	times 512*wfse_buffer_size db 0

times 512*16-($-$$) db 0
db "WFSE V1", 0, 8