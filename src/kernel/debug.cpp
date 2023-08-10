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
#include <lib/libc/stdio.h>
#include <stdarg.h>
#include <stdint.h>

void Debug::log(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("         ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_failure(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\u001b[31m FAIL \u001b[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_info(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\u001b[36m INFO \u001b[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_ok(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\u001b[32m  OK  \u001b[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_test(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\u001b[35m TEST \u001b[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_warning(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\u001b[33m WARN \u001b[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::trace_stack(void) {
	Debug::trace_stack(__builtin_frame_address(0));
}

void Debug::trace_stack(void *frame_ptr) {
	unsigned int count = 0;
	printf("Stack Trace:%s\n", KSyms::is_available() ? "" : " (no symbol table)");

	while (frame_ptr && count < DEFAULT_MAX_FRAMES) {
		uintptr_t return_address = *((uintptr_t *)frame_ptr + 1);
		auto [symbol_name, symbol_address] = KSyms::get_symbol((void *)return_address);

		// TODO Demangle C++ symbols

		if (symbol_name) {
			printf("%3d) [<%#.16lx>] %s (+%#lx)\n",
				   count++,
				   return_address,
				   symbol_name,
				   return_address - symbol_address);
		} else {
			printf("%3d) [<%#.16lx>] <unknown>\n",
				   count++,
				   return_address);
		}
		frame_ptr = (uintptr_t *)*((uintptr_t *)frame_ptr);
	}
}