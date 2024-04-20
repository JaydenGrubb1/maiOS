/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-20
 * @brief Output iterator that prepends elements to a container
 * @link https://en.cppreference.com/w/cpp/iterator/front_insert_iterator @endlink
 * @link https://en.cppreference.com/w/cpp/iterator/front_inserter @endlink
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
	 * @brief Output iterator that prepends elements to a container
	 *
	 * @tparam Container The container type to append elements to
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/front_insert_iterator @endlink
	 */
	template <typename Container>
	class front_insert_iterator {
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
		 * @brief Constructs a front_insert_iterator object
		 *
		 * @param cont The container to append elements to
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/front_insert_iterator/front_insert_iterator @endlink
		 */
		explicit constexpr front_insert_iterator(Container &cont) : _container(std::addressof(cont)) {}

		/**
		 * @brief Prepends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the front_insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/front_insert_iterator/operator%3D @endlink
		 */
		constexpr front_insert_iterator &operator=(const typename Container::value_type &value) {
			_container->push_front(value);
			return *this;
		}

		/**
		 * @brief Prepends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the front_insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/front_insert_iterator/operator%3D @endlink
		 */
		constexpr front_insert_iterator &operator=(typename Container::value_type &&value) {
			_container->push_front(std::move(value));
			return *this;
		}

		// The following operators are required by the standard, but are not used in practice

		[[nodiscard]] constexpr front_insert_iterator &operator*(void) {
			return *this;
		}

		constexpr front_insert_iterator &operator++(void) {
			return *this;
		}

		constexpr front_insert_iterator operator++(int) {
			return *this;
		}
	};

	/**
	 * @brief Constructs a front_insert_iterator
	 *
	 * @tparam Container The container type to append elements to
	 * @param cont The container to append elements to
	 * @return A front_insert_iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/front_inserter @endlink
	 */
	template <typename Container>
	[[nodiscard]] constexpr front_insert_iterator<Container> front_inserter(Container &cont) {
		return front_insert_iterator<Container>(cont);
	}
}