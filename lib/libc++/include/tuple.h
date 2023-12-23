/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Class template for a fixed-size collection of heterogeneous values
 * @link https://en.cppreference.com/w/cpp/utility/tuple @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

namespace kstd {
	/**
	 * @brief Class template for a fixed-size collection of heterogeneous values
	 *
	 * @tparam Types The types of the values to be stored
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/tuple @endlink
	 */
	template <typename... Types>
	class tuple {
	  public:
		constexpr explicit tuple(void);

		constexpr explicit tuple(const Types &...);

		template <typename... UTypes>
		constexpr explicit tuple(UTypes &&...args);

		template <typename... UTypes>
		constexpr explicit tuple(tuple<UTypes...> &other);

		template <typename... UTypes>
		constexpr explicit tuple(const tuple<UTypes...> &other);

		template <typename... UTypes>
		constexpr explicit tuple(tuple<UTypes...> &&other);

		template <typename... UTypes>
		constexpr explicit tuple(const tuple<UTypes...> &&other);

		template <typename U1, typename U2>
		constexpr explicit tuple(pair<U1, U2> &p);

		template <typename U1, typename U2>
		constexpr explicit tuple(const pair<U1, U2> &p);

		template <typename U1, typename U2>
		constexpr explicit tuple(pair<U1, U2> &&p);

		template <typename U1, typename U2>
		constexpr explicit tuple(const std::pair<U1, U2> &&p);

		// template <tuple-like UTuple>
		// constexpr explicit tuple(UTuple &&u);
		// // TODO what is tuple-like?

		tuple(const tuple &other) = default;

		tuple(tuple &&other) = default;

		// TODO allocator extended constructors ?
	};
}