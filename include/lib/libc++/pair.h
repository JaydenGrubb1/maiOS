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
#include <utility> // VERIFY Use this or custom <utility>?
// #include <lib/libc++/utility.h>

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
		constexpr explicit pair()
			requires(std::is_default_constructible_v<T1> && std::is_default_constructible_v<T2>)
			: first(), second() {}
		// TODO check implicit default constructor

		constexpr explicit(!std::is_convertible_v<const T1 &, T1> || !std::is_convertible_v<const T2 &, T2>)
			pair(const T1 &x, const T2 &y)
			requires(std::is_copy_constructible_v<T1> && std::is_copy_constructible_v<T2>)
			: first(x), second(y) {}

		template <typename U1 = T1, typename U2 = T2>
		constexpr explicit(!std::is_convertible_v<U1, T1> || !std::is_convertible_v<U2, T2>)
			pair(U1 &&x, U2 &&y)
			requires(std::is_constructible_v<T1, U1> && std::is_constructible_v<T2, U2>)
			: first(std::forward<U1>(x)), second(std::forward<U2>(y)) {}

		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<const U1 &, T1> || !std::is_convertible_v<const U2 &, T2>)
			pair(pair<U1, U2> &p)
			requires(std::is_constructible_v<T1, const U1 &> && std::is_constructible_v<T2, const U2 &>)
			: first(p.first), second(p.second) {}
		// TODO delete if initialization would bind to a reference to a temporary

		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<U1 &, T1> || !std::is_convertible_v<U2 &, T2>)
			pair(const pair<U1, U2> &p)
			requires(std::is_constructible_v<T1, U1 &> && std::is_constructible_v<T2, U2 &>)
			: first(p.first), second(p.second) {}
		// TODO delete if initialization would bind to a reference to a temporary

		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<U1 &&, T1> || !std::is_convertible_v<U2 &&, T2>)
			pair(pair<U1, U2> &&p)
			requires(std::is_constructible_v<T1, U1 &&> && std::is_constructible_v<T2, U2 &&>)
			: first(std::forward<U1>(p.first)), second(std::forward<U2>(p.second)) {}
		// TODO delete if initialization would bind to a reference to a temporary

		template <typename U1, typename U2>
		constexpr explicit(!std::is_convertible_v<const U1 &&, T1> || !std::is_convertible_v<const U2 &&, T2>)
			pair(const pair<U1, U2> &&p)
			requires(std::is_constructible_v<T1, const U1 &&> && std::is_constructible_v<T2, const U2 &&>)
			: first(std::forward<U1>(p.first)), second(std::forward<U2>(p.second)) {}
		// TODO delete if initialization would bind to a reference to a temporary

		// template <pair-like P>
		// constexpr explicit pair(P &&u);
		// // TODO what is pair-like?

		// template <typename... Args1, typename... Args2>
		// constexpr pair(std::piecewise_construct_t, tuple<Args1...> arg1, tuple<Args2...> arg2);
		// // TODO what is piecewise_construct_t?

		constexpr pair(const pair &p) = default;

		constexpr pair(pair &&p) = default;
#pragma endregion
	};
}