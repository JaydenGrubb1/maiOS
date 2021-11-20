; @file kernel/boot/init32.asm
; @author Jayden Grubb (contact@jaydengrubb.com)
; @date 2021-11-20
; @brief This is the main entry point for the entire operating system,
;	at this point the os is operating in 32-bit protected mode
; 
; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

global init32_start
; extern init64_start

section .text
bits 32
; Kernel entry point
init32_start:
	; Set the stack pointer
	mov esp, stack_top

	; TODO Do stuff here

	; Print error 0 (Unexpected Kernel Exit)
	call clear_screen
	mov al, 0
	call print_error

; Halt the OS and loop if escapes
init32_start.loop
	cli
	hlt
	jmp init32_start.loop

; Prints the word phrase "Error: X"
; where X is the error number specified
; in the AL register
print_error:
	mov word [0xb8000], 0x1F45	; E
	mov word [0xb8002], 0x1F72	; r
	mov word [0xb8004], 0x1F72	; r
	mov word [0xb8006], 0x1F6f	; o
	mov word [0xb8008], 0x1F72	; r
	mov word [0xb800a], 0x1F3a	; :

	add al, 0x30				; Add ASCII value for '0'
	mov byte [0xb800e], al		; ASCII value
	mov byte [0xb800f], 0x1F	; Blue background, white foreground
	ret

; Clears the screen by setting it to all blue
clear_screen:
	mov eax, 0
clear_screen.loop
	mov dword [0xb8000 + eax * 4], 0x11201120
	inc eax
	cmp eax, 1000
	jne clear_screen.loop
	ret

section .bss
; Reserve 16 KiB of memory for the stack
align 4096
stack_top:
	resb 4096 * 4
stack_bottom: