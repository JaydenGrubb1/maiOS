/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-16
 * @brief Automatically disables/enables interrupts when in scope
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <kernel/arch/x86_64/interrupts.h>

namespace Interrupts {
	/**
	 * @brief Automatically disables/enables interrupts when in scope
	 *
	 */
	class Guard {
	  private:
		bool _was_enabled;

	  public:
		/**
		 * @brief Construct a new Interrupt::Guard object and disable interrupts
		 *
		 */
		Guard(void) : _was_enabled(Interrupts::is_enabled()) {
			Interrupts::disable();
		}

		// disallow copy constructor
		Guard(const Guard &) = delete;

		// disallow assignment
		Guard &operator=(const Guard &) = delete;

		/**
		 * @brief Destroy the Interrupt::Guard object and re-enable interrupts if they were enabled
		 *
		 */
		~Guard() {
			if (_was_enabled) {
				Interrupts::enable();
			}
		}
	};
}