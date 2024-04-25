/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-22
 * @brief Defines formatting rules for a type
 * @link https://en.cppreference.com/w/cpp/utility/format/formatter @endlink
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/fmt/format_fwd.h>

namespace std {
	template <typename T, typename Char>
	struct formatter {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('?');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<Char, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const Char value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('c');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<bool, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const bool value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('b');
			return ctx.out();
		}
	};

	template <typename T, typename Char>
		requires std::is_integral_v<T>
	struct formatter<T, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('i');
			return ctx.out();
		}
	};

	template <typename T, typename Char>
		requires std::is_floating_point_v<T>
	struct formatter<T, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const T value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('f');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<basic_string_view<Char>, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const basic_string_view<Char> value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('s');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<const Char *, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const Char *value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('q');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<const void *, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const void *value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('p');
			return ctx.out();
		}
	};

}