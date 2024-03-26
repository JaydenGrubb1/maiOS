/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-26
 * @brief Provides trait information for iterators
 * @link https://en.cppreference.com/w/cpp/iterator/iterator_traits @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstddef>
#include <type_traits>

namespace std {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag : public random_access_iterator_tag {};

	/**
	 * @brief Provides trait information for iterators
	 *
	 * @tparam Iter The type of the iterator to provide traits for
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/iterator_traits @endlink
	 */
	template <typename Iter>
	struct iterator_traits {
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using difference_type = typename Iter::difference_type;
		using iterator_category = typename Iter::iterator_category;
	};

	/**
	 * @brief Provides trait information for pointers
	 *
	 * @tparam T The type of the pointer to provide traits for
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/iterator_traits @endlink
	 */
	template <typename T>
		requires(std::is_object_v<T>)
	struct iterator_traits<T *> {
		using value_type = std::remove_cv_t<T>;
		using pointer = T *;
		using reference = T &;
		using difference_type = ptrdiff_t;
		using iterator_category = random_access_iterator_tag;
	};
}