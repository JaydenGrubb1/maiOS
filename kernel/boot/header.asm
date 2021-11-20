; @file kernel/boot/header.asm
; @author Jayden Grubb (contact@jaydengrubb.com)
; @date 2021-11-20
; @brief // DOC 
; 
; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

section .multiboot_header
header_start:
	dd 0xe85250d6					; multiboot2
	dd 0							; protected mode i386
	dd header_end - header_start	; header length

	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	; end tag
	dw 0
	dw 0
	dd 8
header_end: