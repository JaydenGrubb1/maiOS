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
#include <internal/reverse_iterator.h>
#include <optional.h>

namespace kstd {
	/**
	 * @brief Class template for referencing a constant contiguous sequence of characters
	 *
	 * @tparam T The underlying character type
	 *
	 * @link https://en.cppreference.com/w/cpp/string/basic_string_view @endlink
	 */
	template <typename T>
	class basic_string_view {
	  private:
		const T *_data;
		size_t _size;

	  public:
		/**
		 * @brief Special value to indicate that the end of the string was reached
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/npos @endlink
		 */
		static constexpr size_t npos = -1;

#pragma region Constructors
		/**
		 * @brief Construct a new string view object
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view @endlink
		 */
		constexpr basic_string_view(void) : _data(nullptr), _size(0) {}

		/**
		 * @brief Construct a new string view object
		 *
		 * @param other The string view to copy
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view @endlink
		 */
		constexpr basic_string_view(const basic_string_view &other) = default;

		/**
		 * @brief Construct a new string view object
		 *
		 * @param data The data to view
		 * @param size The size of the data
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view @endlink
		 */
		constexpr basic_string_view(const T *data, size_t size) : _data(data), _size(size) {}

		/**
		 * @brief Construct a new string view object
		 *
		 * @param data The data to view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view @endlink
		 */
		constexpr basic_string_view(const T *data) : _data(data), _size(0) {
			while (_data[_size] != static_cast<T>(0)) {
				_size++;
			}
		}

		/**
		 * @brief Construct a new string view object
		 *
		 * @tparam Iter The iterator type
		 * @tparam End The end iterator type
		 * @param begin The begin iterator
		 * @param end The end iterator
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/basic_string_view @endlink
		 */
		template <typename Iter, typename End>
		constexpr basic_string_view(Iter begin, End end)
			requires(!std::is_convertible_v<End, size_t>)
			: _data(&*begin), _size(end - begin) {}
		// TODO more trait constraints
		// TODO std::to_address(begin)
#pragma endregion

		/**
		 * @brief Assigns a new string view to the string view object
		 *
		 * @param other The string view to copy
		 * @return The string view object
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/operator%3D @endlink
		 */
		constexpr basic_string_view &operator=(const basic_string_view &other) = default;

#pragma region Capacity Functions
		/**
		 * @brief Returns the number of characters in the string view
		 *
		 * @return The number of characters in the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/size @endlink
		 */
		[[nodiscard]] constexpr size_t size(void) const {
			return _size;
		}

		/**
		 * @brief Returns the number of characters in the string view
		 *
		 * @return The number of characters in the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/size @endlink
		 */
		[[nodiscard]] constexpr size_t length(void) const {
			return _size;
		}

		/**
		 * @brief Returns the maximum number of characters the string view can hold
		 *
		 * @return The maximum number of characters the string view can hold
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/max_size @endlink
		 */
		[[nodiscard]] constexpr size_t max_size(void) const {
			return INTMAX_MAX / sizeof(T);
		}

		/**
		 * @brief Checks if the string view is empty
		 *
		 * @return true if the string view is empty, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/empty @endlink
		 */
		[[nodiscard]] constexpr bool empty(void) const {
			return _size == 0;
		}
#pragma endregion

#pragma region Accessor Functions
		/**
		 * @brief Get the character at the given index
		 *
		 * @param index The index to get the character at
		 * @return The character at the given index
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/operator_at @endlink
		 */
		[[nodiscard]] constexpr const T &operator[](size_t index) const {
			return _data[index];
		}

		/**
		 * @brief Get the character at the given index
		 *
		 * @param index The index to get the character at
		 * @return The character at the given index, or an empty optional if the index is out of bounds
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/at @endlink
		 *
		 * @note This function does not conform to the C++ standard
		 */
		[[nodiscard]] constexpr optional<T> at(size_t index) const {
			if (index >= _size) {
				return {};
			}
			return _data[index];
		}

		/**
		 * @brief Return a reference to the first character
		 *
		 * @return A reference to the first character
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/front @endlink
		 */
		[[nodiscard]] constexpr const T &front(void) const {
			return _data[0];
		}

		/**
		 * @brief Return a reference to the last character
		 *
		 * @return A reference to the last character
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/back @endlink
		 */
		[[nodiscard]] constexpr const T &back(void) const {
			return _data[_size - 1];
		}

		/**
		 * @brief Return a pointer to the underlying character array
		 *
		 * @return A pointer to the underlying character array
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/data @endlink
		 */
		[[nodiscard]] constexpr const T *data(void) const {
			return _data;
		}
#pragma endregion

#pragma region Iterator Functions
		/**
		 * @brief Return an iterator to the beginning of the string view
		 *
		 * @return An iterator to the beginning of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/begin @endlink
		 */
		[[nodiscard]] constexpr const T *begin(void) const {
			return _data;
		}

		/**
		 * @brief Return a const iterator to the beginning of the strign view
		 *
		 * @return A const iterator to the beginning of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/begin @endlink
		 */
		[[nodiscard]] constexpr const T *cbegin(void) const {
			return _data;
		}

		/**
		 * @brief Return an iterator past the end of the string view
		 *
		 * @return An iterator past the end of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/end @endlink
		 */
		[[nodiscard]] constexpr const T *end(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a const iterator past the end of the string view
		 *
		 * @return A const iterator past the end of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/end @endlink
		 */
		[[nodiscard]] constexpr const T *cend(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the string view
		 *
		 * @return A reverse iterator to the beginning of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a const reverse iterator to the beginning of the string view
		 *
		 * @return A const reverse iterator to the beginning of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a reverse iterator past the end of the string view
		 *
		 * @return A reverse iterator past the end of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rend(void) const {
			return kstd::reverse_iterator(_data);
		}

		/**
		 * @brief Return a const reverse iterator past the end of the string view
		 *
		 * @return A const reverse iterator past the end of the string view
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crend(void) const {
			return kstd::reverse_iterator(_data);
		}
#pragma endregion

#pragma region Modifier Functions
		/**
		 * @brief Remove n leading characters from the string view
		 * i.e. moves the start of the view forward by n characters
		 *
		 * @param n The number of characters to remove
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/remove_prefix @endlink
		 */
		constexpr void remove_prefix(size_t n) {
			_data += n;
			_size -= n;
		}

		/**
		 * @brief Remove n trailing characters from the string view
		 * i.e. move the end of the view back by n characters
		 *
		 * @param n The number of characters to remove
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/remove_suffix @endlink
		 */
		constexpr void remove_suffix(size_t n) {
			_size -= n;
		}

		/**
		 * @brief Swaps the contents of the string view with the given string view
		 *
		 * @param other The string view to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/swap @endlink
		 */
		constexpr void swap(basic_string_view &other) {
			const T *tmp_data = _data;
			size_t tmp_size = _size;

			_data = other._data;
			_size = other._size;

			other._data = tmp_data;
			other._size = tmp_size;
		}
#pragma endregion

		/**
		 * @brief Copies count characters starting at pos to the array pointed at by dest
		 *
		 * @param dest The destination array to copy to
		 * @param count The number of characters to copy
		 * @param pos The position in the string view to start the copy from
		 * @return The number of characters copied
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/copy @endlink
		 */
		constexpr size_t copy(T *dest, size_t count, size_t pos = 0) const {
			if (pos > _size) {
				return 0;
			}
			size_t len = min(count, _size - pos);
			for (size_t i = 0; i < len; i++) {
				dest[i] = _data[pos + i];
			}
			return len;
		}

		/**
		 * @brief Return a view of a substring starting at pos with count characters
		 *
		 * @param pos The position to start the substring from
		 * @param count The number of character in the substring
		 * @return A view of a substring
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/substr @endlink
		 */
		[[nodiscard]] constexpr basic_string_view substr(size_t pos = 0, size_t count = npos) const {
			if (pos > _size) {
				return basic_string_view();
			}
			return basic_string_view(_data + pos, min(count, _size - pos));
		}

#pragma region Comparison Functions
		/**
		 * @brief Compares this to another character sequence
		 *
		 * @param other The character sequence to compare to
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(basic_string_view other) const {
			const size_t count = min(_size, other._size);
			for (size_t i = 0; i < count; i++) {
				if (_data[i] < other._data[i]) {
					return -1;
				} else if (_data[i] > other._data[i]) {
					return 1;
				}
			}
			if (_size < other._size) {
				return -1;
			} else if (_size > other._size) {
				return 1;
			}
			return 0;
		}

		/**
		 * @brief Compare this to another character sequence
		 *
		 * @param pos The start of this substring
		 * @param count The number of characters in this substring
		 * @param other The character sequence to compare to
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(size_t pos, size_t count, basic_string_view other) const {
			return substr(pos, count).compare(other);
		}

		/**
		 * @brief Compare this to another character sequence
		 *
		 * @param pos1 The start of this substring
		 * @param count1 The number of characters in this substring
		 * @param other The number of characters in this substring
		 * @param pos2 The start of the other substring
		 * @param count2 The number of characters in the other substring
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(size_t pos1, size_t count1, basic_string_view other, size_t pos2, size_t count2) const {
			return substr(pos1, count1).compare(other.substr(pos2, count2));
		}

		/**
		 * @brief Compare this to another character sequence
		 *
		 * @param other The character sequence to compare to
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(const T *other) const {
			return compare(basic_string_view(other));
		}

		/**
		 * @brief Compare this to another character sequence
		 *
		 * @param pos The start of this substring
		 * @param count The number of characters in this substring
		 * @param other The character sequence to compare to
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(size_t pos, size_t count, const T *other) const {
			return substr(pos, count).compare(basic_string_view(other));
		}

		/**
		 * @brief Compare this to another character sequence
		 *
		 * @param pos1 The start of this substring
		 * @param count1 The number of characters in this substring
		 * @param other The character sequence to compare to
		 * @param count2 The number of characters in the compared character sequence
		 * @return 1 if other is greater than this, -1 if other is less than this, 0 if they are equal
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/compare @endlink
		 */
		[[nodiscard]] constexpr int compare(size_t pos1, size_t count1, const T *other, size_t count2) const {
			return substr(pos1, count1).compare(basic_string_view(other, count2));
		}
#pragma endregion

		/**
		 * @brief Find the first substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find @endlink
		 */
		[[nodiscard]] constexpr size_t find(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (substr(i).starts_with(other)) {
					return i;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the first character equal to the given character
		 *
		 * @param ch The character to find
		 * @param pos The position to start searching from
		 * @return The index of the character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find @endlink
		 */
		[[nodiscard]] constexpr size_t find(T ch, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (_data[i] == ch) {
					return i;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the first substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from
		 * @param count The size of the character sequence
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find @endlink
		 */
		[[nodiscard]] constexpr size_t find(const T *other, size_t pos, size_t count) const {
			return find(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the first substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find @endlink
		 */
		[[nodiscard]] constexpr size_t find(const T *other, size_t pos = 0) const {
			return find(basic_string_view(other), pos);
		}

		/**
		 * @brief Find the last substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from (right to left)
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rfind @endlink
		 */
		[[nodiscard]] constexpr size_t rfind(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (substr(_size - i - 1).starts_with(other)) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the last character equal to the given character
		 *
		 * @param ch The character to find
		 * @param pos The position to start searching from (right to left)
		 * @return The index of the character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rfind @endlink
		 */
		[[nodiscard]] constexpr size_t rfind(T ch, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (_data[_size - i - 1] == ch) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the last substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from (right to left)
		 * @param count The size of the character sequence
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rfind @endlink
		 */
		[[nodiscard]] constexpr size_t rfind(const T *other, size_t pos, size_t count) const {
			return rfind(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the last substring equal to the given character sequence
		 *
		 * @param other The character sequence to find
		 * @param pos The position to start searching from (right to left)
		 * @return The index of the substring, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/rfind @endlink
		 */
		[[nodiscard]] constexpr size_t rfind(const T *other, size_t pos = npos) const {
			return rfind(basic_string_view(other), pos);
		}

		/**
		 * @brief Check if the string view starts with a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view starts with the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with @endlink
		 */
		[[nodiscard]] constexpr bool starts_with(basic_string_view other) const {
			return compare(0, other.size(), other) == 0;
		}

		/**
		 * @brief Check if the string view start with a given character
		 *
		 * @param ch The character to check for
		 * @return true if the string view starts with the character, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with @endlink
		 */
		[[nodiscard]] constexpr bool starts_with(T ch) const {
			return size() > 0 && front() == ch;
		}

		/**
		 * @brief Check if the string view starts with a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view starts with the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/starts_with @endlink
		 */
		[[nodiscard]] constexpr bool starts_with(const T *other) const {
			return starts_with(basic_string_view(other));
		}

		/**
		 * @brief Check if the string view ends with a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view ends with the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/ends_with @endlink
		 */
		[[nodiscard]] constexpr bool ends_with(basic_string_view other) const {
			return compare(size() - other.size(), other.size(), other) == 0;
		}

		/**
		 * @brief Check if the string view ends with a given character
		 *
		 * @param ch The character to check for
		 * @return true if the string view ends with the character, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/ends_with @endlink
		 */
		[[nodiscard]] constexpr bool ends_with(T ch) const {
			return size() > 0 && back() == ch;
		}

		/**
		 * @brief Check if the string view ends with a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view ends with the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/ends_with @endlink
		 */
		[[nodiscard]] constexpr bool ends_with(const T *other) const {
			return ends_with(basic_string_view(other));
		}

		/**
		 * @brief Check if the string view contains a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view contains the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/contains @endlink
		 */
		[[nodiscard]] constexpr bool contains(basic_string_view other) const {
			return find(other) != npos;
		}

		/**
		 * @brief Check if the string view contains a given character
		 *
		 * @param ch The character to check for
		 * @return true if the string view contains the character, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/contains @endlink
		 */
		[[nodiscard]] constexpr bool contains(T ch) const {
			return find(ch) != npos;
		}

		/**
		 * @brief Check if the string view contains a given character sequence
		 *
		 * @param other The character sequence to check for
		 * @return true if the string view contains the character sequence, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/contains @endlink
		 */
		[[nodiscard]] constexpr bool contains(const T *other) const {
			return find(other) != npos;
		}

		/**
		 * @brief Find the first instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searhing from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_of(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (other.contains(_data[i])) {
					return i;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the first instance of a given character
		 *
		 * @param ch The character to check for
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_of(T ch, size_t pos = 0) const {
			return find_first_of(basic_string_view(&ch, 1), pos);
		}

		/**
		 * @brief Find the first instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @param count The number of possible characters to find
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_of(const T *other, size_t pos, size_t count) const {
			return find_first_of(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the first instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_of(const T *other, size_t pos = 0) const {
			return find_first_of(basic_string_view(other), pos);
		}

		/**
		 * @brief Find the last instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searhing from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_of(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (other.contains(_data[_size - i - 1])) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the last instance of a given character
		 *
		 * @param ch The character to check for
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_of(T ch, size_t pos = npos) const {
			return find_first_of(basic_string_view(&ch, 1), pos);
		}

		/**
		 * @brief Find the last instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @param count The number of possible characters to find
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_of(const T *other, size_t pos, size_t count) const {
			return find_last_of(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the last instance of any character from a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_of(const T *other, size_t pos = npos) const {
			return find_last_of(basic_string_view(other), pos);
		}

		/**
		 * @brief Find the first instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searhing from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_not_of(basic_string_view other, size_t pos = 0) const {
			for (size_t i = pos; i < _size; i++) {
				if (!other.contains(_data[i])) {
					return i;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the first instance not of a given character
		 *
		 * @param ch The character to check for
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_not_of(T ch, size_t pos = 0) const {
			return find_first_not_of(basic_string_view(&ch, 1), pos);
		}

		/**
		 * @brief Find the first instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @param count The number of possible characters to find
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_not_of(const T *other, size_t pos, size_t count) const {
			return find_first_not_of(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the first instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_first_not_of(const T *other, size_t pos = 0) const {
			return find_first_not_of(basic_string_view(other), pos);
		}

		/**
		 * @brief Find the last instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searhing from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_not_of(basic_string_view other, size_t pos = npos) const {
			for (size_t i = 0; i < _size - pos; i++) {
				if (!other.contains(_data[_size - i - 1])) {
					return _size - i - 1;
				}
			}
			return npos;
		}

		/**
		 * @brief Find the last instance not of a given character
		 *
		 * @param ch The character to check for
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_not_of(T ch, size_t pos = npos) const {
			return find_last_not_of(basic_string_view(&ch, 1), pos);
		}

		/**
		 * @brief Find the last instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @param count The number of possible characters to find
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
		[[nodiscard]] constexpr size_t find_last_not_of(const T *other, size_t pos, size_t count) const {
			return find_last_not_of(basic_string_view(other, count), pos);
		}

		/**
		 * @brief Find the last instance of any character not in a given sequence
		 *
		 * @param other The sequence of characters to check from
		 * @param pos The position to start searching from
		 * @return The position of the found character, or npos if not found
		 *
		 * @link https://en.cppreference.com/w/cpp/string/basic_string_view/find_first_of @endlink
		 */
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

	template <typename T>
	[[nodiscard]] constexpr bool operator==(basic_string_view<T> lhs, basic_string_view<T> rhs) {
		return lhs.compare(rhs) == 0;
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator<=>(basic_string_view<T> lhs, basic_string_view<T> rhs) {
		return lhs.compare(rhs) <=> 0;
	}

	// TODO operator <<
	// TODO hash function

	/**
	 * @brief A contiguous sequence of char characters
	 *
	 */
	using string_view = basic_string_view<char>;

	/**
	 * @brief A contiguous sequence of wchar_t characters
	 *
	 */
	using wstring_view = basic_string_view<wchar_t>;

	/**
	 * @brief A contiguous sequence of char8_t characters
	 *
	 */
	using u8string_view = basic_string_view<char8_t>;

	/**
	 * @brief A contiguous sequence of char16_t characters
	 *
	 */
	using u16string_view = basic_string_view<char16_t>;

	/**
	 * @brief A contiguous sequence of char32_t characters
	 *
	 */
	using u32string_view = basic_string_view<char32_t>;

	inline namespace literals {
		inline namespace string_view_literals {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wliteral-suffix"
			/**
			 * @brief Construct a new string view object
			 *
			 */
			inline consteval kstd::string_view operator""sv(const char *str, size_t len) {
				return kstd::string_view{str, len};
			}

			/**
			 * @brief Construct a new wstring view object
			 *
			 */
			inline consteval kstd::wstring_view operator""sv(const wchar_t *str, size_t len) {
				return kstd::wstring_view{str, len};
			}

			/**
			 * @brief Construct a new u8string view object
			 *
			 */
			inline consteval kstd::u8string_view operator""sv(const char8_t *str, size_t len) {
				return kstd::u8string_view{str, len};
			}

			/**
			 * @brief Construct a new u16string view object
			 *
			 */
			inline consteval kstd::u16string_view operator""sv(const char16_t *str, size_t len) {
				return kstd::u16string_view{str, len};
			}

			/**
			 * @brief Construct a new u32string view object
			 *
			 */
			inline consteval kstd::u32string_view operator""sv(const char32_t *str, size_t len) {
				return kstd::u32string_view{str, len};
			}
#pragma GCC diagnostic pop
		}
	}
}

using namespace kstd::literals::string_view_literals;