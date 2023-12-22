/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Placement new operator
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

#include <defines.h>

/**
 * @brief Placement new operator
 *
 * @param size The size of the memory to construct the object in
 * @param ptr The pointer to the memory to construct the object in
 * @return The pointer to the memory to construct the object in
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] inline void *operator new(UNUSED size_t size, void *ptr) {
	return ptr;
}

/**
 * @brief Placement new operator
 *
 * @param size The size of the memory to construct the object in
 * @param ptr The pointer to the memory to construct the object in
 * @return The pointer to the memory to construct the object in
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] inline void *operator new[](UNUSED size_t size, void *ptr) {
	return ptr;
}

// VERIFY are these even needed?
// void operator delete(UNUSED void *ptr, UNUSED void *place);
// void operator delete[](UNUSED void *ptr, UNUSED void *place);