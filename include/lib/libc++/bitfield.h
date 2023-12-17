/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-12-17
 * @brief // DOC // Experminental // Not part of the C++ standard library
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#define __need_size_t
#include <stddef.h>

namespace kstd {
	template <typename T>
	class bitfield {
	  private:
		T _data;

	  public:
		constexpr bitfield(void) = default;

		constexpr bitfield(T data) : _data(data) {}

		constexpr bitfield(size_t count, bool value) : _data(value ? ~static_cast<T>(0) : 0) {
			if (count > sizeof(T) * 8) {
				count = sizeof(T) * 8;
			}

			_data &= ~(~static_cast<T>(0) << count);
		}

		constexpr T data(void) const { return _data; }

		constexpr bool operator[](size_t index) const {
			return (_data >> index) & 1;
		}

		constexpr void set(size_t index, bool value) {
			if (value) {
				_data |= (1ULL << index);
			} else {
				_data &= ~(1ULL << index);
			}
		}

		constexpr bool full(void) const {
			return _data == ~static_cast<T>(0);
		}

		constexpr bool empty(void) const {
			return _data == 0;
		}
	};
}