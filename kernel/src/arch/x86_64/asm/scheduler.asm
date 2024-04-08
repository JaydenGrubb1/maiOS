section .text
bits 64

extern switch_context

global switch_thread
switch_thread:
	; pushed by cpu: ss, rsp, rflags, cs, rip
	; save current task
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rdi
	push rsi
	push rdx
	push rcx
	push rbx
	push rax
	push rbp
	push rsp

	; swap stack
	mov rdi, rsp
	call switch_context

	; load new task
	add rsp, 8 ; skip return address
	pop rbp
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
	
	; popped by cpu: rip, cs, rflags, rsp, ss
	iretq