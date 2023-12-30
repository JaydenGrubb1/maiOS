/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-02
 * @brief Various functions for constructing and destroying objects
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <internal/placement_new.h>
#include <utility>

namespace kstd {
	/**
	 * @brief Returns the address of the given reference
	 *
	 * @tparam T The type of the reference
	 * @param arg The reference
	 * @return The address of the reference
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/addressof @endlink
	 */
	template <typename T>
	[[nodiscard]] inline constexpr T *addressof(T &arg) {
		return __builtin_addressof(arg);
	}

	// Disallow getting the address of an rvalue
	template <typename T>
	const T *addressof(const T &&arg) = delete;

	/**
	 * @brief Construct an object at the given pointer
	 *
	 * @tparam T The type of the object
	 * @tparam Args The types of the arguments to pass to the constructor
	 * @param ptr The pointer to construct the object at
	 * @param args The arguments to pass to the constructor
	 * @return The constructed object
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/construct_at @endlink
	 */
	template <typename T, typename... Args>
	constexpr auto construct_at(T *ptr, Args &&...args)
		-> decltype(::new(static_cast<void *>(0)) T(std::declval<Args>()...)) {
		return ::new (static_cast<void *>(ptr)) T(kstd::forward<Args>(args)...);
	}

	/**
	 * @brief Destroy the object at the given pointer
	 *
	 * @tparam T The type of the object
	 * @param ptr The object to destroy
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/destroy_at @endlink
	 */
	template <typename T>
	inline constexpr void destroy_at(T *ptr) {
		if constexpr (std::is_array_v<T>) {
			for (auto &elem : *ptr) {
				destroy_at(kstd::addressof(elem));
			}
		} else {
			ptr->~T();
		}
	}
}