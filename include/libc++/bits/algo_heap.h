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

#include <bits/algo_basic.h>
#include <functional>
#include <iterator>

namespace std {
	namespace __detail {
		template <typename Iter, typename Compare, typename Dist>
		constexpr void __heapify(Iter first, Dist len, Dist pos, Compare &comp) {
			Dist largest = pos;
			Dist left = 2 * pos + 1;
			Dist right = 2 * pos + 2;

			while (true) {
				if (left < len && !std::invoke(comp, *(first + left), *(first + largest))) {
					largest = left;
				}

				if (right < len && !std::invoke(comp, *(first + right), *(first + largest))) {
					largest = right;
				}

				if (largest == pos) {
					break;
				}

				std::iter_swap(first + pos, first + largest);
				pos = largest;
				left = 2 * pos + 1;
				right = 2 * pos + 2;
			}
		}

		template <typename Iter, typename Compare, typename Dist>
		constexpr Dist __is_heap_until(Iter first, Dist len, Compare &comp) {
			Dist parent = 0;

			for (Dist child = 1; child < len; ++child) {
				if (std::invoke(comp, *(first + parent), *(first + child))) {
					return child;
				}

				if ((child & 1) == 0) {
					++parent;
				}
			}

			return len;
		}

		template <typename Iter, typename Compare>
		constexpr void __make_heap(Iter first, Iter last, Compare &comp) {
			auto len = std::distance(first, last);
			auto start = (len - 2) / 2;

			for (auto i = start; i >= 0; --i) {
				__heapify(first, len, i, comp);
			}
		}

		template <typename Iter, typename Compare>
		constexpr void __push_heap(Iter first, Iter last, Compare &comp) {
			auto len = std::distance(first, last);
			auto pos = len - 1;

			while (pos > 0) {
				auto parent = (pos - 1) / 2;

				if (std::invoke(comp, *(first + parent), *(first + pos))) {
					std::iter_swap(first + parent, first + pos);
					pos = parent;
				} else {
					break;
				}
			}
		}

		template <typename Iter, typename Compare>
		constexpr void __pop_heap(Iter first, Iter last, Compare &comp) {
			auto len = std::distance(first, last);
			std::iter_swap(first, last - 1);
			__heapify(first, len - 1, static_cast<decltype(len)>(0), comp);
		}

		template <typename Iter, typename Compare>
		constexpr void __sort_heap(Iter first, Iter last, Compare &comp) {
			while (last - first > 1) {
				__pop_heap(first, last--, comp);
			}
		}
	}

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
	[[nodiscard]] constexpr inline bool is_heap(Iter first, Iter last, Compare comp) {
		auto dist = std::distance(first, last);
		return __detail::__is_heap_until(first, dist, comp) == dist;
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
	[[nodiscard]] constexpr inline bool is_heap(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		auto dist = std::distance(first, last);
		return __detail::__is_heap_until(first, dist, comp) == dist;
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
	[[nodiscard]] constexpr inline Iter is_heap_until(Iter first, Iter last, Compare comp) {
		return first + __detail::__is_heap_until(first, std::distance(first, last), comp);
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
	[[nodiscard]] constexpr inline Iter is_heap_until(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		return first + __detail::__is_heap_until(first, std::distance(first, last), comp);
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
	constexpr inline void make_heap(Iter first, Iter last, Compare comp) {
		__detail::__make_heap(first, last, comp);
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
	constexpr inline void make_heap(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		__detail::__make_heap(first, last, comp);
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
	constexpr inline void push_heap(Iter first, Iter last, Compare comp) {
		__detail::__push_heap(first, last, comp);
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
	constexpr inline void push_heap(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		__detail::__push_heap(first, last, comp);
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
	constexpr inline void pop_heap(Iter first, Iter last, Compare comp) {
		__detail::__pop_heap(first, last, comp);
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
	constexpr inline void pop_heap(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		__detail::__pop_heap(first, last, comp);
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
	constexpr inline void sort_heap(Iter first, Iter last, Compare comp) {
		__detail::__sort_heap(first, last, comp);
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
	constexpr inline void sort_heap(Iter first, Iter last) {
		std::less<typename std::iterator_traits<Iter>::value_type> comp;
		__detail::__sort_heap(first, last, comp);
	}
}