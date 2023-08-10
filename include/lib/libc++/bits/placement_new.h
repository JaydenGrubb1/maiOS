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

/**
 * @brief Placement new operator
 *
 * @param ptr The pointer to the memory to construct the object in
 * @return The pointer to the memory to construct the object in
 */
[[nodiscard]] inline void *operator new(size_t, void *ptr) {
	return ptr;
}

/**
 * @brief Placement new operator
 *
 * @param ptr The pointer to the memory to construct the object in
 * @return The pointer to the memory to construct the object in
 */
[[nodiscard]] inline void *operator new[](size_t, void *ptr) {
	return ptr;
}