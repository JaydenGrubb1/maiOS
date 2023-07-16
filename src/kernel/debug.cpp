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
#include <lib/stdio.h>
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
	puts("[\033[31m FAIL \033[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_info(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\033[36m INFO \033[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_ok(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\033[32m  OK  \033[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_test(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\033[35m TEST \033[0m] ");
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_warning(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	puts("[\033[33m WARN \033[0m] ");
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
		uintptr_t symbol_address = 0;
		const char *symbol_name = KSyms::get_symbol((void *)return_address, &symbol_address);

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