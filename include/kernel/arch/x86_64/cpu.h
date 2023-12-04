/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-12
 * @brief CPU related functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <defines.h>

namespace CPU {
	/**
	 * @brief Halts the CPU
	 *
	 */
	inline NORETURN void halt(void) {
		asm volatile("cli; hlt");
		__builtin_unreachable();
	}
}
