/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-07
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <lib/libc++/utility.h>

namespace kstd {
	/**
	 * @brief Specifies the deleter type for unique_ptr
	 *
	 * @tparam T The type of the pointer to be deleted
	 */
	template <typename T>
	class default_delete {
	  public:
		/**
		 * @brief Construct a new default delete object
		 *
		 */
		constexpr default_delete(void) = default;

		/**
		 * @brief Construct a new default delete object
		 *
		 * @tparam U The underlying type of the default_delete to copy
		 * @param other The default_delete to copy
		 */
		template <typename U>
		constexpr default_delete(const default_delete<U> &)
			requires(std::is_convertible_v<U *, T *>)
		{}

		/**
		 * @brief Deletes the pointer
		 *
		 * @param ptr The pointer to delete
		 */
		constexpr void operator()(T *ptr) const {
			static_assert(!std::is_void_v<T>, "can't delete pointer to incomplete type");
			static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
			delete ptr;
		}
	};

	/**
	 * @brief Specifies the deleter type for unique_ptr
	 *
	 * @tparam T The type of the pointer to be deleted
	 */
	template <typename T>
	class default_delete<T[]> {
	  public:
		/**
		 * @brief Construct a new default delete object
		 *
		 */
		constexpr default_delete(void) = default;

		/**
		 * @brief Construct a new default delete object
		 *
		 * @tparam U The underlying type of the default_delete to copy
		 * @param other The default_delete to copy
		 */
		template <typename U>
		constexpr default_delete(const default_delete<U[]> &)
			requires(std::is_convertible_v<U (*)[], T (*)[]>)
		{}

		/**
		 * @brief Deletes the pointer
		 *
		 * @param ptr The pointer to delete
		 */
		constexpr void operator()(T *ptr) const {
			static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
			delete[] ptr;
		}
	};

	template <typename T, typename Deleter = default_delete<T>>
	class unique_ptr {
	  private:
		T *_ptr;
		Deleter _deleter;

	  public:
#pragma region Constructors
		constexpr unique_ptr(void)
			requires(std::is_default_constructible_v<Deleter> && !std::is_pointer_v<Deleter>)
		= default;

		constexpr unique_ptr(std::nullptr_t)
			requires(std::is_default_constructible_v<Deleter> && !std::is_pointer_v<Deleter>)
			: _ptr(nullptr) {}

		constexpr explicit unique_ptr(T *ptr)
			requires(std::is_default_constructible_v<Deleter> && !std::is_pointer_v<Deleter>)
			: _ptr(ptr) {}

		constexpr unique_ptr(T *ptr, const Deleter &deleter)
			requires(!std::is_reference_v<Deleter> && std::is_copy_constructible_v<Deleter>)
			: _ptr(ptr), _deleter(deleter) {}

		constexpr unique_ptr(T *ptr, Deleter &&deleter)
			requires(!std::is_reference_v<Deleter> && std::is_move_constructible_v<Deleter>)
			: _ptr(ptr), _deleter(kstd::move(deleter)) {}

		constexpr unique_ptr(T *ptr, Deleter &deleter)
			requires(std::is_lvalue_reference_v<Deleter &>)
			: _ptr(ptr), _deleter(deleter) {}

		constexpr unique_ptr(T *ptr, Deleter &&deleter)
			requires(std::is_lvalue_reference_v<Deleter &>)
		= delete;

		constexpr unique_ptr(T *ptr, const Deleter &deleter)
			requires(std::is_lvalue_reference_v<const Deleter &>)
			: _ptr(ptr), _deleter(deleter) {}

		constexpr unique_ptr(T *ptr, Deleter &&deleter)
			requires(std::is_lvalue_reference_v<const Deleter &>)
		= delete;

		constexpr unique_ptr(const unique_ptr &) = delete;

		constexpr unique_ptr(unique_ptr &&other)
			requires(std::is_move_constructible_v<Deleter>)
			: _ptr(other._ptr), _deleter(kstd::move(other._deleter)) {
			other._ptr = nullptr;
		}

		template <typename U, typename E>
		constexpr unique_ptr(unique_ptr<U, E> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U> &&
					 std::is_reference_v<E> && std::is_same_v<E, Deleter>)
			: _ptr(other._ptr), _deleter(other._deleter) {
			other._ptr = nullptr;
		}

		template <typename U, typename E>
		constexpr unique_ptr(unique_ptr<U, E> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U> &&
					 !std::is_reference_v<E> && std::is_convertible_v<E, Deleter>)
			: _ptr(other._ptr), _deleter(kstd::forward<E>(other._deleter)) {
			other._ptr = nullptr;
		}
#pragma endregion

#pragma region Assignment Operators
		constexpr unique_ptr &operator=(std::nullptr_t) {
			reset();
			return *this;
		}

		constexpr unique_ptr &operator=(const unique_ptr &) = delete;

		constexpr unique_ptr &operator=(unique_ptr &&other)
			requires(std::is_move_assignable_v<Deleter>)
		{
			reset(other.release());
			_deleter = kstd::move(other._deleter);
			return *this;
		}

		template <typename U, typename E>
		constexpr unique_ptr &operator=(unique_ptr<U, E> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U> &&
					 std::is_assignable_v<Deleter &, E &&>)
		{
			reset(other.release());
			_deleter = other._deleter;
			return *this;
		}
#pragma endregion

		constexpr ~unique_ptr(void) {
			if (_ptr != nullptr) {
				_deleter(_ptr);
			}
		}

		constexpr T *get(void) const {
			return _ptr;
		}

		constexpr Deleter &get_deleter(void) {
			return _deleter;
		}

		constexpr const Deleter &get_deleter(void) const {
			return _deleter;
		}

		constexpr explicit operator bool(void) const {
			return _ptr != nullptr;
		}

		typename std::add_lvalue_reference_t<T> operator*(void) const {
			return *_ptr;
		}

		constexpr T *operator->(void) const {
			return _ptr;
		}

		constexpr T *release(void) {
			auto ptr = _ptr;
			_ptr = nullptr;
			return ptr;
		}

		constexpr void reset(T *ptr = nullptr) {
			if (_ptr != nullptr) {
				_deleter(_ptr);
			}
			_ptr = ptr;
		}

		constexpr void swap(unique_ptr &other) {
			kstd::swap(_ptr, other._ptr);
			kstd::swap(_deleter, other._deleter);
		}
	};
}