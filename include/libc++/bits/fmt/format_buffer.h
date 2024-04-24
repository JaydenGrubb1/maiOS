/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2024-04-21
 * @brief Various type erased buffers for formatting
 *
 * Copyright (c) 2024, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <bits/algo_basic.h>
#include <bits/back_insert_iterator.h>
#include <bits/iterator_traits.h>
#include <utility>

namespace std::__detail {
	inline constexpr size_t __format_buffer_size = 256;

	template <typename Char>
	struct __format_buf {
		using value_type = Char;

		Char *_ptr = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;

		explicit constexpr __format_buf(size_t size)
			: _size(0), _capacity(size) {}

		constexpr __format_buf(Char *ptr, size_t size, size_t capacity)
			: _ptr(ptr), _size(size), _capacity(capacity) {}

		~__format_buf() = default;

		__format_buf(const __format_buf &) = delete;
		__format_buf &operator=(const __format_buf &) = delete;

		virtual void grow(size_t new_size) = 0;

		constexpr void push_back(const Char value) {
			if (_size >= _capacity) {
				grow(_size + 1);
			}
			_ptr[_size++] = value;
		}
	};

	template <typename Iter, typename Char>
	struct __format_iter_buf : public __format_buf<Char> {
		using difference_type = iterator_traits<Iter>::difference_type;

		Char _data[__format_buffer_size];
		Iter _iter;
		difference_type _max;

		explicit __format_iter_buf(Iter iter, difference_type size = __format_buffer_size)
			: __format_buf<Char>(_data, 0, __format_buffer_size), _iter(std::move(iter)), _max(size) {}

		~__format_iter_buf() {
			if (this->_size != 0) {
				flush();
			}
		}

		void flush(void) {
			auto size = std::min(this->_size, static_cast<size_t>(_max));
			// _iter = std::copy_n(this->_ptr, size, _iter); // TODO add copy_n
			for (size_t i = 0; i < size; i++) {
				*_iter++ = this->_ptr[i];
			}
			this->_size = 0;
			_max -= size;
		}

		void grow(size_t) override final {
			if (this->_size != __format_buffer_size) {
				return;
			}
			flush();
		}

		Iter out(void) {
			flush();
			return std::move(_iter);
		}

		difference_type count(void) {
			return this->_size;
		}
	};

	template <typename Char>
	struct __format_count_buf : public __format_buf<Char> {
		Char _data[__format_buffer_size]; // TODO why does it need any buffer
		size_t _count = 0;

		__format_count_buf(void)
			: __format_buf<Char>(_data, 0, __format_buffer_size) {}

		void grow(size_t) override final {
			if (this->_size != __format_buffer_size) {
				return;
			}
			_count += this->_size;
			this->_size = 0;
		}

		size_t count(void) {
			return _count + this->_size;
		}
	};

	template <typename Char>
	using __format_iter = std::back_insert_iterator<__format_buf<Char>>;

	template <typename Char>
	struct __nop_iter {
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