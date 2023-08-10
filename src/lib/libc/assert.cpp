/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Defines the assert macro
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <kernel/arch/cpu.h>
#include <kernel/debug.h>
#include <lib/libc/assert.h>

void __assert_fail(const char *assertion, const char *file, unsigned int line, const char *function) {
	Debug::log_failure("\u001b[31mAssertion failed: '%s'\nFile: \"%s\" (line %d)\nFunction: %s",
					   assertion,
					   file,
					   line,
					   function);
	Debug::trace_stack(__builtin_frame_address(0));
	Debug::log("\u001b[0m");
	CPU::halt();
}