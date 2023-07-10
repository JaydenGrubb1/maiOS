/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-10
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace Interrupts {
	/**
	 * @brief Clears the interrupt flag
	 *
	 */
	inline void cli(void) {
		asm volatile("cli");
	}

	/**
	 * @brief Sets the interrupt flag
	 *
	 */
	inline void sti(void) {
		asm volatile("sti");
	}

	/**
	 * @brief Initializes the Interrupt Descriptor Table
	 *
	 */
	void init_idt(void);
}