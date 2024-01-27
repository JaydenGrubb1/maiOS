/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-11
 * @brief Provides access to the CMOS
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

namespace CMOS {
	/**
	 * @brief Read a byte from the CMOS
	 *
	 * @param reg The register to read from
	 * @return The value read from the register
	 */
	[[nodiscard]] uint8_t read(uint8_t reg);

	/**
	 * @brief Write a byte to the CMOS
	 *
	 * @param reg The register to write to
	 * @param value The value to write to the register
	 */
	void write(uint8_t reg, uint8_t value);
}