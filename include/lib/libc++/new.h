/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-01
 * @brief C++ style memory allocation functions
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <lib/libc++/internal/placement_new.h>

#include <kernel/arch/memory.h>

namespace kstd {
	/**
	 * @brief Allocation alignment value
	 */
	enum class align_val : size_t {};

	/**
	 * @brief Performs memory laundering
	 *
	 * i.e. Prevents the compiler from optimizing away a pointer to an object
	 *
	 * @tparam T The type of the pointer
	 * @param ptr The pointer to launder
	 * @return The laundered pointer
	 */
	template <typename T>
	[[nodiscard]] constexpr T *launder(T *ptr) {
		return __builtin_launder(ptr);
	}
}

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] void *operator new(size_t size) {
	return Memory::allocate(size);
}

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @return A pointer to the allocated memory
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] void *operator new[](size_t size) {
	return Memory::allocate(size);
}

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @param align The alignment of the memory to allocate
 * @return A pointer to the allocated memory
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] void *operator new(size_t size, kstd::align_val align) {
	return Memory::allocate(size, static_cast<size_t>(align));
}

/**
 * @brief Allocate memory
 *
 * @param size The size of the memory to allocate
 * @param align The alignment of the memory to allocate
 * @return A pointer to the allocated memory
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_new @endlink
 */
[[nodiscard]] void *operator new[](size_t size, kstd::align_val align) {
	return Memory::allocate(size, static_cast<size_t>(align));
}

// TODO Should these be implemented?
// void operator delete(void *ptr);
// void operator delete[](void *ptr);
// void operator delete(void *ptr, kstd::align_val align);
// void operator delete[](void *ptr, kstd::align_val align);

/**
 * @brief Deallocate memory
 *
 * @param ptr The memory to deallocate
 * @param size The size of the memory to deallocate
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_delete @endlink
 */
void operator delete(void *ptr, size_t size) {
	Memory::deallocate(ptr, size);
}

/**
 * @brief Deallocate memory
 *
 * @param ptr The memory to deallocate
 * @param size The size of the memory to deallocate
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_delete @endlink
 */
void operator delete[](void *ptr, size_t size) {
	Memory::deallocate(ptr, size);
}

/**
 * @brief Deallocate memory
 *
 * @param ptr The memory to deallocate
 * @param size The size of the memory to deallocate
 * @param align The alignment of the memory to deallocate
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_delete @endlink
 */
void operator delete(void *ptr, size_t size, kstd::align_val align) {
	Memory::deallocate(ptr, size, static_cast<size_t>(align));
}

/**
 * @brief Deallocate memory
 *
 * @param ptr The memory to deallocate
 * @param size The size of the memory to deallocate
 * @param align The alignment of the memory to deallocate
 *
 * @link https://en.cppreference.com/w/cpp/memory/new/operator_delete @endlink
 */
void operator delete[](void *ptr, size_t size, kstd::align_val align) {
	Memory::deallocate(ptr, size, static_cast<size_t>(align));
}