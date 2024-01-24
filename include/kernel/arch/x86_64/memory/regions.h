/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-17
 * @brief Returns a list of all usable memory regions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <vector>

#include <kernel/arch/x86_64/memory/memory_region.h>

namespace Memory {
	/**
	 * @brief Returns a list of all usable memory regions
	 *
	 * @return A list of all usable memory regions
	 */
	std::vector<MemoryRegion> const &regions(void);
}

// VERIFY better way to solve this?
// Memory::regions is defined separately from the rest of the Memory
// namespace to avoid circular dependencies with std::vector