section .text
bits 64

extern scheduler_swap
extern scheduler_tick

global scheduler_preempt
scheduler_preempt:
	call scheduler_tick
	; fallthrough to scheduler_yield

global scheduler_yield
scheduler_yield:
	; pushed by cpu: ss, rsp, rflags, cs, rip
	; save current thread
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

	; swap thread context
	mov rdi, rsp
	call scheduler_swap

	; load new thread
	add rsp, 8 ; discard current stack frame (scheduler_preempt/scheduler_yield)
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