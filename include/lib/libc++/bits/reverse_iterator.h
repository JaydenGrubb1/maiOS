/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-10
 * @brief Iterator adaptor for reversing the direction of a given iterator
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <type_traits>

namespace kstd {
	template <typename>
	class reverse_iterator;

	/**
	 * @brief Iterator adaptor for reversing the direction of a given iterator
	 *
	 * @tparam T The type of the iterator to be reversed
	 */
	template <typename T>
	class reverse_iterator {
	  private:
		T _iterator;

		template <typename U>
		friend class reverse_iterator;

	  public:
		/**
		 * @brief Constructs a reverse_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		constexpr reverse_iterator() = default;

		/**
		 * @brief Constructs a reverse_iterator from the given iterator
		 *
		 * @param iterator The iterator to be reversed
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		constexpr explicit reverse_iterator(T iterator) : _iterator(iterator) {}

		/**
		 * @brief Constructs a reverse_iterator from the given reverse_iterator
		 *
		 * @tparam U The type of the iterator to be reversed
		 * @param other The reverse_iterator to be reversed
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/reverse_iterator @endlink
		 */
		template <typename U>
		constexpr reverse_iterator(const reverse_iterator<U> &other)
			requires(!std::is_same_v<T, U>)
			: _iterator(other._iterator) {}
		// TODO std::convertible_to<const U&, Iter>

		/**
		 * @brief Assigns the given iterator to this reverse_iterator
		 *
		 * @tparam U The type of the iterator to be assigned
		 * @param other The iterator to be assigned
		 * @return A reference to this reverse_iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/operator%3D @endlink
		 */
		template <class U>
		constexpr reverse_iterator &operator=(const reverse_iterator<U> &other)
			requires(!std::is_same_v<T, U>)
		{
			_iterator = other._iterator;
			return *this;
		}
		// TODO std::convertible_to<const U&, Iter>
		// TODO std::assignable_from<Iter&, const U&>

		/**
		 * @brief Returns the underlying iterator
		 *
		 * @return The underlying iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/iterator/reverse_iterator/base @endlink
		 */
		constexpr T base() const {
			return _iterator;
		}

		constexpr T operator*() const {
			// FIXME
			T tmp = _iterator;
			return --tmp;
		}

		constexpr T operator->() const
			requires(std::is_pointer_v<T> || requires(const T i) { i.operator->(); })
		{
			// FIXME
			T tmp = _iterator;
			return (--tmp).operator->();
		}
	};
}