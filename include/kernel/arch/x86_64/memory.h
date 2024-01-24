/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Handles all memory management tasks for the kernel
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

namespace Memory {
	/**
	 * @brief Initilizes the memory manager
	 *
	 */
	void init(void);

	/**
	 * @brief Allocate a block of memory of the given size
	 *
	 * @param size The amount of memory to allocate
	 * @return A pointer to the allocated memory, or nullptr if the allocation failed
	 */
	void *allocate(size_t size, size_t alignment = 0, bool clear = false);

	/**
	 * @brief Deallocate a block of memory
	 *
	 * @param ptr A pointer to the memory to deallocate
	 * @param size The size of the memory block to deallocate
	 */
	void deallocate(void *ptr, size_t size = 0, size_t alignment = 0);
}