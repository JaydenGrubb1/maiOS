/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Provides a context for parsing format strings
 * @link https://en.cppreference.com/w/cpp/utility/format/basic_format_parse_context @endlink
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
	template <typename Char>
	class basic_format_parse_context {
	  public:
		using char_type = Char;
		using iterator = basic_string_view<Char>::iterator;
		using const_iterator = basic_string_view<Char>::const_iterator;

	  private:
		iterator _begin;
		iterator _end;
		size_t _num_args;
		size_t _next_arg = 0;

		enum struct __indexing_mode {
			unknown,
			manual,
			automatic
		} _indexing_mode = __indexing_mode::unknown;

	  public:
		constexpr explicit basic_format_parse_context(basic_string_view<Char> format, size_t num_args = 0)
			: _begin(format.begin()), _end(format.end()), _num_args(num_args) {}

		basic_format_parse_context(const basic_format_parse_context &) = delete;
		void operator=(const basic_format_parse_context &) = delete;

		constexpr const_iterator begin(void) const {
			return _begin;
		}

		constexpr const_iterator end(void) const {
			return _end;
		}

		constexpr void advance_to(const_iterator it) {
			_begin = it;
		}

		constexpr size_t next_arg_id(void) {
			if (_indexing_mode == __indexing_mode::manual) {
				std::unreachable();
			}
			_indexing_mode = __indexing_mode::automatic;

			if (std::is_constant_evaluated()) {
				if (_next_arg == _num_args) {
					std::unreachable();
				}
			}
			return _next_arg++;
		}

		constexpr void check_arg_id(size_t id) {
			if (_indexing_mode == __indexing_mode::automatic) {
				std::unreachable();
			}
			_indexing_mode = __indexing_mode::manual;

			if (std::is_constant_evaluated()) {
				if (id >= _num_args) {
					std::unreachable();
				}
			}
		}
	};
}