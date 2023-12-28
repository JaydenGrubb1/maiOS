; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

%define VIRT_BASE 0xffffffff80000000

global init64_start
extern gdt64.data
extern _start
extern _init
extern _fini

section .text
bits 64
init64_start:
	lea rax, init_high
	jmp rax

init_high:
	; Sets all data segment registers
	mov ax, gdt64.data
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	; Reset stack pointers
	mov rax, VIRT_BASE
	add rsp, rax
	xor rbp, rbp

	; Call the init function provided by gcc for constructing global objects
	call _init
	; Finally, go to main kernel function
	call _start
	; Call the fini function provided by gcc for deconstrucing global objects
	call _fini	; TODO Is this even necessary?

; Halt the OS and loop forever
terminate:
	cli
	hlt
	jmp terminate