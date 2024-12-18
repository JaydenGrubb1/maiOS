/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-01-19
 * @brief Handles stack smashing handling
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <stdint.h>

#include <kernel/panic.h>

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t __stack_chk_guard;

#pragma GCC push_options
#pragma GCC optimize("-fno-stack-protector")
void __stack_chk_init(void) {
	// TODO randomize this value
	__stack_chk_guard = 0xdeadbeefdeadbeef;
}
#pragma GCC pop_options

[[noreturn]] void __stack_chk_fail(void) {
	Kernel::panic("Stack smashing detected");
}

#ifdef __cplusplus
}
#endif