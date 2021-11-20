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
	
	; Do checks
	call check_multiboot
	call check_cpuid
	call check_ext_cpu_info
	call check_long_mode

	call clear_screen
	mov word [0xb8000], 0x1F4F	; O
	mov word [0xb8002], 0x1F4B	; K
test:
	hlt
	jmp test;

	; Print error 0 (Unexpected Kernel Exit)
	call clear_screen
	mov al, 0
	call print_error

; Halt the OS and loop if escapes
terminate:
	cli
	hlt
	jmp terminate

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
	jmp terminate

; Clears the screen by setting it to all blue
clear_screen:
	mov eax, 0
clear_screen.loop
	mov dword [0xb8000 + eax * 4], 0x11201120
	inc eax
	cmp eax, 1000
	jne clear_screen.loop
	ret

; Checks if the multiboot header is present
check_multiboot:
	cmp eax, 0x36d76289
	jne .no_multiboot
	ret
.no_multiboot:
	; Print error 1 (No Multiboot Present)
	call clear_screen
	mov al, 1
	jmp print_error

; Checks if the CPUID instruction is available
check_cpuid:
	pushfd
	pop eax
	mov ecx, eax
	xor eax, 1 << 21
	push eax
	popfd
	pushfd
	pop eax
	push ecx
	popfd
	cmp eax, ecx
	je .no_cpuid
	ret
.no_cpuid:
	; Print error 2 (No CPUID Instruction)
	call clear_screen
	mov al, 2
	jmp print_error

; Checks if extended CPU info is supported
check_ext_cpu_info:
	mov eax, 0x80000000
	cpuid
	cmp eax, 0x80000001
	jb .no_ext_info
	ret
.no_ext_info
	; Print error 3 (No Extended CPU Info)
	call clear_screen
	mov al, 3
	jmp print_error

; Check if the CPU supports long mode
check_long_mode
	mov eax, 0x80000001
	cpuid
	test edx, 1 << 29
	jz .no_long_mode
	ret
.no_long_mode:
	; Print error 4 (No Long Mode)
	call clear_screen
	mov al, 4
	jmp print_error

section .bss
; Reserve 16 KiB of memory for the stack
align 4096
stack_top:
	resb 4096 * 4
stack_bottom: