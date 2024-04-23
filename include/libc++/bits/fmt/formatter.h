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

#include <bits/fmt/forward.h>

#include <kernel/debug.h>

namespace std {
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

	template <typename Char>
	struct formatter<signed int, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const signed int value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('d');
			return ctx.out();
		}
	};

	template <typename Char>
	struct formatter<unsigned int, Char> {
		constexpr auto parse(basic_format_parse_context<Char> &ctx) {
			return ctx.begin();
		}

		template <typename Iter>
		Iter format(const unsigned int value, basic_format_context<Iter, Char> &ctx) {
			(void)value;
			*ctx.out()++ = Char('u');
			return ctx.out();
		}
	};
}