; di = dest
; si = name

_fetch_file:
	mov  [drive_num], dl
	; we need to load the first sector (the disk-meta sector
	mov	 ah, 02h ; read function.
	mov	 al, 1   ; sectors to read.
	mov	 cl, 1   ; sector.
	mov	 ch, 0   ; cylinder.
	mov	 dh, 0   ; head.
	
	; es:bx points to receiving
	;  data buffer:
	mov	 bx, 0xF000
	mov	 es, bx
	mov	 bx, 0
	
	; read!
	int	 13h

	mov  di, 0
	.loop:
		mov al, [es:di]
		inc di
		mov ah, 0xE
		int 10h
		test al, al
		jnz .loop

	mov	 bx, 0x1000
	mov	 es, bx
	mov	 bx, 0
	jmp $

drive_num db 0
File_Sector_Count dd 0