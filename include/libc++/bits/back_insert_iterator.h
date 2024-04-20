/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-20
 * @brief Output iterator that appends elements to a container
 * @link https://en.cppreference.com/w/cpp/iterator/back_insert_iterator @endlink
 * @link https://en.cppreference.com/w/cpp/iterator/back_inserter @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/construct.h>
#include <bits/iterator_traits.h>

namespace std {
	/**
	 * @brief Output iterator that appends elements to a container
	 *
	 * @tparam Container The container type to append elements to
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/back_insert_iterator @endlink
	 */
	template <typename Container>
	class back_insert_iterator {
	  private:
		Container *_container;

	  public:
		using container_type = Container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = ptrdiff_t;
		using iterator_category = std::output_iterator_tag;

		/**
		 * @brief Constructs a back_insert_iterator object
		 *
		 * @param cont The container to append elements to
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/back_insert_iterator/back_insert_iterator @endlink
		 */
		explicit constexpr back_insert_iterator(Container &cont) : _container(std::addressof(cont)) {}

		/**
		 * @brief Appends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the back_insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/back_insert_iterator/operator%3D @endlink
		 */
		constexpr back_insert_iterator &operator=(const typename Container::value_type &value) {
			_container->push_back(value);
			return *this;
		}

		/**
		 * @brief Appends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the back_insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/back_insert_iterator/operator%3D @endlink
		 */
		constexpr back_insert_iterator &operator=(typename Container::value_type &&value) {
			_container->push_back(std::move(value));
			return *this;
		}

		// The following operators are required by the standard, but are not used in practice

		[[nodiscard]] constexpr back_insert_iterator &operator*(void) {
			return *this;
		}

		constexpr back_insert_iterator &operator++(void) {
			return *this;
		}

		constexpr back_insert_iterator operator++(int) {
			return *this;
		}
	};

	/**
	 * @brief Constructs a back_insert_iterator
	 *
	 * @tparam Container The container type to append elements to
	 * @param cont The container to append elements to
	 * @return A back_insert_iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/back_inserter @endlink
	 */
	template <typename Container>
	[[nodiscard]] constexpr back_insert_iterator<Container> back_inserter(Container &cont) {
		return back_insert_iterator<Container>(cont);
	}
}