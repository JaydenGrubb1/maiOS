; @file kernel/boot/init64.asm
; @author Jayden Grubb (contact@jaydengrubb.com)
; @date 2021-11-20
; @brief // DOC 
; 
; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

global init64_start
; extern kmain

section .text
bits 64
init64_start:
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; call kmain
	hlt
