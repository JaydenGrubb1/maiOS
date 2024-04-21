/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Various iterator types for formatting
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/iterator_traits.h>
#include <cstdio>

namespace std::__detail {
	template <typename Char>
	class __format_iter {
	  public:
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = ptrdiff_t;
		using iterator_category = std::output_iterator_tag;

		constexpr __format_iter(void) = default;
		constexpr __format_iter(const __format_iter &) = default;

		constexpr inline __format_iter &operator=(Char c) {
			std::putchar(c);
			if (c == '\n') {
				std::fflush(stdout);
			}
			return *this;
		}

		constexpr inline __format_iter &operator*() {
			return *this;
		}

		constexpr inline __format_iter &operator++() {
			return *this;
		}

		constexpr inline __format_iter &operator++(int) {
			return *this;
		}
	};

	template <typename Char>
	class __nop_iter {
	  public:
		using value_type = void;
		using pointer = void;
		using reference = void;
		using difference_type = ptrdiff_t;
		using iterator_category = std::output_iterator_tag;

		constexpr __nop_iter(void) = default;
		constexpr __nop_iter(const __nop_iter &) = default;

		constexpr inline __nop_iter &operator=(Char) {
			return *this;
		}

		constexpr inline __nop_iter &operator*() {
			return *this;
		}

		constexpr inline __nop_iter &operator++() {
			return *this;
		}

		constexpr inline __nop_iter &operator++(int) {
			return *this;
		}
	};
}