/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-31
 * @brief CPU related functions
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/cpu.h>

bool CPU::has_feature(Feature feature) {
	uint32_t leaf = static_cast<uint32_t>(feature) >> 8;
	uint8_t bit = (static_cast<uint32_t>(feature) >> 2) & 0x3F;
	uint8_t reg = static_cast<uint32_t>(feature) & 0x3;

	uint32_t eax, ebx, ecx, edx;
	asm volatile("cpuid"
				 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
				 : "a"(leaf), "c"(0));

	switch (reg) {
		case 0:
			return (eax & (1 << bit)) != 0;
		case 1:
			return (ebx & (1 << bit)) != 0;
		case 2:
			return (ecx & (1 << bit)) != 0;
		case 3:
			return (edx & (1 << bit)) != 0;
		default:
			return false;
	}
}