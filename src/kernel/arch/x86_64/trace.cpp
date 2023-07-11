/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/x86_64/trace.h>
#include <kernel/kprintf.h>
#include <stdint.h>

void Trace::trace(void) {
	uint64_t *base_ptr = (uint64_t *)__builtin_frame_address(0);

	kprintf("Stack trace:\n");
	int count = 0;
	while (base_ptr) {
		uint64_t return_address = *(base_ptr + 1);
		kprintf("frame #%d: %#.16lx\n", count++, return_address);
		base_ptr = (uint64_t *)*base_ptr;
	}

	// TODO Find kernel symbols and print them, e.g.
	// frame #0: 0x000000000010227c => 'general_protection_fault' (0x00000000001023e7) + 28
}