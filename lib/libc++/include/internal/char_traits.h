/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-26
 * @brief // DOC
 * @link https://en.cppreference.com/w/cpp/string/char_traits @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#define __need_size_t
#include <stddef.h>

namespace kstd {
	/**
	 * @brief // DOC
	 *
	 * @tparam T The character type
	 *
	 * @link https://en.cppreference.com/w/cpp/string/char_traits @endlink
	 */
	template <typename T>
	class char_traits {
	  public:
		static constexpr void assign(T &lhs, const T &rhs) {
			lhs = rhs;
		}

		static constexpr T *assign(T *lhs, size_t count, T rhs) {
			for (size_t i = 0; i < count; i++) {
				lhs[i] = rhs;
			}
			return lhs;
		}

		static constexpr bool eq(const T &lhs, const T &rhs) {
			return lhs == rhs;
		}

		static constexpr bool lt(const T &lhs, const T &rhs) {
			return lhs < rhs;
		}

		static constexpr T *move(T *dest, const T *src, size_t count) {
			if (dest < src) {
				for (size_t i = 0; i < count; i++) {
					dest[i] = src[i];
				}
			} else {
				for (size_t i = count; i > 0; i--) {
					dest[i - 1] = src[i - 1];
				}
			}

			return dest;
		}

		static constexpr T *copy(T *dest, const T *src, size_t count) {
			for (size_t i = 0; i < count; i++) {
				dest[i] = src[i];
			}

			return dest;
		}

		static constexpr int compare(const T *lhs, const T *rhs, size_t count) {
			for (size_t i = 0; i < count; i++) {
				if (lhs[i] < rhs[i]) {
					return -1;
				} else if (lhs[i] > rhs[i]) {
					return 1;
				}
			}

			return 0;
		}

		static constexpr size_t length(const T *str) {
			size_t len = 0;
			while (str[len]) {
				len++;
			}
			return len;
		}

		static constexpr const T *find(const T *str, size_t count, const T &ch) {
			for (size_t i = 0; i < count; i++) {
				if (str[i] == ch) {
					return &str[i];
				}
			}

			return nullptr;
		}

		static constexpr T to_char_type(int ch) {
			return static_cast<T>(ch);
		}

		static constexpr int to_int_type(T ch) {
			return static_cast<int>(ch);
		}

		static constexpr bool eq_int_type(int lhs, int rhs) {
			return lhs == rhs;
		}

		static constexpr int eof() {
			return -1;
		}

		static constexpr int not_eof(int ch) {
			return ch != eof();
		}
	};
}