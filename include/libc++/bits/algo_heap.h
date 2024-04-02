/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-02
 * @brief Various algorithms for working with heaps
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace std {
	/**
	 * @brief Check if the range [first, last) is a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 * @return true if the range is a heap, false otherwise
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/is_heap @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr bool is_heap(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
		return false;
	}

	/**
	 * @brief Check if the range [first, last) is a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 * @return true if the range is a heap, false otherwise
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/is_heap @endlink
	 */
	template <typename Iter>
	constexpr bool is_heap(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
		return false;
	}

	/**
	 * @brief Finds the largest subrange in the range [first, last) that is a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 * @return The end of the largest subrange that is a heap
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/is_heap_until @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr Iter is_heap_until(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
		return first;
	}

	/**
	 * @brief Finds the largest subrange in the range [first, last) that is a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 * @return The end of the largest subrange that is a heap
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/is_heap_until @endlink
	 */
	template <typename Iter>
	constexpr Iter is_heap_until(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
		return first;
	}

	/**
	 * @brief Constructs a heap in the range [first, last)
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/make_heap @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr void make_heap(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
	}

	/**
	 * @brief Constructs a heap in the range [first, last)
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/make_heap @endlink
	 */
	template <typename Iter>
	constexpr void make_heap(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
	}

	/**
	 * @brief Inserts the element at last-1 into the heap in the range [first, last)
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/push_heap @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr void push_heap(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
	}

	/**
	 * @brief Inserts the element at last-1 into the heap in the range [first, last)
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/push_heap @endlink
	 */

	template <typename Iter>
	constexpr void push_heap(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
	}

	/**
	 * @brief Swaps the element at first with the element at last-1 and then makes the range [first, last-1) into a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/pop_heap @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr void pop_heap(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
	}

	/**
	 * @brief Swaps the element at first with the element at last-1 and then makes the range [first, last-1) into a heap
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/pop_heap @endlink
	 */
	template <typename Iter>
	constexpr void pop_heap(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
	}

	/**
	 * @brief Sorts the heap in the range [first, last) into a sorted range
	 *
	 * @tparam Iter The type of the iterator
	 * @tparam Compare The type of the comparison function
	 * @param first The start of the range
	 * @param last The end of the range
	 * @param comp The comparison function
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/sort_heap @endlink
	 */
	template <typename Iter, typename Compare>
	constexpr void sort_heap(Iter first, Iter last, Compare comp) {
		// TODO implement this
		(void)first;
		(void)last;
		(void)comp;
	}

	/**
	 * @brief Sorts the heap in the range [first, last) into a sorted range
	 *
	 * @tparam Iter The type of the iterator
	 * @param first The start of the range
	 * @param last The end of the range
	 *
	 * @link https://en.cppreference.com/w/cpp/algorithm/sort_heap @endlink
	 */
	template <typename Iter>
	constexpr void sort_heap(Iter first, Iter last) {
		// TODO implement this
		(void)first;
		(void)last;
	}
}