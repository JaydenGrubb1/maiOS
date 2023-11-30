/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-30
 * @brief Memory allocation functions for the kernel
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

/**
 * @brief Allocates a block of memory of the given size
 *
 * @param size The amount of memory to allocate
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 */
[[nodiscard]] void *kmalloc(size_t size);

/**
 * @brief Frees a block of memory
 *
 * @param ptr A pointer to the memory to free
 * @param size The size of the memory block to free
 */
void kfree(void *ptr, size_t size);