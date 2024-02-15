/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Various debugging functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdarg.h>
#include <stdint.h>

#include <cstdio>

#include <kernel/arch/ksyms.h>
#include <kernel/debug.h>

void Debug::log(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("         ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_failure(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[31m FAIL \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_info(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[36m INFO \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_ok(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[32m  OK  \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_test(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[35m TEST \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_warning(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[33m WARN \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_raw(const char *__restrict__ format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

void Debug::trace_stack(void) {
	Debug::trace_stack(__builtin_frame_address(0));
}

void Debug::trace_stack(void *frame_ptr) {
	unsigned int count = 0;
	printf("Stack Trace:%s\n", KSyms::is_available() ? "" : " (no symbol table)");

	while (frame_ptr && count < DEFAULT_MAX_FRAMES) {
		uintptr_t return_address = *(static_cast<uintptr_t *>(frame_ptr) + 1);
		auto [symbol_name, symbol_address] = KSyms::get_symbol(reinterpret_cast<void *>(return_address));

		// TODO Demangle C++ symbols

		if (!symbol_name.empty()) {
			printf("%3d) [<%#.16lx>] %s (+%#lx)\n",
				   count++,
				   return_address,
				   symbol_name.data(),
				   return_address - symbol_address);
		} else {
			printf("%3d) [<%#.16lx>] <unknown>\n",
				   count++,
				   return_address);
		}
		frame_ptr = reinterpret_cast<uintptr_t *>(*static_cast<uintptr_t *>(frame_ptr));
	}
}