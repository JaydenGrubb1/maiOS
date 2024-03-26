/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-26
 * @brief Provides trait information for allocators
 * @link https://en.cppreference.com/w/cpp/memory/allocator_traits @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

namespace std {
	/**
	 * @brief Provides trait information for allocators
	 *
	 * @tparam T The type of the allocator to provide traits for
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/allocator_traits @endlink
	 */
	template <typename T>
	struct allocator_traits {
		using allocator_type = T;
		using value_type = typename T::value_type;
		using pointer = typename T::pointer;
		using const_pointer = typename T::const_pointer;
		using void_pointer = typename T::void_pointer;
		using const_void_pointer = typename T::const_void_pointer;
		using difference_type = typename T::difference_type;
		using size_type = typename T::size_type;
		using is_always_equal = typename T::is_always_equal;
		// TODO missing members
	};
	// TODO most members have fallback types
}