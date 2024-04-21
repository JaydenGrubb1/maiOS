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

#include <bits/fmt/forward.h>
#include <utility>

namespace std {
	template <typename Iter, typename Char>
	class basic_format_context {
	  public:
		using iterator = Iter;
		using char_type = Char;

		template <typename T>
		using formatter = formatter<T, Char>;

	  private:
		basic_format_args<basic_format_context> _args;
		Iter _iter;
		// TODO locale ???

		constexpr basic_format_context(basic_format_args<basic_format_context> args, Iter iter)
			: _args(args), _iter(iter) {}

		// locale variant constructor ???

		template <typename It, typename Ch, typename Ctx>
		friend It __detail::__vformat_to(It, basic_string_view<Ch>, const basic_format_args<Ctx> &);

	  public:
		constexpr basic_format_context(void) = default;
		constexpr ~basic_format_context() = default;

		[[nodiscard]] constexpr basic_format_arg<basic_format_context> args(size_t idx) const {
			return _args.get(idx);
		}

		[[nodiscard]] constexpr Iter out(void) const {
			return std::move(_iter);
		}

		constexpr void advance_to(Iter iter) {
			_iter = std::move(iter);
		}

		// TODO locale() ???
	};
}