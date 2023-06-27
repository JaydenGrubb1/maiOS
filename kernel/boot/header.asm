; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

section .multiboot
header_start:
	align 8
	dd 0xe85250d6					; multiboot2 magic number
	dd 0							; architecture
	dd header_end - header_start	; header length

	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	; end tag
	align 8
	dw 0
	dw 0
	dd 8
header_end: