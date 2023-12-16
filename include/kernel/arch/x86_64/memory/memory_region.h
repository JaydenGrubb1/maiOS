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

#include <kernel/arch/x86_64/memory/physaddr.h>

namespace Memory {
	/**
	 * @brief A region of memory
	 * 
	 */
	struct MemoryRegion {
		PhysAddr base;
		size_t length;
	};
}