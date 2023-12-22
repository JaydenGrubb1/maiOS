/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-16
 * @brief Represents a region of memory
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define __need_size_t
#include <stddef.h>

#include <arch/x86_64/memory/paging.h>
#include <arch/x86_64/memory/physaddr.h>
#include <arch/x86_64/memory/zone.h>

namespace Memory {
	/**
	 * @brief A region of memory
	 *
	 */
	struct MemoryRegion {
		PhysAddr lower;
		PhysAddr upper;

		/**
		 * @brief Returns the size of the region
		 *
		 * @return The size of the region in bytes
		 */
		constexpr size_t size(void) const {
			return upper - lower;
		}

		/**
		 * @brief Returns the number of pages in the region
		 *
		 * @return The number of pages in the region
		 */
		constexpr size_t pages(void) const {
			return size() / Paging::PAGE_SIZE;
		}

		/**
		 * @brief Returns the number of zones in the region
		 *
		 * @return The number of zones in the region
		 */
		constexpr size_t zones(void) const {
			return pages() / FrameAllocator::ZONE_SIZE;
		}

		/**
		 * @brief Checks if the region contains the given address
		 *
		 * @param addr The address to check
		 * @return true if the region contains the address
		 */
		constexpr bool contains(PhysAddr addr) const {
			return addr >= lower && addr < upper;
		}
	};
}