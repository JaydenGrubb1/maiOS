/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-04
 * @brief Wraps the kernel entry point in an extern "C" block
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

extern "C" void __stdio_init(void);
extern "C" void __stack_chk_init(void);

/**
 * @brief Wraps the kernel entry point in an extern "C" block
 *
 */
#define KERNEL_ENTRY(FUNC)                               \
	extern "C" void _start(uint32_t magic, void *addr) { \
		__stack_chk_init();                              \
		__stdio_init();                                  \
		FUNC(magic, addr);                               \
	}
