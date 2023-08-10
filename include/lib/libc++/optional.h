/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-31
 * @brief Class template for objects that may or may not contain a value
 * @link https://en.cppreference.com/w/cpp/utility/optional @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <initializer_list>
#include <lib/libc++/bits/placement_new.h>
#include <lib/libc/assert.h>
#include <type_traits>
#include <utility> // VERIFY Use this or custom <utility>?
// #include <lib/libc++/utility.h>

namespace kstd {
	/**
	 * @brief Tag type used to indicate an empty optional
	 *
	 */
	struct nullopt_t {
		enum class _construct { _token };
		explicit constexpr nullopt_t(_construct) {}
	};

	/**
	 * @brief Constant of type nullopt_t that is used to indicate an empty optional
	 *
	 */
	inline constexpr nullopt_t nullopt{nullopt_t::_construct::_token};

	template <typename>
	class optional;

	/**
	 * @brief Class template for objects that may or may not contain a value
	 *
	 * @tparam T The type of the value to be stored
	 */
	template <typename T>
	class optional {
		static_assert(!std::is_same_v<std::remove_cv_t<T>, nullopt_t>);
		static_assert(!std::is_same_v<std::remove_cv_t<T>, std::in_place_t>);
		static_assert(!std::is_reference_v<T>);

	  private:
		bool _has_value;
		union {
			T _value;
		}; // union allows <optional> to exist without constructing T

		template <typename U>
		friend class optional;

	  public:
#pragma region Constructors
		/**
		 * @brief Construct a new optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(void)
			: _has_value(false) {}

		/**
		 * @brief Construct a new optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(nullopt_t)
			: _has_value(false) {}

		/**
		 * @brief Construct a new optional object
		 *
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(const optional &other) = default;

		/**
		 * @brief Construct a new optional object
		 *
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(const optional &other)
			requires(std::is_copy_constructible_v<T> && !std::is_trivially_copy_constructible_v<T>)
			: _has_value(other._has_value) {
			if (other._has_value) {
				new (&_value) T(other._value);
			}
		}

		/**
		 * @brief Construct a new optional object
		 *
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(optional &&other) = default;

		/**
		 * @brief Construct a new optional object
		 *
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		constexpr optional(optional &&other)
			requires(std::is_move_constructible_v<T> && !std::is_trivially_move_constructible_v<T>)
			: _has_value(other._has_value) {
			if (other._has_value) {
				new (&_value) T(std::move(other._value));
			}
		}

		/**
		 * @brief Construct a new optional object
		 *
		 * @tparam T The type of the value to be stored
		 * @tparam U The type of the object to construct from
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		template <typename U>
		constexpr explicit(!std::is_convertible_v<const U &, T>) optional(const optional<U> &other)
			requires(std::is_constructible_v<T, const U &>)
			: _has_value(other._has_value) {
			if (other._has_value) {
				new (&_value) T(other._value);
			}
		}

		/**
		 * @brief Construct a new optional object
		 *
		 * @tparam T The type of the value to be stored
		 * @tparam U The type of the object to construct from
		 * @param other The optional object to construct from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		template <typename U>
		constexpr explicit(!std::is_convertible_v<U &&, T>) optional(optional<U> &&other)
			: _has_value(other._has_value) {
			if (other._has_value) {
				new (&_value) T(std::move(other._value));
			}
		}

		/**
		 * @brief Construct a new optional object
		 *
		 * @tparam Args The types of the arguments to be passed to the constructor of T
		 * @param args The arguments to be passed to the constructor of T
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		template <typename... Args>
		constexpr explicit optional(std::in_place_t, Args &&...args)
			requires(std::is_constructible_v<T, Args...>)
			: _has_value(true), _value(std::forward<Args>(args)...) {}

		/**
		 * @brief Construct a new optional object
		 *
		 * @tparam U The type of the value to be stored
		 * @tparam Args The types of the arguments to be passed to the constructor of T
		 * @param list The initializer list to be passed to the constructor of T
		 * @param args The arguments to be passed to the constructor of T
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		template <typename U, typename... Args>
		constexpr explicit optional(std::in_place_t, std::initializer_list<U> list, Args &&...args)
			requires(std::is_constructible_v<T, std::initializer_list<U> &, Args...>)
			: _has_value(true), _value(list, std::forward<Args>(args)...) {}

		/**
		 * @brief Construct a new optional object
		 *
		 * @tparam U The type of the value to be stored
		 * @param value The value to be stored
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/optional @endlink
		 */
		template <typename U = T>
		constexpr explicit(!std::is_convertible_v<U &&, T>) optional(U &&value)
			requires(std::is_constructible_v<T, U &&> &&
					 !std::is_same_v<std::remove_cvref_t<U>, std::in_place_t> &&
					 !std::is_same_v<std::remove_cvref_t<U>, optional<T>>)
			: _has_value(true) {
			new (&_value) T(std::forward<U>(value));
		}
#pragma endregion

#pragma region Destructors
		/**
		 * @brief Destroy the optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/~optional @endlink
		 */
		constexpr ~optional() = default;

		/**
		 * @brief Destroy the optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/~optional @endlink
		 */
		constexpr ~optional()
			requires(!std::is_trivially_destructible_v<T>)
		{
			if (_has_value) {
				_value.~T();
			}
		}
#pragma endregion

#pragma region Assignment Operators
		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @return The optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		constexpr optional &operator=(nullopt_t) {
			if (_has_value) {
				_value.~T();
				_has_value = false;
			}
			return *this;
		}

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @param other The optional object to assign from
		 * @return The optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		constexpr optional &operator=(const optional &other) = default;

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @param other The optional object to assign from
		 * @return The optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		constexpr optional &operator=(const optional &other)
			requires(std::is_copy_constructible_v<T> &&
					 std::is_copy_assignable_v<T> &&
					 std::is_trivially_copy_constructible_v<T> &&
					 std::is_trivially_copy_assignable_v<T> &&
					 std::is_trivially_destructible_v<T>)
		{
			if (_has_value && !other._has_value) {
				_value.~T();
			} else if (other._has_value) {
				if (_has_value) {
					_value = other._value;
				} else {
					new (&_value) T(other._value);
				}
			}
			_has_value = other._has_value;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @param other The optional object to assign from
		 * @return The optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		constexpr optional &operator=(optional &&other) = default;

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @param other The optional object to assign from
		 * @return The optional object
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		constexpr optional &operator=(optional &&other)
			requires(std::is_move_constructible_v<T> &&
					 std::is_move_assignable_v<T> &&
					 std::is_trivially_move_constructible_v<T> &&
					 std::is_trivially_move_assignable_v<T> &&
					 std::is_trivially_destructible_v<T>)
		{
			if (_has_value && !other._has_value) {
				_value.~T();
			} else if (other._has_value) {
				if (_has_value) {
					_value = std::move(other._value);
				} else {
					new (&_value) T(std::move(other._value));
				}
			}
			_has_value = other._has_value;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @tparam U The type of the value to be stored
		 * @param value The value to be stored
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		template <typename U = T>
		constexpr optional &operator=(U &&value)
			requires(!std::is_same_v<std::remove_cvref_t<U>, optional<T>> &&
					 std::is_assignable_v<T &, U> &&
					 std::is_constructible_v<T, U> &&
					 (!std::is_scalar_v<T> || !std::is_same_v<T, std::decay_t<U>>))
		{
			if (_has_value) {
				_value = std::forward<U>(value);
			} else {
				new (&_value) T(std::forward<U>(value));
			}
			_has_value = true;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @tparam U The type of the value to be stored
		 * @param other The optional object to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		template <typename U>
		constexpr optional &operator=(const optional<U> &other)
			requires(std::is_constructible_v<T, const U &> && std::is_assignable_v<T &, const U &>)
		{
			if (_has_value && !other._has_value) {
				_value.~T();
			} else if (other._has_value) {
				if (_has_value) {
					_value = other._value;
				} else {
					new (&_value) T(other._value);
				}
			}
			_has_value = other._has_value;
			return *this;
		}

		/**
		 * @brief Assigns a new value to the optional object
		 *
		 * @tparam U The type of the value to be stored
		 * @param other The optional object to assign from
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator%3D @endlink
		 */
		template <typename U>
		constexpr optional &operator=(optional<U> &&other)
			requires(std::is_constructible_v<T, U> && std::is_assignable_v<T &, U>)
		{
			if (_has_value && !other._has_value) {
				_value.~T();
			} else if (other._has_value) {
				if (_has_value) {
					_value = std::move(other._value);
				} else {
					new (&_value) T(std::move(other._value));
				}
			}
			_has_value = other._has_value;
			return *this;
		}
#pragma endregion

#pragma region Value Functions
		/**
		 * @brief Checks if the optional object has a value
		 *
		 * @return true if the optional object has a value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator_bool @endlink
		 */
		constexpr explicit operator bool(void) const {
			return _has_value;
		}

		/**
		 * @brief Checks if the optional object has a value
		 *
		 * @return true if the optional object has a value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator_bool @endlink
		 */
		constexpr bool has_value(void) const {
			return _has_value;
		}

		/**
		 * @brief Returns a pointer to the value
		 *
		 * @return A pointer to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr const T *operator->(void) const {
			return &_value;
		}

		/**
		 * @brief Returns a pointer to the value
		 *
		 * @return A pointer to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr T *operator->(void) {
			return &_value;
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr const T &operator*(void) const & {
			return _value;
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr T &operator*(void) & {
			return _value;
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr const T &&operator*(void) const && {
			return std::forward<const T>(_value);
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/operator* @endlink
		 */
		constexpr T &&operator*(void) && {
			return std::forward<T>(_value);
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value @endlink
		 */
		constexpr T &value(void) & {
			assert(_has_value);
			return _value;
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value @endlink
		 */
		constexpr const T &value(void) const & {
			assert(_has_value);
			return _value;
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value @endlink
		 */
		constexpr T &&value(void) && {
			assert(_has_value);
			return std::forward<T>(_value);
		}

		/**
		 * @brief Returns a reference to the value
		 *
		 * @return A reference to the value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value @endlink
		 */
		constexpr const T &&value(void) const && {
			assert(_has_value);
			return std::forward<T>(_value);
		}

		/**
		 * @brief Returns either a reference to the value or the default value
		 *
		 * @tparam U The type of the default value
		 * @param default_value The default value
		 * @return Either a reference to the value or the default value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value_or @endlink
		 */
		template <typename U>
		constexpr T value_or(U &&default_value) const & {
			if (_has_value) {
				return _value;
			}
			return std::forward<U>(default_value);
		}

		/**
		 * @brief Returns either a reference to the value or the default value
		 *
		 * @tparam U The type of the default value
		 * @param default_value The default value
		 * @return Either a reference to the value or the default value
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/value_or @endlink
		 */
		template <typename U>
		constexpr T value_or(U &&default_value) && {
			if (_has_value) {
				return std::move(_value);
			}
			return std::forward<U>(default_value);
		}
#pragma endregion

		// TODO monadic operations

		/**
		 * @brief Swaps the values of two optionals
		 *
		 * @param other The other optional
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/swap @endlink
		 */
		constexpr void swap(optional &other) {
			if (_has_value && other._has_value) {
				using std::swap;
				swap(**this, *other);
			} else if (_has_value) {
				new (&other._value) T(std::move(_value));
				_value.~T();
				other._has_value = true;
				_has_value = false;
			} else if (other._has_value) {
				new (&_value) T(std::move(other._value));
				other._value.~T();
				_has_value = true;
				other._has_value = false;
			}
		}

		/**
		 * @brief Destroys the value if it exists
		 *
		 * @link https://en.cppreference.com/w/cpp/utility/optional/reset @endlink
		 */
		constexpr void reset(void) {
			if (_has_value) {
				_value.~T();
				_has_value = false;
			}
		}

		/**
		 * @brief Constructs the value in-place
		 *
		 * @tparam Args The types of the arguments to be passed to the constructor of T
		 * @param args The arguments to be passed to the constructor of T
		 * @return A reference to the value
		 */
		template <class... Args>
		constexpr T &emplace(Args &&...args) {
			if (_has_value) {
				_value.~T();
			}
			new (&_value) T(std::forward<Args>(args)...);
			_has_value = true;
			return _value;
		}

		/**
		 * @brief Constructs the value in-place
		 *
		 * @tparam U The type of the initializer list
		 * @tparam Args The types of the arguments to be passed to the constructor of T
		 * @param list The initializer list to be passed to the constructor of T
		 * @param args The arguments to be passed to the constructor of T
		 * @return A reference to the value
		 */
		template <class U, class... Args>
		constexpr T &emplace(std::initializer_list<U> ilist, Args &&...args)
			requires(std::is_constructible_v<T, std::initializer_list<U> &, Args && ...>)
		{
			if (_has_value) {
				_value.~T();
			}
			_value = T(ilist, std::forward<Args>(args)...);
			_has_value = true;
			return _value;
		}
	};

// Comparison operators
// https://en.cppreference.com/w/cpp/utility/optional/operator_cmp
#pragma region Comparison Operators
	template <typename T, typename U>
	constexpr bool operator==(const optional<T> &lhs, const optional<U> &rhs) {
		if (lhs.has_value() != rhs.has_value()) {
			return false;
		}
		if (lhs.has_value()) {
			return *lhs == *rhs;
		}
		return true;
	}

	template <typename T, typename U>
	constexpr bool operator!=(const optional<T> &lhs, const optional<U> &rhs) {
		if (lhs.has_value() != rhs.has_value()) {
			return true;
		}
		if (lhs.has_value()) {
			return *lhs != *rhs;
		}
		return false;
	}

	template <typename T, typename U>
	constexpr bool operator<(const optional<T> &lhs, const optional<U> &rhs) {
		if (!rhs.has_value()) {
			return false;
		}
		if (!lhs.has_value()) {
			return true;
		}
		return *lhs < *rhs;
	}

	template <typename T, typename U>
	constexpr bool operator>(const optional<T> &lhs, const optional<U> &rhs) {
		if (!lhs.has_value()) {
			return false;
		}
		if (!rhs.has_value()) {
			return true;
		}
		return *lhs > *rhs;
	}

	template <typename T, typename U>
	constexpr bool operator<=(const optional<T> &lhs, const optional<U> &rhs) {
		if (!lhs.has_value()) {
			return true;
		}
		if (!rhs.has_value()) {
			return false;
		}
		return *lhs <= *rhs;
	}

	template <typename T, typename U>
	constexpr bool operator>=(const optional<T> &lhs, const optional<U> &rhs) {
		if (!rhs.has_value()) {
			return true;
		}
		if (!lhs.has_value()) {
			return false;
		}
		return *lhs >= *rhs;
	}

	// TODO 3-way comparison between optionals

	template <typename T>
	constexpr bool operator==(const optional<T> &lhs, nullopt_t) {
		return !lhs.has_value();
	}

	// TODO 3-way comparison between optional and nullopt

	template <typename T, typename U>
	constexpr bool operator==(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return false;
		}
		return *opt == value;
	}

	template <typename T, typename U>
	constexpr bool operator==(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return false;
		}
		return value == *opt;
	}

	template <typename T, typename U>
	constexpr bool operator!=(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return true;
		}
		return *opt != value;
	}

	template <typename T, typename U>
	constexpr bool operator!=(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return true;
		}
		return value != *opt;
	}

	template <typename T, typename U>
	constexpr bool operator<(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return true;
		}
		return *opt < value;
	}

	template <typename T, typename U>
	constexpr bool operator<(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return false;
		}
		return value < *opt;
	}

	template <typename T, typename U>
	constexpr bool operator>(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return false;
		}
		return *opt > value;
	}

	template <typename T, typename U>
	constexpr bool operator>(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return true;
		}
		return value > *opt;
	}

	template <typename T, typename U>
	constexpr bool operator<=(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return true;
		}
		return *opt <= value;
	}

	template <typename T, typename U>
	constexpr bool operator<=(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return false;
		}
		return value <= *opt;
	}

	template <typename T, typename U>
	constexpr bool operator>=(const optional<T> &opt, const U &value) {
		if (!opt.has_value()) {
			return false;
		}
		return *opt >= value;
	}

	template <typename T, typename U>
	constexpr bool operator>=(const U &value, const optional<T> &opt) {
		if (!opt.has_value()) {
			return true;
		}
		return value >= *opt;
	}

	// TODO 3-way comparison between optional and value
#pragma endregion

	/**
	 * @brief Creates an optional object from a value
	 *
	 * @tparam T The type of the value
	 * @param value The value
	 * @return The optional object
	 */
	template <typename T>
	constexpr optional<std::decay_t<T>> make_optional(T &&value)
		requires(std::is_constructible_v<std::decay_t<T>>)
	{
		return optional<std::decay_t<T>>(std::forward<T>(value));
	}

	/**
	 * @brief Creates an optional object in-place from arguments
	 *
	 * @tparam T The type of the optional object
	 * @tparam Args The types of the arguments to be passed to the constructor of T
	 * @param args The arguments to be passed to the constructor of T
	 * @return The optional object
	 */
	template <typename T, typename... Args>
	constexpr optional<T> make_optional(Args &&...args)
		requires(std::is_constructible_v<T, Args && ...>)
	{
		return optional<T>(std::in_place, std::forward<Args>(args)...);
	}

	/**
	 * @brief Creates an optional object in-place from an initializer list and arguments
	 *
	 * @tparam T The type of the optional object
	 * @tparam U The type of the initializer list
	 * @tparam Args The types of the arguments to be passed to the constructor of T
	 * @param list The initializer list to be passed to the constructor of T
	 * @param args The arguments to be passed to the constructor of T
	 * @return The optional object
	 */
	template <typename T, typename U, typename... Args>
	constexpr optional<T> make_optional(std::initializer_list<U> list, Args &&...args)
		requires(std::is_constructible_v<T, std::initializer_list<U> &, Args && ...>)
	{
		return optional<T>(std::in_place, list, std::forward<Args>(args)...);
	}

	/**
	 * @brief Swaps the values of two optional objects
	 *
	 * @tparam T The type of the optional objects
	 * @param lhs The first optional object
	 * @param rhs The second optional object
	 *
	 * @endlink https://en.cppreference.com/w/cpp/utility/optional/swap2 @endlink
	 */
	template <typename T>
	constexpr void swap(optional<T> &lhs, optional<T> &rhs)
		requires(std::is_move_constructible_v<T> && std::is_swappable_v<T>)
	{
		lhs.swap(rhs);
	}

	// TODO hash function
}