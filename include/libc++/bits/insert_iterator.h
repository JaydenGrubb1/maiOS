/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-20
 * @brief Output iterator that inserts elements into a container
 * @link https://en.cppreference.com/w/cpp/iterator/insert_iterator @endlink
 * @link https://en.cppreference.com/w/cpp/iterator/inserter @endlink
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
	 * @brief Output iterator that inserts elements into a container
	 *
	 * @tparam Container The container type to append elements to
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/insert_iterator @endlink
	 */
	template <typename Container>
	class insert_iterator {
	  private:
		Container *_container;
		Container::iterator _iter;

	  public:
		using container_type = Container;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = ptrdiff_t;
		using iterator_category = std::output_iterator_tag;

		/**
		 * @brief Constructs a insert_iterator object
		 *
		 * @param cont The container to append elements to
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/insert_iterator/insert_iterator @endlink
		 */
		explicit constexpr insert_iterator(Container &cont, typename Container::iterator iter) : _container(std::addressof(cont)), _iter(iter) {}

		/**
		 * @brief Prepends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/insert_iterator/operator%3D @endlink
		 */
		constexpr insert_iterator &operator=(const typename Container::value_type &value) {
			_container->insert(_iter, value);
			_iter++;
			return *this;
		}

		/**
		 * @brief Prepends an element to the container
		 *
		 * @param value The value to append to the container
		 * @return A reference to the insert_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/insert_iterator/operator%3D @endlink
		 */
		constexpr insert_iterator &operator=(typename Container::value_type &&value) {
			_container->insert(_iter, std::move(value));
			_iter++;
			return *this;
		}

		// The following operators are required by the standard, but are not used in practice

		[[nodiscard]] constexpr insert_iterator &operator*() {
			return *this;
		}

		constexpr insert_iterator &operator++() {
			return *this;
		}

		constexpr insert_iterator operator++(int) {
			return *this;
		}
	};

	/**
	 * @brief Constructs a insert_iterator
	 *
	 * @tparam Container The container type to append elements to
	 * @param cont The container to append elements to
	 * @return A insert_iterator
	 *
	 * @link https://en.cppreference.com/w/cpp/iterator/inserter @endlink
	 */
	template <typename Container>
	[[nodiscard]] constexpr insert_iterator<Container> inserter(Container &cont, typename Container::iterator iter) {
		return insert_iterator<Container>(cont, iter);
	}
}