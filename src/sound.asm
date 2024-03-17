beep:
	mov ax, 4096
	mov cx, 0x001
	mov dx, 0x000
	call tone
ret

; ---------------------------------------------
; Generate tone with frequency specified in AX.
; The tone will last for CX:DX microseconds.
; For instance, CX=0, DX=F4240h will play the
; specified tone for one second.
; Registers preserved.
tone:
    PUSHA               ; Prolog: Preserve all registers
    MOV BX, AX          ; 1) Preserve the note value by storing it in BX.
    MOV AL, 182         ; 2) Set up the write to the control word register.
    OUT 43h, AL         ; 2) Perform the write.
    MOV AX, BX          ; 2) Pull back the frequency from BX.
    OUT 42h, AL         ; 2) Send lower byte of the frequency.
    MOV AL, AH          ; 2) Load higher byte of the frequency.
    OUT 42h, AL         ; 2) Send the higher byte.
    IN AL, 61h          ; 3) Read the current keyboard controller status.
    OR AL, 03h          ; 3) Turn on 0 and 1 bit, enabling the PC speaker gate and the data transfer.
    OUT 61h, AL         ; 3) Save the new keyboard controller status.
    MOV AH, 86h         ; 4) Load the BIOS WAIT, int15h function AH=86h.
    INT 15h             ; 4) Immidiately interrupt. The delay is already in CX:DX.
    IN AL, 61h          ; 5) Read the current keyboard controller status.
    AND AL, 0FCh        ; 5) Zero 0 and 1 bit, simply disabling the gate.
    OUT 61h, AL         ; 5) Write the new keyboard controller status.
    POPA                ; Epilog: Pop off all the registers pushed
    RET                 ; Epilog: Return.