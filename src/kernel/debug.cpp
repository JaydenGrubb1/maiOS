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

#include <kernel/debug.h>
#include <kernel/kprintf.h>
#include <stdarg.h>
#include <stdint.h>

void Debug::log(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	kputs("         ");
	kvprintf(format, ap);
	kputchar('\n');
	va_end(ap);
}

void Debug::log_failure(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	kputs("[\u001b[31m FAIL \u001b[0m] ");
	kvprintf(format, ap);
	kputchar('\n');
	va_end(ap);
}

void Debug::log_info(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	kputs("[\u001b[36m INFO \u001b[0m] ");
	kvprintf(format, ap);
	kputchar('\n');
	va_end(ap);
}

void Debug::log_ok(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	kputs("[\u001b[32m  OK  \u001b[0m] ");
	kvprintf(format, ap);
	kputchar('\n');
	va_end(ap);
}

void Debug::log_warning(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	kputs("[\u001b[33m WARN \u001b[0m] ");
	kvprintf(format, ap);
	kputchar('\n');
	va_end(ap);
}

void Debug::trace(void) {
	Debug::trace(DEFAULT_MAX_FRAMES);
}

void Debug::trace(unsigned int max_frames) {
	uint64_t *base_ptr = (uint64_t *)__builtin_frame_address(0);

	kprintf("Stack trace:\n");
	unsigned int count = 0;
	while (base_ptr && count < max_frames) {
		uint64_t return_address = *(base_ptr + 1);
		kprintf("frame #%d: %#.16lx => '%s' (%#.16lx) + %d\n",
				count++,
				return_address,
				"unknown",
				return_address,
				0);
		base_ptr = (uint64_t *)*base_ptr;
	}

	// TODO Find kernel symbols and print them, e.g.
	// frame #0: 0x000000000010229c => 'invalid_opcode' (0x000000000010226a) + 32
}