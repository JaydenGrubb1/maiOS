/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-07
 * @brief Manages a unique pointer to an object
 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <compare>
#include <type_traits>

#include <utility.h>

namespace kstd {
	/**
	 * @brief Manages a unique pointer to an object
	 *
	 * @tparam T The type of object to manage
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr @endlink
	 */
	template <typename T>
	class unique_ptr {
	  private:
		T *_ptr;

	  public:
#pragma region Constructors
		/**
		 * @brief Construct a new unique_ptr object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr @endlink
		 */
		constexpr unique_ptr(void) = default;

		/**
		 * @brief Construct a new unique_ptr object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr @endlink
		 */
		constexpr unique_ptr(std::nullptr_t) : _ptr(nullptr) {}

		/**
		 * @brief Construct a new unique_ptr object
		 *
		 * @param ptr The pointer to managess
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr @endlink
		 */
		constexpr explicit unique_ptr(T *ptr) : _ptr(ptr) {}

		// disallow copy construction
		constexpr unique_ptr(const unique_ptr &) = delete;

		/**
		 * @brief Construct a new unique_ptr object
		 *
		 * @param other The unique_ptr to take ownership from
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr @endlink
		 */
		constexpr unique_ptr(unique_ptr &&other) : _ptr(other._ptr) {
			other._ptr = nullptr;
		}

		/**
		 * @brief Construct a new unique_ptr object
		 *
		 * @tparam U The type of object to manage
		 * @param other The unique_ptr to take ownership from
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/unique_ptr @endlink
		 */
		template <typename U>
		constexpr unique_ptr(unique_ptr<U> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U>)
			: _ptr(other._ptr) {
			other._ptr = nullptr;
		}
#pragma endregion

#pragma region Assignment Operators

		/**
		 * @brief Assigns a new pointer to the unique_ptr
		 *
		 * @return A reference to the unique_ptr
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D @endlink
		 */
		constexpr unique_ptr &operator=(std::nullptr_t) {
			reset();
			return *this;
		}

		// disallow copy assignment
		constexpr unique_ptr &operator=(const unique_ptr &) = delete;

		/**
		 * @brief Assigns a new pointer to the unique_ptr
		 *
		 * @param other The unique_ptr to take ownership from
		 * @return A reference to the unique_ptr
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D @endlink
		 */
		constexpr unique_ptr &operator=(unique_ptr &&other) {
			reset(other.release());
			return *this;
		}

		/**
		 * @brief Assigns a new pointer to the unique_ptr
		 *
		 * @tparam U The type of object to manage
		 * @param other The unique_ptr to take ownership from
		 * @return A reference to the unique_ptr
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator%3D @endlink
		 */
		template <typename U>
		constexpr unique_ptr &operator=(unique_ptr<U> &&other)
			requires(std::is_convertible_v<U *, T *> && !std::is_array_v<U>)
		{
			reset(other.release());
			return *this;
		}
#pragma endregion

		/**
		 * @brief Destroy the unique_ptr object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/%7Eunique_ptr @endlink
		 */
		constexpr ~unique_ptr(void) {
			if (_ptr != nullptr) {
				delete _ptr;
			}
		}

		/**
		 * @brief Returns a pointer to the managed object
		 *
		 * @return A pointer to the managed object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/get @endlink
		 */
		[[nodiscard]] constexpr T *get(void) const {
			return _ptr;
		}

		/**
		 * @brief Checks if the unique_ptr is managing an object
		 *
		 * @return true If the unique_ptr is managing an object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_bool @endlink
		 */
		[[nodiscard]] constexpr explicit operator bool(void) const {
			return _ptr != nullptr;
		}

		/**
		 * @brief Dereferences the managed object
		 *
		 * @return A reference to the managed object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator* @endlink
		 */
		[[nodiscard]] typename std::add_lvalue_reference_t<T> operator*(void) const {
			return *_ptr;
		}

		/**
		 * @brief Dereferences the managed object
		 *
		 * @return A pointer to the managed object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/operator* @endlink
		 */
		[[nodiscard]] constexpr T *operator->(void) const {
			return _ptr;
		}

		/**
		 * @brief Releases ownership of the managed object
		 *
		 * @return A pointer to the managed object
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/release @endlink
		 */
		[[nodiscard]] constexpr T *release(void) {
			auto ptr = _ptr;
			_ptr = nullptr;
			return ptr;
		}

		/**
		 * @brief Resets the unique_ptr to manage a new object
		 *
		 * @param ptr The pointer to manage
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/reset @endlink
		 */
		constexpr void reset(T *ptr = nullptr) {
			if (_ptr != nullptr) {
				delete _ptr;
			}
			_ptr = ptr;
		}

		/**
		 * @brief Swaps the managed object with another unique_ptr
		 *
		 * @param other The unique_ptr to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/swap @endlink
		 */
		constexpr void swap(unique_ptr &other) {
			kstd::swap(_ptr, other._ptr);
		}
	};

	/**
	 * @brief Creates a new unique_ptr from arguements
	 *
	 * @tparam T The type of object to manage
	 * @tparam Args The types of arguments to pass to the constructor
	 * @param args The arguments to pass to the constructor
	 * @return A unique_ptr to the new object
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique @endlink
	 */
	template <typename T, typename... Args>
	[[nodiscard]] constexpr unique_ptr<T> make_unique(Args &&...args)
		requires(!std::is_array_v<T>)
	{
		return unique_ptr<T>(new T(kstd::forward<Args>(args)...));
	}

	/**
	 * @brief Creates a new unique_ptr with default-initialization
	 *
	 * @tparam T The type of object to manage
	 * @return A unique_ptr to the new object
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique @endlink
	 */
	template <typename T>
	[[nodiscard]] constexpr unique_ptr<T> make_unique_for_overwrite(void)
		requires(!std::is_array_v<T>)
	{
		return unique_ptr<T>(new T);
	}

// Comparison operators
// https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_cmp
#pragma region Comparison Operators

	template <typename T1, typename T2>
	[[nodiscard]] constexpr bool operator==(const unique_ptr<T1> &lhs, const unique_ptr<T2> &rhs) {
		return lhs.get() == rhs.get();
	}

	template <typename T1, typename T2>
	[[nodiscard]] constexpr auto operator<=>(const unique_ptr<T1> &lhs, const unique_ptr<T2> &rhs) {
		return lhs.get() <=> rhs.get();
	}

	template <typename T>
	[[nodiscard]] constexpr bool operator==(const unique_ptr<T> &ptr, std::nullptr_t) {
		return ptr.get() == static_cast<T *>(nullptr);
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator<=>(const unique_ptr<T> &ptr, std::nullptr_t) {
		return ptr.get() <=> static_cast<T *>(nullptr);
	}

#pragma endregion

	// TODO left shift operator

	/**
	 * @brief Swaps the managed objects of two unique_ptrs
	 *
	 * @tparam T The type of object being managed
	 * @param lhs The first unique_ptr
	 * @param rhs The second unique_ptr
	 *
	 * @link https://en.cppreference.com/w/cpp/memory/unique_ptr/swap2 @endlink
	 */
	template <typename T>
	constexpr void swap(unique_ptr<T> &lhs, unique_ptr<T> &rhs) {
		lhs.swap(rhs);
	}

	// TODO hash

	// The below functions rely on the array specialization of unique_ptr.
	// This specialization is not currently possible as it requires use of
	// the delete operator without a size. This operator is not currently
	// supported for performance reasons, however it may be added in the
	// future, but likely not within the kernel.

	// TODO unique_ptr<T[]> class specialization
	// TODO make_unique array unknown bounds
	// TODO make_unique array known bounds = delete
	// TODO make_unique_for_overwrite array unknown bounds
	// TODO make_unique_for_overwrite array known bounds = delete
}