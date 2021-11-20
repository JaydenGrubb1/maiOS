; @file kernel/boot/init32.asm
; @author Jayden Grubb (contact@jaydengrubb.com)
; @date 2021-11-20
; @brief // DOC 
; 
; Copyright (c) 2021, Jayden Grubb
; All rights reserved.
; 
; This source code is licensed under the BSD-style license found in the
; LICENSE file in the root directory of this source tree.

global start
; extern long_mode_start

section .text
bits 32

start:
	mov dword [0xb8000], 0x2f4b2f4f
	hlt