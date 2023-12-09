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
	template <typename T>
	class unique_ptr {
	  private:
		T *_ptr;

	  public:
#pragma region Constructors
		constexpr unique_ptr(void) = default;

		constexpr unique_ptr(std::nullptr_t) : _ptr(nullptr) {}

		constexpr explicit unique_ptr(T *ptr) : _ptr(ptr) {}

		constexpr unique_ptr(const unique_ptr &) = delete;

		constexpr unique_ptr(unique_ptr &&other) : _ptr(other._ptr) {
			other._ptr = nullptr;
		}

		template <typename U>
		constexpr unique_ptr(unique_ptr<U> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U>)
			: _ptr(other._ptr) {
			other._ptr = nullptr;
		}
#pragma endregion

#pragma region Assignment Operators
		constexpr unique_ptr &operator=(std::nullptr_t) {
			reset();
			return *this;
		}

		constexpr unique_ptr &operator=(const unique_ptr &) = delete;

		constexpr unique_ptr &operator=(unique_ptr &&other) {
			reset(other.release());
			return *this;
		}

		template <typename U>
		constexpr unique_ptr &operator=(unique_ptr<U> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U>)
		{
			reset(other.release());
			return *this;
		}
#pragma endregion

		constexpr ~unique_ptr(void) {
			if (_ptr != nullptr) {
				delete _ptr;
			}
		}

		[[nodiscard]] constexpr T *get(void) const {
			return _ptr;
		}

		[[nodiscard]] constexpr explicit operator bool(void) const {
			return _ptr != nullptr;
		}

		[[nodiscard]] typename std::add_lvalue_reference_t<T> operator*(void) const {
			return *_ptr;
		}

		[[nodiscard]] constexpr T *operator->(void) const {
			return _ptr;
		}

		[[nodiscard]] constexpr T *release(void) {
			auto ptr = _ptr;
			_ptr = nullptr;
			return ptr;
		}

		constexpr void reset(T *ptr = nullptr) {
			if (_ptr != nullptr) {
				delete _ptr;
			}
			_ptr = ptr;
		}

		constexpr void swap(unique_ptr &other) {
			kstd::swap(_ptr, other._ptr);
		}
	};

	// TODO Class array specialization

	template <typename T, typename... Args>
	[[nodiscard]] constexpr unique_ptr<T> make_unique(Args &&...args)
		requires(!std::is_array_v<T>)
	{
		return unique_ptr<T>(new T(kstd::forward<Args>(args)...));
	}

	// TODO make_unique array unknown bounds
	// TODO make_unique array known bounds = delete

	template <typename T>
	[[nodiscard]] constexpr unique_ptr<T> make_unique_for_overwrite(void)
		requires(!std::is_array_v<T>)
	{
		return unique_ptr<T>(new T);
	}

	// TODO make_unique_for_overwrite array unknown bounds
	// TODO make_unique_for_overwrite array known bounds = delete

	// TODO comparison operators
	// TODO left shift operator

	template <typename T>
	constexpr void swap(unique_ptr<T> &lhs, unique_ptr<T> &rhs) {
		lhs.swap(rhs);
	}

	// TODO hash
}