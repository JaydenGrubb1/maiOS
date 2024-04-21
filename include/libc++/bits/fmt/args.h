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

#include <bits/fmt/forward.h>
#include <bits/fmt/store.h>

// TODO add support for packed arguments

namespace std {
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
	[[nodiscard]] inline auto make_format_args(Args &&...args) {
		return basic_format_args<Context>(__detail::__format_store<Context, Args...>(std::forward<Args>(args)...));
	}

	template <typename... Args>
	[[nodiscard]] inline auto make_wformat_args(Args &&...args) {
		return make_format_args<wformat_context>(std::forward<Args>(args)...);
	}
}