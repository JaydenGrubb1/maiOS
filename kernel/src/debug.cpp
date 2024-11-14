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

#include <cstdint>

#include <algorithm>
#include <cstdio>
#include <cstring>

#include <kernel/arch/ksyms.h>
#include <kernel/debug.h>

void Debug::log(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("         ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_failure(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[31m FAIL \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_info(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[36m INFO \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_ok(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[32m  OK  \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_test(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[35m TEST \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_warning(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	fputs("[\u001b[33m WARN \u001b[0m] ", stdout);
	vprintf(format, ap);
	putchar('\n');
	va_end(ap);
}

void Debug::log_raw(const char *format, ...) {
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

void Debug::dump_memory(const void *start, const void *end) {
	uintptr_t diff = reinterpret_cast<uintptr_t>(end) ^ reinterpret_cast<uintptr_t>(start);
	uint8_t *ptr = static_cast<uint8_t *>(const_cast<void *>(start));
	const uint8_t *last = static_cast<const uint8_t *>(end);

	bool was_skipped = false;
	int digits = 1;
	while (diff) {
		diff >>= 4;
		digits++;
	}
	uintptr_t mask = (-1UL >> (64 - (digits * 4)));

	printf("Memory Dump: [%p => %p] (%zu bytes)\n", start, end, last - ptr);

	while (ptr < last) {
		if (last - ptr > 16) {
			if (memcmp(ptr, ptr - 16, 16) == 0) {
				was_skipped = true;
				ptr += 16;
				continue;
			}
		}
		if (was_skipped) {
			printf("*\n");
			was_skipped = false;
		}

		printf("%.*lx:  ", digits, reinterpret_cast<uintptr_t>(ptr) & mask);
		for (auto byte = ptr; byte < ptr + 16; byte++) {
			if (byte < last) {
				printf("%.2x ", *byte);
			} else {
				printf("   ");
			}
			if (byte == ptr + 7) {
				printf(" ");
			}
		}
		printf(" |");
		for (auto byte = ptr; byte < ptr + 16; byte++) {
			if (byte < last) {
				printf("%c", (*byte >= 32 && *byte <= 126) ? *byte : '.');
			} else {
				printf(" ");
			}
		}
		printf("|\n");

		ptr += 16;
	}
}