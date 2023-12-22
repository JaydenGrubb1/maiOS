/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Iterator adaptor for reversing the direction of a given iterator
 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stddef.h> // Used for ptrdiff_t
#include <type_traits>
// TODO replace ptrdiff_t with "more appropriate type"

namespace kstd {
	template <typename>
	class reverse_iterator;

	/**
	 * @brief Iterator adaptor for reversing the direction of a given iterator
	 *
	 * @tparam T The type of the iterator to be reversed
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator @endlink
	 */
	template <typename T>
	class reverse_iterator {
	  private:
		T _iterator;

		template <typename U>
		friend class reverse_iterator;

	  public:
		/**
		 * @brief Constructs a reverse_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		constexpr reverse_iterator(void) = default;

		/**
		 * @brief Constructs a reverse_iterator from the given iterator
		 *
		 * @param iterator The iterator to be reversed
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		constexpr explicit reverse_iterator(T iterator) : _iterator(iterator) {}

		/**
		 * @brief Constructs a reverse_iterator from the given reverse_iterator
		 *
		 * @tparam U The type of the iterator to be reversed
		 * @param other The reverse_iterator to be reversed
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		template <typename U>
		constexpr reverse_iterator(const reverse_iterator<U> &other)
			requires(!std::is_same_v<T, U>)
			: _iterator(other._iterator) {}
		// TODO std::convertible_to<const U&, Iter>

		/**
		 * @brief Assigns the given iterator to this reverse_iterator
		 *
		 * @tparam U The type of the iterator to be assigned
		 * @param other The iterator to be assigned
		 * @return A reference to this reverse_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator%3D @endlink
		 */
		template <typename U>
		constexpr reverse_iterator &operator=(const reverse_iterator<U> &other)
			requires(!std::is_same_v<T, U>)
		{
			_iterator = other._iterator;
			return *this;
		}
		// TODO std::convertible_to<const U&, Iter>
		// TODO std::assignable_from<Iter&, const U&>

		/**
		 * @brief Returns the underlying iterator
		 *
		 * @return The underlying iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/base @endlink
		 */
		[[nodiscard]] constexpr T base(void) const {
			return _iterator;
		}

		/**
		 * @brief Returns a reference to the element previous to the one currently pointed to
		 *
		 * @return A reference to the element previous to the one currently pointed to
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator* @endlink
		 */
		[[nodiscard]] constexpr std::remove_pointer_t<T> &operator*(void) const {
			// TODO better type safety
			T tmp = _iterator;
			return *--tmp;
		}

		/**
		 * @brief Returns a pointer to the element previous to the one currently pointed to
		 *
		 * @return A pointer to the element previous to the one currently pointed to
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator* @endlink
		 */
		[[nodiscard]] constexpr T operator->(void) const
			requires(std::is_pointer_v<T> || requires(const T i) { i.operator->(); })
		{
			// TODO better type safety
			T tmp = _iterator;
			return --tmp;
		}

		/**
		 * @brief Return a reference to the element at the specified index
		 *
		 * @param n Position of the element to return relative to the one currently pointed to
		 * @return A reference to the element at the specified index
		 */
		[[nodiscard]] constexpr std::remove_pointer_t<T> &operator[](ptrdiff_t n) const {
			// TODO better type safety
			return base()[-n - 1];
		}

// Increment/Decrement Operators
// https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator_arith
#pragma region Increment/Decrement Operators
		constexpr reverse_iterator &operator++(void) {
			--_iterator;
			return *this;
		}

		constexpr reverse_iterator &operator--(void) {
			++_iterator;
			return *this;
		}

		constexpr reverse_iterator operator++(int) {
			auto tmp = *this;
			--_iterator;
			return tmp;
		}

		constexpr reverse_iterator operator--(int) {
			auto tmp = *this;
			++_iterator;
			return tmp;
		}

		constexpr reverse_iterator operator+(ptrdiff_t n) const {
			return reverse_iterator(_iterator - n);
		}

		constexpr reverse_iterator operator-(ptrdiff_t n) const {
			return reverse_iterator(_iterator + n);
		}

		constexpr reverse_iterator &operator+=(ptrdiff_t n) {
			_iterator -= n;
			return *this;
		}

		constexpr reverse_iterator &operator-=(ptrdiff_t n) {
			_iterator += n;
			return *this;
		}
#pragma endregion

		// TODO iter_move
		// https://en.cppreference.com/w/cpp/iterator/reverse_iterator/iter_move
		// friend constexpr std::iter_rvalue_reference_t<T> iter_move(const reverse_iterator &i) {
		// }

		// TODO iter_swap
		// https://en.cppreference.com/w/cpp/iterator/reverse_iterator/iter_swap
		// template <std::indirectly_swappable<T> T2>
		// friend constexpr void iter_swap(const reverse_iterator &x, const reverse_iterator<T2> &y) {
		// }
	};

// Comparison Operators
// https://en.cppreference.com/w/cpp/iterator/reverse_iterator
#pragma region Comparison Operators
	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator==(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() == rhs.base();
	}

	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator!=(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() != rhs.base();
	}

	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator<(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() > rhs.base();
	}

	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator<=(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() >= rhs.base();
	}

	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator>(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() < rhs.base();
	}

	template <typename T, typename U>
	[[nodiscard]] constexpr bool operator>=(const reverse_iterator<T> &lhs, const reverse_iterator<U> &rhs) {
		return lhs.base() <= rhs.base();
	}

	// TODO 3-way comparison
#pragma endregion

	/**
	 * @brief Advances the given reverse_iterator by the given amount
	 *
	 * @tparam T The type of the reverse_iterator
	 * @param n The amount to advance the reverse_iterator by
	 * @param iter The reverse_iterator to advance
	 * @return The advanced reverse_iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator%2B @endlink
	 */
	template <typename T>
	[[nodiscard]] constexpr reverse_iterator<T> operator+(ptrdiff_t n, const reverse_iterator<T> &iter) {
		return reverse_iterator<T>(iter.base() - n);
	}

	/**
	 * @brief Calculates the distance between two reverse_iterators
	 *
	 * @tparam I1 The type of the first reverse_iterator
	 * @tparam I2 The type of the second reverse_iterator
	 * @param lhs The first reverse_iterator
	 * @param rhs The second reverse_iterator
	 * @return The distance between the two reverse_iterators
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator- @endlink
	 */
	template <typename T1, typename T2>
	[[nodiscard]] constexpr ptrdiff_t operator-(const reverse_iterator<T1> &lhs, const reverse_iterator<T2> &rhs) {
		return rhs.base() - lhs.base();
	}

	/**
	 * @brief Creates a reverse_iterator from the given iterator
	 *
	 * @tparam T The type of the iterator to be reversed
	 * @param i The iterator to be reversed
	 * @return A reverse_iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/make_reverse_iterator @endlink
	 */
	template <typename T>
	[[nodiscard]] constexpr reverse_iterator<T> make_reverse_iterator(T i) {
		return reverse_iterator<T>(i);
	}
}