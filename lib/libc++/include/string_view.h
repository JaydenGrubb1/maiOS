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

#include <type_traits>

#include <internal/algo_basic.h>
#include <internal/char_traits.h>
#include <internal/reverse_iterator.h>
#include <optional.h>

namespace kstd {
	template <typename T, typename C = char_traits<T>>
	class basic_string_view {
	  private:
		const T *_data;
		size_t _size;

	  public:
		static constexpr size_t npos = -1;

#pragma region Constructors
		constexpr basic_string_view(void) : _data(nullptr), _size(0) {}

		constexpr basic_string_view(const basic_string_view &other) = default;

		constexpr basic_string_view(const T *data, size_t size) : _data(data), _size(size) {}

		constexpr basic_string_view(const T *data) : _data(data), _size(C::length(data)) {}

		template <typename Iter, typename End>
		constexpr basic_string_view(Iter begin, End end)
			requires(!std::is_convertible_v<End, size_t>)
			: _data(&*begin), _size(end - begin) {}
		// TODO more trait constraints
		// TODO std::to_address(begin)
#pragma endregion

		constexpr basic_string_view &operator=(const basic_string_view &other) = default;

#pragma region Capacity Functions
		[[nodiscard]] constexpr size_t size(void) const {
			return _size;
		}

		[[nodiscard]] constexpr size_t length(void) const {
			return _size;
		}

		[[nodiscard]] constexpr size_t max_size(void) const {
			return INTMAX_MAX / sizeof(T);
		}

		[[nodiscard]] constexpr bool empty(void) const {
			return _size == 0;
		}
#pragma endregion

#pragma region Accessor Functions
		[[nodiscard]] constexpr const T &operator[](size_t index) const {
			return _data[index];
		}

		[[nodiscard]] constexpr optional<T> at(size_t index) const {
			if (index >= _size) {
				return {};
			}
			return _data[index];
		}

		[[nodiscard]] constexpr const T &front(void) const {
			return _data[0];
		}

		[[nodiscard]] constexpr const T &back(void) const {
			return _data[_size - 1];
		}

		[[nodiscard]] constexpr const T *data(void) const {
			return _data;
		}
#pragma endregion

#pragma region Iterator Functions
		[[nodiscard]] constexpr const T *begin(void) const {
			return _data;
		}

		[[nodiscard]] constexpr const T *cbegin(void) const {
			return _data;
		}

		[[nodiscard]] constexpr const T *end(void) const {
			return _data + _size;
		}

		[[nodiscard]] constexpr const T *cend(void) const {
			return _data + _size;
		}

		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rend(void) const {
			return kstd::reverse_iterator(_data);
		}

		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crend(void) const {
			return kstd::reverse_iterator(_data);
		}
#pragma endregion

#pragma region Modifier Functions
		constexpr void remove_prefix(size_t n) {
			_data += n;
			_size -= n;
		}

		constexpr void remove_suffix(size_t n) {
			_size -= n;
		}

		constexpr void swap(basic_string_view &other) {
			const T *tmp_data = _data;
			size_t tmp_size = _size;

			_data = other._data;
			_size = other._size;

			other._data = tmp_data;
			other._size = tmp_size;
		}
#pragma endregion

		constexpr size_t copy(T *dest, size_t count, size_t pos = 0) const {
			if (pos > _size) {
				return 0;
			}
			size_t len = min(count, _size - pos);
			C::copy(dest, _data + pos, len);
			return len;
		}

		[[nodiscard]] constexpr basic_string_view substr(size_t pos = 0, size_t count = npos) const {
			if (pos > _size) {
				return basic_string_view();
			}
			return basic_string_view(_data + pos, min(count, _size - pos));
		}

#pragma region Comparison Functions
		[[nodiscard]] constexpr int compare(basic_string_view other) const {
			const size_t count = min(_size, other._size);
			int result = C::compare(_data, other._data, count);
			if (result != 0) {
				return result;
			}
			if (_size < other._size) {
				return -1;
			} else if (_size > other._size) {
				return 1;
			}
			return 0;
		}

		[[nodiscard]] constexpr int compare(size_t pos, size_t count, basic_string_view other) const {
			return substr(pos, count).compare(other);
		}

		[[nodiscard]] constexpr int compare(size_t pos1, size_t count1, basic_string_view other, size_t pos2, size_t count2) const {
			return substr(pos1, count1).compare(other.substr(pos2, count2));
		}

		[[nodiscard]] constexpr int compare(const T *other) const {
			return compare(basic_string_view(other));
		}

		[[nodiscard]] constexpr int compare(size_t pos, size_t count, const T *other) const {
			return substr(pos, count).compare(basic_string_view(other));
		}

		[[nodiscard]] constexpr int compare(size_t pos1, size_t count1, const T *other, size_t count2) const {
			return substr(pos1, count1).compare(basic_string_view(other, count2));
		}
#pragma endregion

		[[nodiscard]] constexpr size_t find(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (substr(i).starts_with(other)) {
					return i;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find(T ch, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (C::eq(_data[i], ch)) {
					return i;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find(const T *other, size_t pos, size_t count) const {
			return find(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t find(const T *other, size_t pos = 0) const {
			return find(basic_string_view(other), pos);
		}

		[[nodiscard]] constexpr size_t rfind(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (substr(_size - i - 1).starts_with(other)) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t rfind(T ch, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (C::eq(_data[_size - i - 1], ch)) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t rfind(const T *other, size_t pos, size_t count) const {
			return rfind(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t rfind(const T *other, size_t pos = npos) const {
			return rfind(basic_string_view(other), pos);
		}

		[[nodiscard]] constexpr bool starts_with(basic_string_view other) const {
			return compare(0, other.size(), other) == 0;
		}

		[[nodiscard]] constexpr bool starts_with(T ch) const {
			return size() > 0 && C::eq(front(), ch);
		}

		[[nodiscard]] constexpr bool starts_with(const T *other) const {
			return starts_with(basic_string_view(other));
		}

		[[nodiscard]] constexpr bool ends_with(basic_string_view other) const {
			return compare(size() - other.size(), other.size(), other) == 0;
		}

		[[nodiscard]] constexpr bool ends_with(T ch) const {
			return size() > 0 && C::eq(back(), ch);
		}

		[[nodiscard]] constexpr bool ends_with(const T *other) const {
			return ends_with(basic_string_view(other));
		}

		[[nodiscard]] constexpr bool contains(basic_string_view other) const {
			return find(other) != npos;
		}

		[[nodiscard]] constexpr bool contains(T ch) const {
			return find(ch) != npos;
		}

		[[nodiscard]] constexpr bool contains(const T *other) const {
			return find(other) != npos;
		}

		[[nodiscard]] constexpr size_t find_first_of(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (other.contains(_data[i])) {
					return i;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find_first_of(T ch, size_t pos = 0) const {
			return find_first_of(basic_string_view(&ch, 1), pos);
		}

		[[nodiscard]] constexpr size_t find_first_of(const T *other, size_t pos, size_t count) const {
			return find_first_of(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t find_first_of(const T *other, size_t pos = 0) const {
			return find_first_of(basic_string_view(other), pos);
		}

		[[nodiscard]] constexpr size_t find_last_of(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (other.contains(_data[_size - i - 1])) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find_last_of(T ch, size_t pos = npos) const {
			return find_first_of(basic_string_view(&ch, 1), pos);
		}

		[[nodiscard]] constexpr size_t find_last_of(const T *other, size_t pos, size_t count) const {
			return find_last_of(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t find_last_of(const T *other, size_t pos = npos) const {
			return find_last_of(basic_string_view(other), pos);
		}

		[[nodiscard]] constexpr size_t find_first_not_of(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (!other.contains(_data[i])) {
					return i;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find_first_not_of(T ch, size_t pos = 0) const {
			return find_first_not_of(basic_string_view(&ch, 1), pos);
		}

		[[nodiscard]] constexpr size_t find_first_not_of(const T *other, size_t pos, size_t count) const {
			return find_first_not_of(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t find_first_not_of(const T *other, size_t pos = 0) const {
			return find_first_not_of(basic_string_view(other), pos);
		}

		[[nodiscard]] constexpr size_t find_last_not_of(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (!other.contains(_data[_size - i - 1])) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		[[nodiscard]] constexpr size_t find_last_not_of(T ch, size_t pos = npos) const {
			return find_last_not_of(basic_string_view(&ch, 1), pos);
		}

		[[nodiscard]] constexpr size_t find_last_not_of(const T *other, size_t pos, size_t count) const {
			return find_last_not_of(basic_string_view(other, count), pos);
		}

		[[nodiscard]] constexpr size_t find_last_not_of(const T *other, size_t pos = npos) const {
			return find_last_not_of(basic_string_view(other), pos);
		}
	};

	// Deduction guides
	// https://en.cppreference.com/w/cpp/string/basic_string_view/deduction_guides
	template <typename T, typename End>
	basic_string_view(T *, End) -> basic_string_view<T>;
	// HACK This only works for basic iterators (i.e. not reverse)
	// this will require iterator_traits to work with more iterators

	template <typename T, typename C>
	[[nodiscard]] constexpr bool operator==(basic_string_view<T, C> lhs, basic_string_view<T, C> rhs) {
		return lhs.compare(rhs) == 0;
	}

	template <typename T, typename C>
	[[nodiscard]] constexpr auto operator<=>(basic_string_view<T, C> lhs, basic_string_view<T, C> rhs) {
		return lhs.compare(rhs) <=> 0;
	}

	// TODO operator <<
	// TODO hash function

	using string_view = basic_string_view<char>;
	using wstring_view = basic_string_view<wchar_t>;
	using u8string_view = basic_string_view<char8_t>;
	using u16string_view = basic_string_view<char16_t>;
	using u32string_view = basic_string_view<char32_t>;

	inline namespace literals {
		inline namespace string_view_literals {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"
			inline consteval kstd::string_view operator""sv(const char *str, size_t len) {
				return kstd::string_view{str, len};
			}

			inline consteval kstd::wstring_view operator""sv(const wchar_t *str, size_t len) {
				return kstd::wstring_view{str, len};
			}

			inline consteval kstd::u8string_view operator""sv(const char8_t *str, size_t len) {
				return kstd::u8string_view{str, len};
			}

			inline consteval kstd::u16string_view operator""sv(const char16_t *str, size_t len) {
				return kstd::u16string_view{str, len};
			}

			inline consteval kstd::u32string_view operator""sv(const char32_t *str, size_t len) {
				return kstd::u32string_view{str, len};
			}
#pragma GCC diagnostic pop
		}
	}
}

using namespace kstd::literals::string_view_literals;