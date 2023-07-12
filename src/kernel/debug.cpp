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

#include <kernel/arch/ksyms.h>
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
	const char *symbol_name = nullptr;
	unsigned int count = 0;

	while (base_ptr && count < max_frames) {
		uint64_t return_address = *(base_ptr + 1);
		symbol_name = KSyms::get_symbol((void *)return_address);

		kprintf("frame #%d: %#.16lx => '%s' (%#.16lx) + %d\n",
				count++,
				return_address,
				symbol_name ? symbol_name : "<unknown>",
				return_address,
				0);
		base_ptr = (uint64_t *)*base_ptr;
	}
}