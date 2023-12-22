/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Defines the assert macro
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/assert.h.html @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <arch/cpu.h>
#include <assert.h>
#include <debug.h>

void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function) {
	Debug::log_failure("Assertion failed: '%s'\nFile: \"%s\" (line %d)\nFunction: %s",
					   assertion,
					   file,
					   line,
					   function);
	Debug::trace_stack(__builtin_frame_address(0));
	CPU::halt();
}