/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-17
 * @brief Represents a zone of memory
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace Memory::PhysicalMemory {
	/**
	 * @brief A zone of memory
	 *
	 */
	using Zone = uintmax_t;

	/**
	 * @brief The size of a zone
	 *
	 */
	constexpr size_t ZONE_SIZE = sizeof(Zone) * 8;
}