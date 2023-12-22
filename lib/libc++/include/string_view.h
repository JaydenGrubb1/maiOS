/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-06
 * @brief Class template for referencing a constant contiguous sequence of characters
 * @link https://en.cppreference.com/w/cpp/string/basic_string_view @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

namespace kstd {
	template <typename T>
	class basic_string_view {
	  private:
		const T *_data;
		size_t _size;

	  public:
		constexpr basic_string_view(void) : _data(nullptr), _size(0) {}
		constexpr basic_string_view(const basic_string_view &other) = default;
		constexpr basic_string_view(const T* data, size_t size) : _data(data), _size(size) {}

		constexpr basic_string_view(const T* data) : _data(data), _size(0) {
			while (*data++) {
				_size++;
			}
			// TODO use std::strlen or std::char_traits<T>::length or std::distance
			// remember needs to be constexpr
		}

		template <typename Iter, typename End>
		constexpr basic_string_view(Iter begin, End end) : _data(&*begin), _size(0) {
			while (begin != end) {
				_size++;
				begin++;
			}
			// TODO use std::distance
			// remember needs to be constexpr
		}
		// TODO check trait constraints
	};

	using string_view = basic_string_view<char>;
	using wstring_view = basic_string_view<wchar_t>;
	using u8string_view = basic_string_view<char8_t>;
	using u16string_view = basic_string_view<char16_t>;
	using u32string_view = basic_string_view<char32_t>;
}