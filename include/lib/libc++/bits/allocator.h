/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-29
 * @brief Default allocator class for all containers
 * @link https://en.cppreference.com/w/cpp/memory/allocator @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <kernel/debug.h>

namespace kstd {
	/**
	 * @brief Default allocator class for all containers
	 *
	 * @tparam T The type of the object to allocate
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/allocator @endlink
	 */
	template <typename T>
	class allocator {
	  public:
		/**
		 * @brief Constructs a new allocator object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/allocator @endlink
		 */
		constexpr allocator(void) = default;

		/**
		 * @brief Constructs a new allocator object
		 *
		 * @param other The allocator to copy
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/allocator @endlink
		 */
		constexpr allocator(const allocator &other) = default;

		/**
		 * @brief Constructs a new allocator object
		 *
		 * @tparam U The base type of the allocator to copy
		 * @param other The allocator to copy
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/allocator @endlink
		 */
		template <typename U>
		constexpr allocator(const allocator<U> &other) {}

		/**
		 * @brief Destroy the allocator object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/%7Eallocator @endlink
		 */
		constexpr ~allocator(void) = default;

		/**
		 * @brief Allocates n * sizeof(T) bytes of uninitialized storage
		 *
		 * @param n The number of objects to allocate
		 * @return A pointer to the allocated storage
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/allocate @endlink
		 */
		[[nodiscard]] constexpr T *allocate(__attribute__((unused)) size_t n) {
			Debug::log_warning("kstd::allocator is a placeholder and should not be used");
			return nullptr;
		}

		/**
		 * @brief Deallocates the storage referenced by the pointer p
		 *
		 * @param p The pointer to the storage to deallocate
		 * @param n The number of objects to deallocate
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/allocator/deallocate @endlink
		 */
		constexpr void deallocate(__attribute__((unused)) T *p, __attribute__((unused)) size_t n) {
			Debug::log_warning("kstd::allocator is a placeholder and should not be used");
		}
	};
}