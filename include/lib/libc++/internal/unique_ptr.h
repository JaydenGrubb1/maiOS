/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-07
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

namespace kstd {
	/**
	 * @brief Specifies the deleter type for unique_ptr
	 *
	 * @tparam T The type of the pointer to be deleted
	 */
	template <typename T>
	class default_delete {
	  public:
		/**
		 * @brief Construct a new default delete object
		 *
		 */
		constexpr default_delete() = default;

		/**
		 * @brief Construct a new default delete object
		 *
		 * @tparam U The underlying type of the default_delete to copy
		 * @param other The default_delete to copy
		 */
		template <typename U>
		constexpr default_delete(const default_delete<U> &)
			requires std::is_convertible_v<U *, T *>
		{}

		/**
		 * @brief Deletes the pointer
		 *
		 * @param ptr The pointer to delete
		 */
		constexpr void operator()(T *ptr) const {
			static_assert(!std::is_void_v<T>, "can't delete pointer to incomplete type");
			static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
			delete ptr;
		}
	};

	/**
	 * @brief Specifies the deleter type for unique_ptr
	 *
	 * @tparam T The type of the pointer to be deleted
	 */
	template <typename T>
	class default_delete<T[]> {
	  public:
		/**
		 * @brief Construct a new default delete object
		 *
		 */
		constexpr default_delete() = default;

		/**
		 * @brief Construct a new default delete object
		 *
		 * @tparam U The underlying type of the default_delete to copy
		 * @param other The default_delete to copy
		 */
		template <typename U>
		constexpr default_delete(const default_delete<U[]> &)
			requires std::is_convertible_v<U (*)[], T (*)[]>
		{}

		/**
		 * @brief Deletes the pointer
		 *
		 * @param ptr The pointer to delete
		 */
		constexpr void operator()(T *ptr) const {
			static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
			delete[] ptr;
		}
	};
}