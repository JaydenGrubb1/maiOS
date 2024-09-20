/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2021-11-24
 * @brief Used to read and write value to IO
 *
 * Copyright (c) 2021, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

namespace IO {
	/**
	 * @brief Reads a value from a port
	 *
	 * @tparam T The type of value to read (must be 8, 16, or 32 bits)
	 * @param port The port to read from
	 * @return The value read from the port
	 */
	template <typename T>
	inline T read(uint16_t port)
		requires(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4)
	{
		T value;
		asm volatile("in %0, %1"
					 : "=a"(value)
					 : "Nd"(port));
		return value;
	}

	/**
	 * @brief Writes a value to a port
	 *
	 * @tparam T The type of value to write (must be 8, 16, or 32 bits)
	 * @param port The port to write to
	 * @param value The value to write to the port
	 */
	template <typename T>
	inline void write(uint16_t port, T value)
		requires(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4)
	{
		asm volatile("out %1, %0" ::"a"(value), "Nd"(port));
	}
}