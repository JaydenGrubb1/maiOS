/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief A type-safe format string with compile-time validation
 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_string @endlink
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
	/**
	 * @brief A type-safe format string with compile-time validation
	 *
	 * @tparam Char The character type of the format string
	 * @tparam Args The types of the arguments to be formatted
	 *
	 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_string @endlink
	 */
	template <typename Char, typename... Args>
	struct basic_format_string {
	  private:
		basic_string_view<Char> _format;

	  public:
		/**
		 * @brief Construct a new basic format string object
		 *
		 * @tparam T The character type of the format string
		 */
		template <typename T>
		consteval basic_format_string(const T &format)
			requires(std::is_convertible_v<const T &, basic_string_view<Char>>)
			: _format(format) {
			using iterator = __detail::__nop_iter<Char>;
			using context = basic_format_context<iterator, Char>;
			auto store = __detail::__format_store<context, Args...>();
			auto args = basic_format_args(store);
			basic_format_context<iterator, Char> ctx(args, iterator());
			__detail::__do_format(ctx, _format);
		}

		// TODO runtime format string constructor

		/**
		 * @brief Get the format string
		 *
		 * @return The format string
		 */
		constexpr inline basic_string_view<Char> get(void) const {
			return _format;
		}
	};
}