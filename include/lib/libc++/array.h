/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-29
 * @brief Class template encapsulating a fixed-size array
 * @link https://en.cppreference.com/w/cpp/container/array @endlink
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

#include <lib/libc++/bits/reverse_iterator.h>
#include <lib/libc++/optional.h>
#include <type_traits>
#include <utility> // VERIFY Use this or custom <utility>?

namespace kstd {
	/**
	 * @brief Class template encapsulating a fixed-size array
	 *
	 * @tparam T The type of the elements in the array
	 * @tparam N The number of elements in the array
	 * 
	 * @link https://en.cppreference.com/w/cpp/container/array @endlink
	 */
	template <typename T, size_t N>
	class array {
	  public:
		T _data[N];

		/**
		 * @brief Return the number of elements in the array
		 *
		 * @return The number of elements in the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/size @endlink
		 */
		[[nodiscard]] constexpr size_t size(void) const {
			return N;
		}

		/**
		 * @brief Return the maximum number of elements the array can hold
		 *
		 * @return The maximum number of elements the array can hold
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/max_size @endlink
		 */
		[[nodiscard]] constexpr size_t max_size(void) const {
			return N;
		}

		/**
		 * @brief Check if the array is empty
		 *
		 * @return true if the array is empty, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/empty @endlink
		 */
		[[nodiscard]] constexpr bool empty(void) const {
			return N == 0;
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/operator_at @endlink
		 */
		[[nodiscard]] constexpr T &operator[](size_t index) {
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index
		 */
		[[nodiscard]] constexpr const T &operator[](size_t index) const {
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index, or an empty optional if the index is out of bounds
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/at @endlink
		 *
		 * @note This function does not conform to the C++ standard
		 */
		[[nodiscard]] constexpr optional<T> at(size_t index) {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index, or an empty optional if the index is out of bounds
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/at @endlink
		 *
		 * @note This function does not conform to the C++ standard
		 */
		[[nodiscard]] constexpr optional<const T> at(size_t index) const {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		/**
		 * @brief Return a reference to the first element
		 *
		 * @return A reference to the first element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/front @endlink
		 */
		[[nodiscard]] constexpr T &front(void) {
			return _data[0];
		}

		/**
		 * @brief Return a reference to the first element
		 *
		 * @return A reference to the first element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/front @endlink
		 */
		[[nodiscard]] constexpr const T &front(void) const {
			return _data[0];
		}

		/**
		 * @brief Return a reference to the last element
		 *
		 * @return A reference to the last element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/back @endlink
		 */
		[[nodiscard]] constexpr T &back(void) {
			return _data[N - 1];
		}

		/**
		 * @brief Return a reference to the last element
		 *
		 * @return A reference to the last element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/back @endlink
		 */
		[[nodiscard]] constexpr const T &back(void) const {
			return _data[N - 1];
		}

		/**
		 * @brief Return a pointer to the underlying array
		 *
		 * @return A pointer to the underlying array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/data @endlink
		 */
		[[nodiscard]] constexpr T *data(void) {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		/**
		 * @brief Return a pointer to the underlying array
		 *
		 * @return A pointer to the underlying array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/data @endlink
		 */
		[[nodiscard]] constexpr const T *data(void) const {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		/**
		 * @brief Return an iterator to the beginning of the array
		 *
		 * @return An iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/begin @endlink
		 */
		[[nodiscard]] constexpr T *begin(void) {
			return _data;
		}

		/**
		 * @brief Return an iterator to the beginning of the array
		 *
		 * @return An iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/begin @endlink
		 */
		[[nodiscard]] constexpr const T *begin(void) const {
			return _data;
		}

		/**
		 * @brief Return a const iterator to the beginning of the array
		 *
		 * @return A const iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/begin @endlink
		 */
		[[nodiscard]] constexpr const T *cbegin(void) const {
			return _data;
		}

		/**
		 * @brief Return an iterator past the end of the array
		 *
		 * @return An iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/end @endlink
		 */
		[[nodiscard]] constexpr T *end(void) {
			return _data + size();
		}

		/**
		 * @brief Return an iterator past the end of the array
		 *
		 * @return An iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/end @endlink
		 */
		[[nodiscard]] constexpr const T *end(void) const {
			return _data + size();
		}

		/**
		 * @brief Return a const iterator past the end of the array
		 *
		 * @return A const iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/end @endlink
		 */
		[[nodiscard]] constexpr const T *cend(void) const {
			return _data + size();
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the array
		 *
		 * @return A reverse iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<T *> rbegin(void) {
			return kstd::reverse_iterator(end());
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the array
		 *
		 * @return A reverse iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rbegin @endlink
		 */
		[[nodiscard]] constexpr const kstd::reverse_iterator<T *> rbegin(void) const {
			return kstd::reverse_iterator(end());
		}

		/**
		 * @brief Return a const reverse iterator to the beginning of the array
		 *
		 * @return A const reverse iterator to the beginning of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rbegin @endlink
		 */
		[[nodiscard]] constexpr const kstd::reverse_iterator<T *> crbegin(void) const {
			return kstd::reverse_iterator(end());
		}

		/**
		 * @brief Return a reverse iterator past the end of the array
		 *
		 * @return A reverse iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<T *> rend(void) {
			return kstd::reverse_iterator(begin());
		}

		/**
		 * @brief Return a reverse iterator past the end of the array
		 *
		 * @return A reverse iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rend @endlink
		 */
		[[nodiscard]] constexpr const kstd::reverse_iterator<T *> rend(void) const {
			return kstd::reverse_iterator(begin());
		}

		/**
		 * @brief Return a const reverse iterator past the end of the array
		 *
		 * @return A const reverse iterator past the end of the array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/rend @endlink
		 */
		[[nodiscard]] constexpr const kstd::reverse_iterator<T *> crend(void) const {
			return kstd::reverse_iterator(begin());
		}

		/**
		 * @brief Fill the array with the given value
		 *
		 * @param value The value to fill the array with
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/fill @endlink
		 */
		void fill(const T &value) {
			// TODO replace with kstd::fill_n(begin(), size(), value);
			for (size_t i = 0; i < size(); i++) {
				_data[i] = value;
			}
		}

		/**
		 * @brief Swap the contents of this array with another array
		 *
		 * @param other The array to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/container/array/swap @endlink
		 */
		void swap(array &other) {
			// TODO replace with kstd::swap_ranges(begin(), end(), other.begin());
			for (size_t i = 0; i < size(); i++) {
				T temp = _data[i];
				_data[i] = other._data[i];
				other._data[i] = temp;
			}
		}
	};

	template <typename T, std::size_t N>
	[[nodiscard]] constexpr bool operator==(const array<T, N> &lhs, const array<T, N> &rhs) {
		// TODO replace with kstd::equal(lhs.begin(), lhs.end(), rhs.begin());
		if (lhs.size() != rhs.size()) {
			return false;
		}
		for (size_t i = 0; i < lhs.size(); i++) {
			if (lhs[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}

	// TODO lexographical comparison operators

	/**
	 * @brief Get the element at the given index
	 *
	 * @tparam I The index to get the element at
	 * @tparam T The type of the array
	 * @tparam N The size of the array
	 * @param array The array to get the element from
	 * @return The element at the given index
	 *
	 * @link https://en.cppreference.com/w/cpp/container/array/get @endlink
	 */
	template <size_t I, typename T, size_t N>
	[[nodiscard]] constexpr T &get(array<T, N> &array) {
		static_assert(I < N, "Index out of bounds");
		return array[I];
	}

	/**
	 * @brief Get the element at the given index
	 *
	 * @tparam I The index to get the element at
	 * @tparam T The type of the array
	 * @tparam N The size of the array
	 * @param array The array to get the element from
	 * @return The element at the given index
	 *
	 * @link https://en.cppreference.com/w/cpp/container/array/get @endlink
	 */
	template <size_t I, typename T, size_t N>
	[[nodiscard]] constexpr T &&get(array<T, N> &&array) {
		static_assert(I < N, "Index out of bounds");
		return std::move(array[I]);
	}

	/**
	 * @brief Get the element at the given index
	 *
	 * @tparam I The index to get the element at
	 * @tparam T The type of the array
	 * @tparam N The size of the array
	 * @param array The array to get the element from
	 * @return The element at the given index
	 *
	 * @link https://en.cppreference.com/w/cpp/container/array/get @endlink
	 */
	template <size_t I, typename T, size_t N>
	[[nodiscard]] constexpr const T &get(const array<T, N> &array) {
		static_assert(I < N, "Index out of bounds");
		return array[I];
	}

	/**
	 * @brief Get the element at the given index
	 *
	 * @tparam I The index to get the element at
	 * @tparam T The type of the array
	 * @tparam N The size of the array
	 * @param array The array to get the element from
	 * @return The element at the given index
	 *
	 * @link https://en.cppreference.com/w/cpp/container/array/get @endlink
	 */
	template <size_t I, typename T, size_t N>
	[[nodiscard]] constexpr const T &&get(const array<T, N> &&array) {
		static_assert(I < N, "Index out of bounds");
		return std::move(array[I]);
	}

	/**
	 * @brief Swap the contents of two arrays
	 *
	 * @tparam T The type of the arrays
	 * @tparam N The size of the arrays
	 * @param lhs The first array
	 * @param rhs The second array
	 *
	 * @link https://en.cppreference.com/w/cpp/container/array/swap2 @endlink
	 */
	template <typename T, size_t N>
	constexpr void swap(array<T, N> &lhs, array<T, N> &rhs)
		requires(std::is_swappable_v<T> || N == 0)
	{
		lhs.swap(rhs);
	}

	// TODO to_array
	// https://en.cppreference.com/w/cpp/container/array/to_array
	// template <typename T, size_t N>
	// constexpr array<std::remove_cv_t<T>, N> to_array(T (&a)[N]);

	// TODO to_array
	// https://en.cppreference.com/w/cpp/container/array/to_array
	// template <typename T, size_t N>
	// constexpr array<std::remove_cv_t<T>, N> to_array(T (&&a)[N]);

	// TODO tuple_size
	// TODO tuple_element
}