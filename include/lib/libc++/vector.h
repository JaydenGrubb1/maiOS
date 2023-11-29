/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-29
 * @brief Class template encapsulating a dynamic-size array
 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <initializer_list>
#include <memory> // TODO replace with <lib/libc++/memory.h> ???

#include <lib/libc++/bits/allocator.h>
#include <lib/libc++/utility.h>
#include <lib/libc/assert.h>

namespace kstd {
	/**
	 * @brief Class template encapsulating a dynamic-size array
	 *
	 * @tparam T The type of the elements in the vector
	 * @tparam Alloc The allocator type used to allocate memory for the vector
	 *
	 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
	 */
	template <typename T, typename Alloc = allocator<T>>
	class vector {
	  public:
#pragma region Constructors
		constexpr vector(void) = default;

		constexpr explicit vector(const Alloc &alloc) : _alloc(alloc) {}

		constexpr vector(size_t count, const T &value, const Alloc &alloc = Alloc())
			: _capacity(count), _size(count), _alloc(alloc) {
			_data = _alloc.allocate(count);
			assert(_data != nullptr);

			for (size_t i = 0; i < count; i++) {
				_data[i] = value;
			}
		}

		constexpr explicit vector(size_t count, const Alloc &alloc = Alloc())
			: _capacity(count), _size(count), _alloc(alloc) {
			_data = _alloc.allocate(count);
		}

		// template <typename Iter>
		// constexpr vector(Iter first, Iter last, const Alloc &alloc = Alloc()) : _alloc(alloc) {}
		// TODO Add requirements to avoid ambiguity with the above constructor

		constexpr vector(const vector &other)
			: _capacity(other._capacity), _size(other._size), _alloc(other._alloc) {
			_data = _alloc.allocate(other._capacity);
			assert(_data != nullptr);

			for (size_t i = 0; i < other._size; i++) {
				_data[i] = other._data[i];
			}
		}

		constexpr vector(const vector &other, const Alloc &alloc)
			: _capacity(other._capacity), _size(other._size), _alloc(alloc) {
			_data = _alloc.allocate(other._capacity);
			assert(_data != nullptr);

			for (size_t i = 0; i < other._size; i++) {
				_data[i] = other._data[i];
			}
		}

		constexpr vector(vector &&other) noexcept
			: _capacity(other._capacity), _size(other._size), _alloc(other._alloc) {
			_data = other._data;
			other._data = nullptr;
		}

		constexpr vector(vector &&other, const Alloc &alloc)
			: _capacity(other._capacity), _size(other._size), _alloc(alloc) {
			if (alloc == other._alloc) {
				_data = other._data;
				other._data = nullptr;
			} else {
				_data = _alloc.allocate(other._capacity);
				assert(_data != nullptr);

				for (size_t i = 0; i < other._size; i++) {
					_data[i] = kstd::move(other._data[i]);
				}
			}
		}

		constexpr vector(std::initializer_list<T> list, const Alloc &alloc = Alloc())
			: _capacity(list.size()), _size(list.size()), _alloc(alloc) {
			_data = _alloc.allocate(list.size());
			assert(_data != nullptr);

			for (size_t i = 0; i < list.size(); i++) {
				_data[i] = list[i];
			}
		}

		// TODO Range constructor
#pragma endregion

		constexpr ~vector(void) {
			for (size_t i = 0; i < _size; i++) {
				std::destroy_at(&_data[i]);
			}

			_alloc.deallocate(_data, _capacity);
		}

	  private:
		size_t _capacity = 0;
		size_t _size = 0;
		Alloc _alloc = {};
		T *_data = nullptr;
	};
}