/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-30
 * @brief Basic algorithms and utilities
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace kstd {
	/**
	 * @brief Returns the smaller of the two values
	 *
	 * @tparam T The type of the values
	 * @param a The first value
	 * @param b The second value
	 * @return The smaller of the two values
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/min @endlink
	 */
	template <typename T>
	constexpr inline const T &min(const T &a, const T &b) {
		return (a < b) ? a : b;
	}

	// TODO min with comparer
	// TODO min with initializer list
	// TODO min with initializer list and comparer

	/**
	 * @brief Returns the larger of the two values
	 *
	 * @tparam T The type of the values
	 * @param a The first value
	 * @param b The second value
	 * @return The larger of the two values
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/max @endlink
	 */
	template <typename T>
	constexpr inline const T &max(const T &a, const T &b) {
		return (a > b) ? a : b;
	}

	// TODO max with comparer
	// TODO max with initializer list
	// TODO max with initializer list and comparer
}