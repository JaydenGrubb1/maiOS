/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Provides access to the Real Time Clock
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

#include <kernel/arch/x86_64/time.h>

namespace Time::RTC {
	/**
	 * @brief Initialize the Real Time Clock
	 *
	 */
	void init(void);

	/**
	 * @brief Get the current time
	 *
	 * @return The current time
	 */
	DateTime now(void);

	/**
	 * @brief Get the time the system was booted
	 *
	 * @return The time the system was booted
	 */
	DateTime boot_time(void);
}