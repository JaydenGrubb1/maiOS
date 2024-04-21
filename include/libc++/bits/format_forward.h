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

#include <bits/format_iter.h>
#include <string_view>

namespace std {
	template <typename T, typename Char>
	struct formatter;

	template <typename Char, typename... Args>
	struct basic_format_string;

	template <typename Iter, typename Char>
	class basic_format_context;

	using format_context = basic_format_context<__detail::__format_iter<char>, char>;
	using wformat_context = basic_format_context<__detail::__format_iter<wchar_t>, wchar_t>;
	// FIXME add proper iterator types

	template <typename Context>
	class basic_format_arg;

	template <typename Context>
	class basic_format_args;

	using format_args = basic_format_args<format_context>;
	using wformat_args = basic_format_args<wformat_context>;

	template <typename Char>
	class basic_format_parse_context;

	namespace __detail {
		template <typename Iter, typename Char, typename Context>
		inline Iter __vformat_to(Iter, basic_string_view<Char>, const basic_format_args<Context> &);
	}
}