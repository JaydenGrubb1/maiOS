; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

global init64_start
extern kmain

section .text
bits 64
init64_start:
	; Sets all data segment registers to 0 as they aren't used
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Finally, go to main kernel function
	call kmain
	hlt
