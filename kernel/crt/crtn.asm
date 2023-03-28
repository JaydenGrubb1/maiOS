section .init
	; Contents of crtend.o init section will be linked here
	pop rbp
	ret

section .fini
	; Contents of crtend.o fini section will be linked here
	pop rbp
	ret