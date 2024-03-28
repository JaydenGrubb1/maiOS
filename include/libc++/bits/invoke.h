/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-03-28
 * @brief Provides a mechanism to invoke a callable object with a set of arguments
 * @link https://en.cppreference.com/w/cpp/utility/functional/invoke @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <type_traits>

#include <utility>

namespace std {
	/**
	 * @brief Provides a mechanism to invoke a callable object with a set of arguments
	 *
	 * @tparam Callable The type of the callable object
	 * @tparam Args The types of the arguments to pass to the callable object
	 * @param func The callable object to invoke
	 * @param args The arguments to pass to the callable object
	 * @return The result of invoking the callable object with the arguments
	 */
	template <typename Callable, typename... Args>
	constexpr inline invoke_result_t<Callable, Args...> invoke(Callable &&func, Args &&...args) {
		// TODO support member function pointers
		return std::forward<Callable>(func)(std::forward<Args>(args)...);
	}
}