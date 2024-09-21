/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-09-22
 * @brief Standard C library memory allocation functions
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html @endlink
 */
[[nodiscard]] void *malloc(size_t size);

/**
 * @brief Allocate memory and set it to zero
 *
 * @param num_elem The number of elements to allocate
 * @param size_elem The size of each element
 * @return A pointer to the allocated memory, or nullptr if the allocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/calloc.html @endlink
 */
[[nodiscard]] void *calloc(size_t num_elem, size_t size_elem);

/**
 * @brief Reallocate memory
 *
 * @param ptr The pointer to the memory to reallocate
 * @param size The new size of the memory
 * @return A pointer to the reallocated memory, or nullptr if the reallocation failed
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/realloc.html @endlink
 */
[[nodiscard]] void *realloc(void *ptr, size_t size);

/**
 * @brief Deallocate memory
 *
 * @param ptr A pointer to the memory to deallocate
 *
 * @link https://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html @endlink
 */
void free(void *ptr);

#ifdef __cplusplus
}
#endif