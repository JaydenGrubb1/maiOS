section .init
	; Contents of crtend.o init section will be linked here
	pop rbp
	
	; Registers used by multiboot for magic number and info struct pointer
	pop rsi
	pop rdi
	ret

section .fini
	; Contents of crtend.o fini section will be linked here
	pop rbp
	ret