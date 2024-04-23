/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Internal storage struct for format arguments
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <array>
#include <bits/fmt/forward.h>
#include <utility>

// TODO add support for packed arguments

namespace std::__detail {
	template <typename Context, typename... Args>
	struct __format_store {
		array<basic_format_arg<Context>, sizeof...(Args)> args;

		constexpr __format_store(Args &&...args)
			requires(sizeof...(Args) > 0)
			: args{basic_format_arg<Context>(std::forward<Args>(args))...} {}

		consteval __format_store(void) : args{basic_format_arg<Context>{type_identity<remove_cvref_t<Args>>{}}...} {}
	};
}