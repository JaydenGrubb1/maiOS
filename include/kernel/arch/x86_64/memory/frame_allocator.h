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

namespace Memory::FrameAllocator {
	/**
	 * @brief Initilizes the page frame allocator
	 *
	 */
	void init(void);

	/**
	 * @brief Allocates a page frame
	 *
	 * @return The physical address of the allocated page frame, or nullopt if allocation failed
	 */
	[[nodiscard]] kstd::optional<PhysAddr> alloc(void);

	/**
	 * @brief Frees a page frame
	 *
	 * @param addr The physical address of the page frame to free
	 */
	void free(PhysAddr addr);
}