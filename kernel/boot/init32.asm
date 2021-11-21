; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

global init32_start
extern init64_start

section .text
bits 32
; Kernel entry point
init32_start:
	; Set the stack pointer
	mov esp, stack_top
	
	; Do CPU checks
	call check_multiboot
	call check_cpuid
	call check_ext_cpu_info
	call check_long_mode

	; Setup and enable paging (1 GiB identity mapped)
	call init_pages
	call enable_pages

	; Load GDT and jump to 64-bit code
	lgdt [gdt64.pointer]
	jmp gdt64.code:init64_start

	; Print error 0 (Unexpected Kernel Exit)
	call clear_screen
	mov al, 0
	jmp print_error


; Halt the OS and loop if escapes
terminate:
	cli
	hlt
	jmp terminate


; Initializes the page tables with the first
; 1 GiB of memory identity mapped
init_pages:
	; Setup level 4 page table
	mov eax, l3_page_table
	or eax, 0b11	; writeable, present
	mov [l4_page_table], eax
	; Setup level 3 page table
	mov eax, l2_page_table
	or eax, 0b11	; writeable, present
	mov [l3_page_table], eax
	; Setup 512 level 2 huge page tables
	mov ecx, 0
.loop:
	mov eax, 0x200000	; 2MiB
	mul ecx
	or eax, 0b10000011	; huge page, writeable, present
	mov [l2_page_table + ecx * 8], eax
	inc ecx
	cmp ecx, 512
	jne .loop
	ret


; Enable paging with page table initialized in init_pages
enable_pages:
	; Set page table location
	mov eax, l4_page_table
	mov cr3, eax
	; Enable physical address extension (PAE)
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax
	; Enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr
	; Enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax
	; Done
	ret


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
.loop:
	mov dword [0xb8000 + eax * 4], 0x11201120
	inc eax
	cmp eax, 1000
	jne .loop
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
.no_ext_info:
	; Print error 3 (No Extended CPU Info)
	call clear_screen
	mov al, 3
	jmp print_error


; Check if the CPU supports long mode
check_long_mode:
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
align 4096

; Reserve 12 KiB of memory for the initial page tables
l4_page_table:
	resb 8 * 512
l3_page_table:
	resb 8 * 512
l2_page_table:
	resb 8 * 512

; Reserve 16 KiB of memory for the stack
stack_top:
	resb 1024 * 16
stack_bottom:

section .rodata

; Define the GDT, it's not really used in 64-bit mode,
; but still needs to be defined, has the below structure
; 0x0000000000000000	->	Zero entry always required
; 0x0020980000000000	->	Code segment (everything else)
gdt64:
	dq 0												; Zero entry
.code: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)	; Code segment entry
	; executable, code type, present, 64-bit
.pointer:				; Value used by LGDT
	dw $ - gdt64 - 1	; Length of GDT
	dq gdt64			; Address of GDT