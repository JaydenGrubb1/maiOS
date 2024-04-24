/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Provides access to multiple type-erased arguments for formatting functions
 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_args @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstddef>

#include <array>
#include <bits/fmt/format_fwd.h>
#include <utility>

// TODO add support for packed arguments

namespace std {
	namespace __detail {
		template <typename Context, typename... Args>
		struct __format_store {
			array<basic_format_arg<Context>, sizeof...(Args)> args;

			constexpr __format_store(Args &&...args)
				requires(sizeof...(Args) > 0)
				: args{basic_format_arg<Context>(std::forward<Args>(args))...} {}

			consteval __format_store(void) : args{basic_format_arg<Context>{type_identity<remove_cvref_t<Args>>{}}...} {}
		};
	}

	template <typename Context>
	class basic_format_args {
	  private:
		size_t _count;
		const basic_format_arg<Context> *_args;

	  public:
		constexpr basic_format_args(void) = default;

		template <typename... Args>
		constexpr basic_format_args(const __detail::__format_store<Context, Args...> &store)
			: _count(sizeof...(Args)), _args(store.args.data()) {}

		[[nodiscard]] constexpr basic_format_arg<Context> get(size_t idx) const {
			if (idx < _count) {
				return _args[idx];
			} else {
				return basic_format_arg<Context>();
			}
		}
	};

	template <typename Context = format_context, typename... Args>
	[[nodiscard]] constexpr inline auto make_format_args(Args &&...args) {
		return __detail::__format_store<Context, Args...>(std::forward<Args>(args)...);
	}

	template <typename... Args>
	[[nodiscard]] constexpr inline auto make_wformat_args(Args &&...args) {
		return __detail::__format_store<wformat_context, Args...>(std::forward<Args>(args)...);
	}
}