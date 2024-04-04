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

#include <utility>

namespace std {
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
	[[nodiscard]] constexpr inline const T &min(const T &a, const T &b) {
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
	[[nodiscard]] constexpr inline const T &max(const T &a, const T &b) {
		return (a > b) ? a : b;
	}

	// TODO max with comparer
	// TODO max with initializer list
	// TODO max with initializer list and comparer

	/**
	 * @brief Moves the elements in the range [src_first, src_last) to another range beginning at dest_first
	 *
	 * @tparam IterIn The type of the input iterator
	 * @tparam IterOut The type of the output iterator
	 * @param src_first The beginning of the source range
	 * @param src_last The end of the source range
	 * @param dest_first The beginning of the destination range
	 * @return The end of the destination range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/move @endlink
	 */
	template <typename IterIn, typename IterOut>
	constexpr IterOut move(IterIn src_first, IterIn src_last, IterOut dest_first) {
		while (src_first != src_last) {
			*dest_first++ = std::move(*src_first++);
		}
		return dest_first;
	}

	/**
	 * @brief Moves the elements in the range [src_first, src_last) to another range ending at dest_last
	 *
	 * @tparam IterIn The type of the input iterator
	 * @tparam IterOut The type of the output iterator
	 * @param src_first The beginning of the source range
	 * @param src_last The end of the source range
	 * @param dest_last The end of the destination range
	 * @return The beginning of the destination range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/move_backward @endlink
	 */
	template <typename IterIn, typename IterOut>
	constexpr IterOut move_backward(IterIn src_first, IterIn src_last, IterOut dest_last) {
		while (src_first != src_last) {
			*--dest_last = std::move(*--src_last);
		}
		return dest_last;
	}

	// TODO copy
	// TODO copy_backward
	// TODO copy_if
	// TODO copy_n

	/**
	 * @brief Swaps the values of two elements
	 *
	 * @tparam Iter1 The type of the first iterator
	 * @tparam Iter2 The type of the second iterator
	 * @param lhs The first iterator
	 * @param rhs The second iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/iter_swap @endlink
	 */
	template <typename Iter1, typename Iter2>
	constexpr inline void iter_swap(Iter1 lhs, Iter2 rhs) {
		using std::swap;
		swap(*lhs, *rhs);
	}
}