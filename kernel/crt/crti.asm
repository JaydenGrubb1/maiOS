section .init
global _init
_init:
	push rbp
	mov rbp, rsp
	; Contents of crtbegin.o init section will be linked here

section .fini
global _fini
_fini:
	push rbp
	mov rbp, rsp
	; Contents of crtbegin.o fini section will be linked here