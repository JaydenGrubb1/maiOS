/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-16
 * @brief Handles the allocation of physical page frames
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <optional>

#include <kernel/arch/x86_64/memory/physaddr.h>

namespace Memory::PhysicalMemory {
	/**
	 * @brief Initilizes the physical memory allocator
	 *
	 */
	void init(void);

	/**
	 * @brief Allocates a page of physical memory
	 *
	 * @return The physical address of the allocated page, or nullopt if allocation failed
	 */
	[[nodiscard]] std::optional<PhysAddr> alloc(void);

	/**
	 * @brief Frees a page of physical memory
	 *
	 * @param addr The physical address of the page to free
	 */
	void free(PhysAddr addr);
}