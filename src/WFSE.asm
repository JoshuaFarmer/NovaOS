init_wfse:
    ; Load FS meta into memory
    mov ah, 2 ; read disk sectors function
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [wfse_fs_meta] ; sector number
    mov dh, 0 ; head number
    mov dl, 1 ; drive number
    mov bx, wfse_diskinfo_buffer ; buffer to read data into
    int 0x13 ; BIOS interrupt

    ; Parse FS start
    mov si, wfse_diskinfo_buffer
	add si, 7
	mov al, [si]
    mov [wfse_C], al ; load C from buffer
	sub si, 7

    ; Read file pointers
    mov ah, 0x02 ; read disk sectors function
    mov al, 1 ; number of sectors to read
    mov ch, 0 ; cylinder number
    mov cl, [wfse_fs_start] ; sector number (start from next sector after FS start)
    mov dh, 0 ; head number
    mov dl, 1 ; drive number
    mov bx, wfse_buffer ; buffer to read data into
    int 0x13 ; BIOS interrupt

    ret

wfse_ls:
	mov bx, 1
	call wfse_fetch
	ret

; get file data from pointer [bx]
wfse_fetch:
	call _wfse_get_file_meta
	mov si, wfse_buffer
	mov al, [si]
	mov [wfse_File_Len], al
	inc si
	ret
	; get meta data from pointer [bx]
_wfse_get_file_meta:
		call _wfse_get_pointer
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
_wfse_get_pointer:
		mov si, wfse_diskinfo_buffer

		; get the index of the pointer
		mov ax, bx
		mov bx, 4
		mul bx

		sub bx, 3 ; goto start pos
		
		add si, bx

		mov [wfse_ptr], si ; save ram location

		mov al, [si]
		mov [wfse_Dr],  al ; load Dr from pointer
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

wfse_Sp  rb 0 ; meta sector pointer            (8 bit)
wfse_Cl  rb 0 ; cylinder/track                 (8 bit)
wfse_Hd  rb 0 ; head                           (8 bit)
wfse_Dr  rb 0 ; drive                          (8 bit)

wfse_Drive rb 0 ; drive                          (8 bit)

wfse_File_Len    rb 0
wfse_File_Name:  times 24 rb 0

wfse_C   rb 0 ; number of file pointer sectors
wfse_ptr rw 0 ; file pointer                   (16 bit)
wfse_fs_meta  rb 4
wfse_fs_start rb 5

; 4kb per program sector
wfse_buffer_size = 8
wfse_buffer:
	times 512*wfse_buffer_size rb 0

wfse_diskinfo_buffer:
	times 512 rb 0

times 512*4-($-$$) db 0
db "WFSE V1", 0, 1, "test drive 1", 0 ; first FS sector
times 512*5-($-$$) db 0

db 1, 0, 0, 18
db 0, 0, 0, 0

times 512*6-($-$$) db 0

_hello:
	db 1, "hi.txt", 0
	times 512*7-($-$$) db 0
	db "hello world!", 0