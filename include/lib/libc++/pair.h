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
	class paid {
	  public:
		constexpr explicit pair();

		constexpr explicit pair(const T1 &x, const T2 &y);

		template <typename U1 = T1, typename U2 = T2>
		constexpr explicit pair(U1 &&x, U2 &&y);

		template <typename U1, typename U2>
		constexpr explicit pair(pair<U1, U2> &p);

		template <typename U1, typename U2>
		constexpr explicit pair(const pair<U1, U2> &p);

		template <typename U1, typename U2>
		constexpr explicit pair(pair<U1, U2> &&p);

		template <typename U1, typename U2>
		constexpr explicit pair(const pair<U1, U2> &&p);

		// template <pair-like P>
		// constexpr explicit pair(P &&u);
		// // TODO what is pair-like?

		// template <typename... Args1, typename... Args2>
		// constexpr pair(std::piecewise_construct_t, tuple<Args1...> arg1, tuple<Args2...> arg2);
		// // TODO what is piecewise_construct_t?

		pair(const pair &p) = default;

		pair(pair &&p) = default;
	};
}