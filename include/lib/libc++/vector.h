/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-29
 * @brief Class template encapsulating a dynamic-size array
 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <initializer_list>

#include <lib/libc++/bits/allocator.h>

namespace kstd {
	/**
	 * @brief Class template encapsulating a dynamic-size array
	 *
	 * @tparam T The type of the elements in the vector
	 * @tparam Alloc The allocator type used to allocate memory for the vector
	 *
	 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
	 */
	template <typename T, typename Alloc = allocator<T>>
	class vector {
	  public:
#pragma region Constructors
		constexpr vector(void) : _alloc() {}

		constexpr explicit vector(const Alloc &alloc) : _alloc(alloc) {}

		constexpr vector(size_t count, const T &value, const Alloc &alloc = Alloc()) : _alloc(alloc) {}

		constexpr explicit vector(size_t count, const Alloc &alloc = Alloc()) : _alloc(alloc) {}

		template <typename Iter>
		constexpr vector(Iter first, Iter last, const Alloc &alloc = Alloc()) : _alloc(alloc) {}

		constexpr vector(const vector &other) : _alloc(other._alloc) {}

		constexpr vector(const vector &other, const Alloc &alloc) : _alloc(alloc) {}

		constexpr vector(vector &&other) noexcept : _alloc(other._alloc) {}

		constexpr vector(vector &&other, const Alloc &alloc) : _alloc(alloc) {}

		constexpr vector(std::initializer_list<T> init, const Alloc &alloc = Alloc()) : _alloc(alloc) {}

		// TODO Range constructor
#pragma endregion

	  private:
		Alloc _alloc;
		T *_data;
	};
}