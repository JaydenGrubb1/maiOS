/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-08-01
 * @brief Class template for objects that may contain either an error or a value
 * @link https://en.cppreference.com/w/cpp/utility/expected @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <utility> // VERIFY Use this or custom <utility>?
// #include <lib/libc++/utility.h>

namespace kstd {
	/**
	 * @brief Tag type used to indicate that an unexpected value was encountered
	 *
	 */
	struct unexpect_t {
		explicit unexpect_t(void) = default;
	};

	/**
	 * @brief Constant of type unexpect_t that is used to indicate an unexpected value
	 *
	 */
	inline constexpr unexpect_t unexpect{};

	/**
	 * @brief Class template for objects that may contain either an error or a value
	 *
	 * @tparam T The type of the expected value
	 * @tparam E The type of the expected error
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/expected @endlink
	 */
	template <typename T, typename E>
	class expected {
	  private:
		bool _has_value;
		union {
			T _value;
			E _error;
		};

	  public:
		/**
		 * @brief Construct a new expected object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/expected/expected @endlink
		 */
		constexpr expected(void)
			: _has_value(true), _value() {}
		// TODO check if void or is_default_constructible_v<T> is true

		/**
		 * @brief Construct a new expected object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/expected/expected @endlink
		 */
		constexpr expected(const expected &other)
			: _has_value(other._has_value) {
			if (_has_value) {
				_value = other._value;
			} else {
				_error = other._error;
			}
		}
		// TODO check is_void, is_copy_constructible_v<T> and/or is_trivially_copy_constructible_v<T>

		/**
		 * @brief Construct a new expected object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/expected/expected @endlink
		 */
		constexpr expected(expected &&other)
			: _has_value(other._has_value) {
			if (_has_value) {
				_value = std::move(other._value);
			} else {
				_error = std::move(other._error);
			}
		}
		// TODO check is_void, is_move_constructible_v<T> and/or is_trivially_move_constructible_v<T>

		/**
		 * @brief Construct a new expected object
		 *
		 * @tparam U The type of the expected value
		 * @tparam G The type of the expected error
		 */
		template <typename U, typename G>
		constexpr explicit expected(const expected<U, G> &other)
			: _has_value(other._has_value) {
			if (_has_value) {
				_value = other._value;
			} else {
				_error = other._error;
			}
		}
		// TODO check the spec
		// TODO add explicit arguments

		/**
		 * @brief Construct a new expected object
		 *
		 * @tparam U The type of the expected value
		 * @tparam G The type of the expected error
		 */
		template <typename U, typename G>
		constexpr explicit expected(expected<U, G> &&other)
			: _has_value(other._has_value) {
			if (_has_value) {
				_value = std::move(other._value);
			} else {
				_error = std::move(other._error);
			}
		}
		// TODO check the spec
		// TODO add explicit arguments

		// TODO missing expected constructors

		// TODO assigment operators

		/**
		 * @brief Destroy the expected object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/expected/~expected @endlink
		 */
		constexpr ~expected() {
			if (_has_value) {
				_value.~T();
			} else {
				_error.~E();
			}
		}
		// TODO check is_void and/or is_trivially_destructible_v<T>
	};
}