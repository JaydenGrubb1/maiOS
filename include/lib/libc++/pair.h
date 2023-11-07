/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Class template for storing a pair of heterogeneous values
 * @note https://en.cppreference.com/w/cpp/utility/pair @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <lib/libc++/utility.h>

namespace kstd {
	/**
	 * @brief Class template for storing a pair of heterogeneous values
	 *
	 * @tparam T1 The type of the first value
	 * @tparam T2 The type of the second value
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/pair @endlink
	 */
	template <typename T1, typename T2>
	class pair {
	  public:
		T1 first;
		T2 second;

#pragma region Constructors
		/**
		 * @brief Construct a new pair object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		constexpr explicit pair()
			requires(std::is_default_constructible_v<T1> && std::is_default_constructible_v<T2>)
			: first(), second() {}
		// TODO check implicit default constructor

		/**
		 * @brief Construct a new pair object
		 *
		 * @param x The first value
		 * @param y The second value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		constexpr explicit(!std::is_convertible_v<const T1 &, T1> || !std::is_convertible_v<const T2 &, T2>)
			pair(const T1 &x, const T2 &y)
			requires(std::is_copy_constructible_v<T1> && std::is_copy_constructible_v<T2>)
			: first(x), second(y) {}

		/**
		 * @brief Construct a new pair object
		 *
		 * @tparam U1 The type of the first value to construct from
		 * @tparam U2 The type of the second value to construct from
		 * @param x The first value to construct from
		 * @param y The second value to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		template <typename U1 = T1, typename U2 = T2>
		constexpr explicit(!std::is_convertible_v<U1, T1> || !std::is_convertible_v<U2, T2>)
			pair(U1 &&x, U2 &&y)
			requires(std::is_constructible_v<T1, U1> && std::is_constructible_v<T2, U2>)
			: first(kstd::forward<U1>(x)), second(kstd::forward<U2>(y)) {}

		/**
		 * @brief Construct a new pair object
		 *
		 * @tparam U1 The type of the first value of the pair to construct from
		 * @tparam U2 The type of the second value of the pair to construct from
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<const U1 &, T1> || !std::is_convertible_v<const U2 &, T2>)
			pair(pair<U1, U2> &p)
			requires(std::is_constructible_v<T1, const U1 &> && std::is_constructible_v<T2, const U2 &>)
			: first(p.first), second(p.second) {}
		// TODO delete if initialization would bind to a reference to a temporary

		/**
		 * @brief Construct a new pair object
		 *
		 * @tparam U1 The type of the first value of the pair to construct from
		 * @tparam U2 The type of the second value of the pair to construct from
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<U1 &, T1> || !std::is_convertible_v<U2 &, T2>)
			pair(const pair<U1, U2> &p)
			requires(std::is_constructible_v<T1, U1 &> && std::is_constructible_v<T2, U2 &>)
			: first(p.first), second(p.second) {}
		// TODO delete if initialization would bind to a reference to a temporary

		/**
		 * @brief Construct a new pair object
		 *
		 * @tparam U1 The type of the first value of the pair to construct from
		 * @tparam U2 The type of the second value of the pair to construct from
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<U1 &&, T1> || !std::is_convertible_v<U2 &&, T2>)
			pair(pair<U1, U2> &&p)
			requires(std::is_constructible_v<T1, U1 &&> && std::is_constructible_v<T2, U2 &&>)
			: first(kstd::forward<U1>(p.first)), second(kstd::forward<U2>(p.second)) {}
		// TODO delete if initialization would bind to a reference to a temporary

		/**
		 * @brief Construct a new pair object
		 *
		 * @tparam U1 The type of the first value of the pair to construct from
		 * @tparam U2 The type of the second value of the pair to construct from
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<const U1 &&, T1> || !std::is_convertible_v<const U2 &&, T2>)
			pair(const pair<U1, U2> &&p)
			requires(std::is_constructible_v<T1, const U1 &&> && std::is_constructible_v<T2, const U2 &&>)
			: first(kstd::forward<U1>(p.first)), second(kstd::forward<U2>(p.second)) {}
		// TODO delete if initialization would bind to a reference to a temporary

		/**
		 * @brief Construct a new pair object
		 *
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		constexpr pair(const pair &p) = default;

		/**
		 * @brief Construct a new pair object
		 *
		 * @param p The pair to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/pair @endlink
		 */
		constexpr pair(pair &&p) = default;

		// template <pair-like P>
		// constexpr explicit pair(P &&u);
		// // TODO what is pair-like?

		// template <typename... Args1, typename... Args2>
		// constexpr pair(std::piecewise_construct_t, tuple<Args1...> arg1, tuple<Args2...> arg2);
		// // TODO what is piecewise_construct_t?
#pragma endregion

#pragma region Assignment Operators
		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		constexpr pair &operator=(const pair &other)
			requires(std::is_copy_assignable_v<T1> && std::is_copy_assignable_v<T2>)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		constexpr const pair &operator=(const pair &other) const
			requires(std::is_copy_assignable_v<const T1> && std::is_copy_assignable_v<const T2>)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @tparam U1 The type of the first value of the pair to assign from
		 * @tparam U2 The type of the second value of the pair to assign from
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		template <typename U1, typename U2>
		constexpr pair &operator=(const pair<U1, U2> &other)
			requires(std::is_assignable_v<T1 &, const U1 &> && std::is_assignable_v<T2 &, const U2 &>)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @tparam U1 The type of the first value of the pair to assign from
		 * @tparam U2 The type of the second value of the pair to assign from
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		template <typename U1, typename U2>
		constexpr const pair &operator=(const pair<U1, U2> &other) const
			requires(std::is_assignable_v<const T1 &, const U1 &> && std::is_assignable_v<const T2 &, const U2 &>)
		{
			first = other.first;
			second = other.second;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		constexpr pair &operator=(pair &&other)
			requires(std::is_move_assignable_v<T1> && std::is_move_assignable_v<T2>)
		{
			first = kstd::move(other.first);
			second = kstd::move(other.second);
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		constexpr const pair &operator=(pair &&other) const
			requires(std::is_move_assignable_v<const T1> && std::is_move_assignable_v<const T2>)
		{
			first = kstd::move(other.first);
			second = kstd::move(other.second);
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @tparam U1 The type of the first value of the pair to assign from
		 * @tparam U2 The type of the second value of the pair to assign from
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		template <typename U1, typename U2>
		constexpr pair &operator=(pair<U1, U2> &&other)
			requires(std::is_assignable_v<T1 &, U1> && std::is_assignable_v<T2 &, U2>)
		{
			first = kstd::forward<U1>(other.first);
			second = kstd::forward<U2>(other.second);
			return *this;
		}

		/**
		 * @brief Assigns a new value to the pair
		 *
		 * @tparam U1 The type of the first value of the pair to assign from
		 * @tparam U2 The type of the second value of the pair to assign from
		 * @param other The pair to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/operator%3D @endlink
		 */
		template <typename U1, typename U2>
		constexpr const pair &operator=(pair<U1, U2> &&other) const
			requires(std::is_assignable_v<const T1 &, U1> && std::is_assignable_v<const T2 &, U2>)
		{
			first = kstd::forward<U1>(other.first);
			second = kstd::forward<U2>(other.second);
			return *this;
		}

		// template <pair-like P>
		// constexpr pair &operator=(P &&other);
		// TODO what is pair-like?

		// template<pair-like P>
		// constexpr const pair &operator=(P &&other) const;
		// TODO what is pair-like?
#pragma endregion

		/**
		 * @brief Swaps the values of two pairs
		 *
		 * @param other The pair to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/swap @endlink
		 */
		constexpr void swap(pair &other)
			requires(std::is_swappable_v<T1> && std::is_swappable_v<T2>)
		{
			using std::swap;
			if (this != &other) {
				swap(first, other.first);
				swap(second, other.second);
			}
		}

		/**
		 * @brief Swaps the values of two pairs
		 *
		 * @param other The pair to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/pair/swap @endlink
		 */
		constexpr void swap(const pair &other) const
			requires(std::is_swappable_v<const T1> && std::is_swappable_v<const T2>)
		{
			using std::swap;
			if (this != &other) {
				swap(first, other.first);
				swap(second, other.second);
			}
		}
	};

	// Deduction guides
	// https://en.cppreference.com/w/cpp/utility/pair/deduction_guides
	template <typename T1, typename T2>
	pair(T1, T2) -> pair<T1, T2>;

	// make_pair type helper
	template <typename T>
	struct _unwrap_ref_helper {
		using type = T;
	};
	template <typename T>
	struct _unwrap_ref_helper<std::reference_wrapper<T>> {
		using type = T &;
	};
	template <typename T>
	using _unwrap_ref_t = typename _unwrap_ref_helper<std::decay_t<T>>::type;

	/**
	 * @brief Creates a pair object from two values
	 *
	 * @tparam T1 The type of the first value
	 * @tparam T2 The type of the second value
	 * @param t1 The first value
	 * @param t2 The second value
	 * @return The pair object
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/pair/make_pair @endlink
	 */
	template <typename T1, typename T2>
	constexpr pair<_unwrap_ref_t<T1>, _unwrap_ref_t<T2>> make_pair(T1 &&t1, T2 &&t2)
		requires(std::is_constructible_v<_unwrap_ref_t<T1>, T1> && std::is_constructible_v<_unwrap_ref_t<T2>, T2>)
	{
		return pair<_unwrap_ref_t<T1>, _unwrap_ref_t<T2>>(kstd::forward<T1>(t1), kstd::forward<T2>(t2));
	}

	// TODO lexographical comparison operators

	/**
	 * @brief Swaps the values of two pairs objects
	 *
	 * @tparam T1 The type of the first value of the pair
	 * @tparam T2 The type of the second value of the pair
	 * @param lhs The first pair object
	 * @param rhs The second pair object
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/pair/swap2 @endlink
	 */
	template <typename T1, typename T2>
	constexpr void swap(pair<T1, T2> &lhs, pair<T1, T2> &rhs)
		requires(std::is_swappable_v<T1> && std::is_swappable_v<T2>)
	{
		lhs.swap(rhs);
	}

	/**
	 * @brief Swaps the values of two pairs objects
	 *
	 * @tparam T1 The type of the first value of the pair
	 * @tparam T2 The type of the second value of the pair
	 * @param lhs The first pair object
	 * @param rhs The second pair object
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/pair/swap2 @endlink
	 */
	template <typename T1, typename T2>
	constexpr void swap(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
		requires(std::is_swappable_v<const T1> && std::is_swappable_v<const T2>)
	{
		lhs.swap(rhs);
	}

	// TODO tuple_size
	// TODO tuple_element
	// TODO get
}