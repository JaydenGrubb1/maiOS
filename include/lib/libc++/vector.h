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
#include <stdint.h>
#include <type_traits>

#include <lib/libc++/bits/allocator.h>
#include <lib/libc++/bits/reverse_iterator.h>
#include <lib/libc++/optional.h>
#include <lib/libc++/utility.h>
#include <lib/libc/assert.h>
#include <lib/libc/string.h>

namespace kstd {
	namespace internal {
		template <typename T>
		constexpr void __transfer(T *dest, T *src, size_t count) {
			if (count == 0) {
				return;
			}

			// VERIFY is this the correct check?
			if constexpr (std::is_trivially_copyable_v<T>) {
				memmove(dest, src, count * sizeof(T));
			} else {
				for (size_t i = 0; i < count; i++) {
					if (dest <= src) {
						std::construct_at<T>(&dest[i], kstd::move(src[i]));
					} else {
						std::construct_at<T>(&dest[count - i - 1], kstd::move(src[count - i - 1]));
					}
				}
			}
		}
		// TODO move this somewhere else
	}

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
	  private:
		size_t _capacity = 0;
		size_t _size = 0;
		Alloc _alloc = {};
		T *_data = nullptr;

		constexpr T *__insert_space(T *ptr, size_t count) {
			if (_capacity >= _size + count) {
				internal::__transfer(ptr + count, ptr, _size - (ptr - _data));
			} else {
				T *new_data = _alloc.allocate(_size + count); // TODO grow by some factor
				assert(new_data);

				auto len = ptr - _data;
				internal::__transfer(new_data, _data, len);
				internal::__transfer(new_data + count + len, ptr, _size - len);

				_alloc.deallocate(_data, _capacity);
				_data = new_data;
				_capacity = _size + count; // TODO grow by some factor
				ptr = new_data + len;
			}

			return ptr;
		}

	  public:
#pragma region Constructors
		/**
		 * @brief Construct a new vector object
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(void) = default;

		/**
		 * @brief Construct a new vector object
		 *
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr explicit vector(const Alloc &alloc) : _alloc(alloc) {}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param count The number of elements to construct
		 * @param value The value to initialize elements of the vector with
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(size_t count, const T &value, const Alloc &alloc = Alloc())
			: _capacity(count), _size(count), _alloc(alloc) {
			_data = _alloc.allocate(count);
			assert(_data);

			for (size_t i = 0; i < count; i++) {
				_data[i] = value;
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param count The number of elements to construct
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr explicit vector(size_t count, const Alloc &alloc = Alloc())
			: _capacity(count), _size(count), _alloc(alloc) {
			_data = _alloc.allocate(count);
			assert(_data);

			for (size_t i = 0; i < count; i++) {
				std::construct_at<T>(&_data[i]);
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @tparam Iter The type of the iterators
		 * @param first The beginning of the range
		 * @param last The end of the range
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		template <typename Iter>
		constexpr vector(Iter first, Iter last, const Alloc &alloc = Alloc())
			requires(!std::is_integral_v<Iter>)
			: _alloc(alloc) {
			_data = _alloc.allocate(last - first); // TODO Use kstd::distance
			assert(_data);

			size_t i = 0;
			for (auto &item = first; item != last; item++) {
				_data[i++] = *item;
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to copy
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(const vector &other)
			: _capacity(other._capacity), _size(other._size), _alloc(other._alloc) {
			_data = _alloc.allocate(other._capacity);
			assert(_data);

			for (size_t i = 0; i < other._size; i++) {
				_data[i] = other._data[i];
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to copy
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(const vector &other, const Alloc &alloc)
			: _capacity(other._capacity), _size(other._size), _alloc(alloc) {
			_data = _alloc.allocate(other._capacity);
			assert(_data);

			for (size_t i = 0; i < other._size; i++) {
				_data[i] = other._data[i];
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to move
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(vector &&other) noexcept
			: _capacity(other._capacity), _size(other._size), _alloc(other._alloc) {
			_data = other._data;
			other._data = nullptr;
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to move
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(vector &&other, const Alloc &alloc)
			: _capacity(other._capacity), _size(other._size), _alloc(alloc) {
			if (alloc == other._alloc) {
				_data = other._data;
				other._data = nullptr;
			} else {
				_data = _alloc.allocate(other._capacity);
				assert(_data);

				for (size_t i = 0; i < other._size; i++) {
					_data[i] = kstd::move(other._data[i]);
				}
			}
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param list The initializer list to copy
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(std::initializer_list<T> list, const Alloc &alloc = Alloc())
			: _capacity(list.size()), _size(list.size()), _alloc(alloc) {
			_data = _alloc.allocate(list.size());
			assert(_data);

			size_t i = 0;
			for (auto &item : list) {
				_data[i++] = item;
			}
		}
#pragma endregion

		/**
		 * @brief Destroy the vector object
		 */
		constexpr ~vector(void) {
			clear();
			_alloc.deallocate(_data, _capacity);
		}

#pragma region Assignment Operators and Functions
		constexpr vector &operator=(const vector &other) {
		}
		// TODO Implement this

		constexpr vector &operator=(vector &&other) {
		}
		// TODO Implement this

		constexpr vector &operator=(std::initializer_list<T> list) {
		}
		// TODO Implement this

		constexpr void assign(size_t count, const T &value) {
		}
		// TODO Implement this

		template <typename Iter>
		constexpr void assign(Iter first, Iter last) {
		}
		// TODO Implement this

		constexpr void assign(std::initializer_list<T> list) {
		}
		// TODO Implement this
#pragma endregion

		/**
		 * @brief Returns the allocator associated with the vector
		 *
		 * @return The allocator associated with the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/get_allocator @endlink
		 */
		[[nodiscard]] constexpr Alloc get_allocator(void) const {
			return _alloc;
		}

		/**
		 * @brief Check if the vector is empty
		 *
		 * @return true if the vector is empty, false otherwise
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/empty @endlink
		 */
		[[nodiscard]] constexpr bool empty(void) const {
			return _size == 0;
		}

		/**
		 * @brief Returns the number of elements in the vector
		 *
		 * @return The number of elements in the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/size @endlink
		 */
		[[nodiscard]] constexpr size_t size(void) const {
			return _size;
		}

		/**
		 * @brief Returns the maximum number of elements the vector can hold
		 *
		 * @return The maximum number of elements the vector can hold
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/max_size @endlink
		 */
		[[nodiscard]] constexpr size_t max_size(void) const {
			return INTMAX_MAX / sizeof(T);
		}

		/**
		 * @brief Returns the number of elements that can be held in currently allocated storage
		 *
		 * @return The number of elements that can be held in currently allocated storage
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/capacity @endlink
		 */
		[[nodiscard]] constexpr size_t capacity(void) const {
			return _capacity;
		}

		/**
		 * @brief Reserve storage for at least cap elements
		 *
		 * @param cap The number of elements to reserve storage for
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/reserve @endlink
		 */
		constexpr void reserve(size_t cap) {
			assert(cap < max_size());
			if (cap < _capacity) {
				return;
			}

			T *new_data = _alloc.allocate(cap);
			assert(new_data);

			internal::__transfer(new_data, _data, _size);

			_alloc.deallocate(_data, _capacity);
			_data = new_data;
			_capacity = cap;
		}

		/**
		 * @brief Reduce capacity to the current size
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/shrink_to_fit @endlink
		 */
		constexpr void shrink_to_fit(void) {
			if (_capacity == _size) {
				return;
			}

			T *new_data = _alloc.allocate(_size);
			assert(new_data);

			internal::__transfer(new_data, _data, _size);

			_alloc.deallocate(_data, _capacity);
			_data = new_data;
			_capacity = _size;
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/operator_at @endlink
		 */
		[[nodiscard]] constexpr T &operator[](size_t index) {
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/operator_at @endlink
		 */
		[[nodiscard]] constexpr T &operator[](size_t index) const {
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index, or an empty optional if the index is out of bounds
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/at @endlink
		 *
		 * @note This function does not conform to the C++ standard
		 */
		[[nodiscard]] constexpr optional<T> at(size_t index) {
			if (index >= _size) {
				return {};
			}
			return _data[index];
		}

		/**
		 * @brief Get the element at the given index
		 *
		 * @param index The index to get the element at
		 * @return The element at the given index, or an empty optional if the index is out of bounds
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/at @endlink
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
		 * @brief Return a reference to the first element
		 *
		 * @return A reference to the first element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/front @endlink
		 */
		[[nodiscard]] constexpr T &front(void) {
			return _data[0];
		}

		/**
		 * @brief Return a reference to the first element
		 *
		 * @return A reference to the first element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/front @endlink
		 */
		[[nodiscard]] constexpr T &front(void) const {
			return _data[0];
		}

		/**
		 * @brief Return a reference to the last element
		 *
		 * @return A reference to the last element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/back @endlink
		 */
		[[nodiscard]] constexpr T &back(void) {
			return _data[_size - 1];
		}

		/**
		 * @brief Return a reference to the last element
		 *
		 * @return A reference to the last element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/back @endlink
		 */
		[[nodiscard]] constexpr T &back(void) const {
			return _data[_size - 1];
		}

		/**
		 * @brief Return a pointer to the underlying array
		 *
		 * @return A pointer to the underlying array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/data @endlink
		 */
		[[nodiscard]] constexpr T *data(void) {
			return _data;
		}

		/**
		 * @brief Return a pointer to the underlying array
		 *
		 * @return A pointer to the underlying array
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/data @endlink
		 */
		[[nodiscard]] constexpr const T *data(void) const {
			return _data;
		}

		/**
		 * @brief Return an iterator to the beginning of the vector
		 *
		 * @return An iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr T *begin(void) {
			return _data;
		}

		/**
		 * @brief Return an iterator to the beginning of the vector
		 *
		 * @return An iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr const T *begin(void) const {
			return _data;
		}

		/**
		 * @brief Return a const iterator to the beginning of the vector
		 *
		 * @return A const iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr const T *cbegin(void) const {
			return _data;
		}

		/**
		 * @brief Return an iterator past the end of the vector
		 *
		 * @return An iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr T *end(void) {
			return _data + _size;
		}

		/**
		 * @brief Return an iterator past the end of the vector
		 *
		 * @return An iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr const T *end(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a const iterator past the end of the vector
		 *
		 * @return A const iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr const T *cend(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the vector
		 *
		 * @return A reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<T *> rbegin(void) {
			return kstd::reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the vector
		 *
		 * @return A reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a const reverse iterator to the beginning of the vector
		 *
		 * @return A const reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crbegin(void) const {
			return kstd::reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a reverse iterator past the end of the vector
		 *
		 * @return A reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<T *> rend(void) {
			return kstd::reverse_iterator(_data);
		}

		/**
		 * @brief Return a reverse iterator past the end of the vector
		 *
		 * @return A reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> rend(void) const {
			return kstd::reverse_iterator(_data);
		}

		/**
		 * @brief Return a const reverse iterator past the end of the vector
		 *
		 * @return A const reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr kstd::reverse_iterator<const T *> crend(void) const {
			return kstd::reverse_iterator(_data);
		}

		/**
		 * @brief Erases all elements from the container
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/clear @endlink
		 */
		constexpr void clear(void) {
			for (size_t i = 0; i < _size; i++) {
				std::destroy_at(&_data[i]);
			}
			_size = 0;
		}

		constexpr T *erase(const T *pos) {
			return erase(pos, pos + 1);
		}

		constexpr T *erase(const T *first, const T *last) {
			for (auto item = first; item != last; item++) {
				std::destroy_at(&item);
			}

			auto ptr = const_cast<T *>(first);
			auto dist = last - first;
			internal::__transfer(ptr, ptr + dist, _size - (ptr - _data) - dist);

			_size -= dist;
			return ptr;
		}

		constexpr T *insert(const T *pos, const T &value) {
			auto ptr = __insert_space(const_cast<T *>(pos), 1);
			*ptr = value;
			_size++;
			return ptr;
		}

		constexpr T *insert(const T *pos, T &&value) {
			auto ptr = __insert_space(const_cast<T *>(pos), 1);
			*ptr = kstd::move(value);
			_size++;
			return ptr;
		}

		constexpr T *insert(const T *pos, size_t count, const T &value) {
			auto ptr = __insert_space(const_cast<T *>(pos), count);

			for (size_t i = 0; i < count; i++) {
				ptr[i] = value;
			}

			_size += count;
			return ptr;
		}

		template <typename Iter>
		constexpr T *insert(const T *pos, Iter first, Iter last)
			requires(!std::is_integral_v<Iter>)
		{
			auto ptr = __insert_space(const_cast<T *>(pos), last - first); // TODO Use kstd::distance

			size_t i = 0;
			for (auto item = first; item != last; item++) {
				ptr[i++] = *item;
			}
			_size += i;

			return ptr;
		}

		constexpr T *insert(const T *pos, std::initializer_list<T> list) {
			auto ptr = __insert_space(const_cast<T *>(pos), list.size());

			size_t i = 0;
			for (auto &item : list) {
				ptr[i++] = item;
			}
			_size += i;

			return ptr;
		}

		template <typename... Args>
		constexpr T *emplace(const T *pos, Args &&...args) {
			// TODO Implement this
		}

		constexpr void push_back(const T &value) {
			// TODO Implement this
		}

		constexpr void push_back(T &&value) {
			// TODO Implement this
		}

		template <typename... Args>
		constexpr T &emplace_back(Args &&...args) {
			// TODO Implement this
		}

		constexpr void pop_back(void) {
			// TODO Implement this
		}

		constexpr void resize(size_t count) {
			// TODO Implement this
		}

		constexpr void resize(size_t count, const T &value) {
			// TODO Implement this
		}

		constexpr void swap(vector &other) {
			// TODO Implement this
		}
	};
}