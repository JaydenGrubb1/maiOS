/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Format library forward declarations
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/fmt/format_buffer.h>
#include <string_view>

namespace std {
	template <typename T, typename Char>
	struct formatter {};

	template <typename Char, typename... Args>
	struct basic_format_string;

	template <typename... Args>
	using format_string = basic_format_string<char, std::type_identity_t<Args>...>;

	template <typename... Args>
	using wformat_string = basic_format_string<wchar_t, std::type_identity_t<Args>...>;

	template <typename Iter, typename Char>
	class basic_format_context;

	using format_context = basic_format_context<__detail::__format_iter<char>, char>;
	using wformat_context = basic_format_context<__detail::__format_iter<wchar_t>, wchar_t>;

	template <typename Context>
	class basic_format_arg;

	template <typename Context>
	class basic_format_args;

	using format_args = basic_format_args<format_context>;
	using wformat_args = basic_format_args<wformat_context>;

	template <typename Char>
	class basic_format_parse_context;

	using format_parse_context = basic_format_parse_context<char>;
	using wformat_parse_context = basic_format_parse_context<wchar_t>;

	namespace __detail {
		template <typename Iter, typename Char, typename Context>
		inline constexpr Iter __vformat_to(Iter, basic_string_view<Char>, const basic_format_args<Context> &);

		template <typename Iter, typename Char>
		inline constexpr void __do_format(basic_format_context<Iter, Char> &, basic_string_view<Char>);
	}

	template <typename Iter, typename... Args>
	inline Iter format_to(Iter, format_string<Args...>, Args &&...);

	template <typename Iter, typename... Args>
	inline Iter format_to(Iter, wformat_string<Args...>, Args &&...);
}