/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Provides a basic format context for formatting functions
 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_context @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstddef>

#include <bits/format_forward.h>
#include <utility>

namespace std {
	template <typename Iter, typename Char>
	class basic_format_context {
	  public:
		using iterator = Iter;
		using char_type = Char;
		// TODO formatter_type

	  private:
		basic_format_args<basic_format_context> _args;
		Iter _iter;
		// TODO locale ???

	  public:
		constexpr basic_format_context(void) = default;

		constexpr ~basic_format_context() = default;

		[[nodiscard]] constexpr basic_format_arg<basic_format_context> args(size_t idx) const {
			// TODO implement this
			// _args.get(idx);
			return {};
		}

		[[nodiscard]] constexpr Iter out(void) const {
			return std::move(_iter);
		}

		constexpr void advance_to(Iter iter) {
			_iter = std::move(iter);
		}

		// TODO locale() ???
	};

	// TODO add these, requires type erased iterator (sink)
	// using format_context = basic_format_context<///, char>;
	// using wformat_context = basic_format_context<///, wchar_t>;
}