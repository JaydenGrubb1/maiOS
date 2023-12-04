/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-30
 * @brief Defines the alloca function
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

extern "C" {
#undef alloca

/**
 * @brief Allocates memory on the stack
 * 
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory
 */
extern void *alloca(size_t size);

/**
 * @brief Allocates memory on the stack
 * 
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory
 */
#define alloca(size) __builtin_alloca(size)
}