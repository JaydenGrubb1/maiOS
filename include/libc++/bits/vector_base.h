/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-11-29
 * @brief Base implementation of the vector class
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <initializer_list>
#include <type_traits>

#include <bits/algo_basic.h>
#include <bits/allocator.h>
#include <bits/allocator_traits.h>
#include <bits/construct.h>
#include <bits/reverse_iterator.h>
#include <cassert>
#include <cstring>
#include <memory_resource>
#include <optional>
#include <utility>

namespace std {
	/**
	 * @brief Class template encapsulating a dynamic-size array
	 *
	 * @tparam T The type of the elements in the vector
	 * @tparam A The allocator type used to allocate memory for the vector
	 *
	 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
	 */
	template <typename T, typename A = allocator<T>>
	class vector {
	  public:
		using value_type = T;
		using allocator_type = A;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type &;
		using const_reference = const value_type &;
		using pointer = std::allocator_traits<A>::pointer;
		using const_pointer = std::allocator_traits<A>::const_pointer;
		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	  private:
		T *_data;
		size_t _size;
		size_t _capacity;
		[[no_unique_address]] allocator_type _alloc;

		/**
		 * @brief Inserts space for the given number of elements at the given pointer
		 *
		 * @param ptr The pointer to insert space at
		 * @param count The number of elements to insert
		 * @param exp_growth Whether or not to use exponential growth
		 * @param copy_data Whether or not to copy the data from the old array to the new one
		 * @return The pointer to the inserted space
		 */
		constexpr T *__insert_space(T *ptr, size_t count, bool exp_growth = true, bool copy_data = true) {
			if (_capacity >= _size + count) {
				if (copy_data) {
					std::move(ptr, end(), ptr + count);
				}
			} else {
				size_t new_capacity = exp_growth ? std::max(_capacity * 2, _size + count) : _size + count;

				T *new_data = _alloc.allocate(new_capacity);
				assert(new_data);

				auto len = ptr - _data;
				if (copy_data) {
					std::move(begin(), ptr, new_data);
					std::move(ptr, end(), new_data + count + len);
				}

				_alloc.deallocate(_data, _capacity);
				_data = new_data;
				_capacity = new_capacity;
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
		constexpr explicit vector(const allocator_type &alloc) : _alloc(alloc) {}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param count The number of elements to construct
		 * @param value The value to initialize elements of the vector with
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(size_t count, const T &value, const allocator_type &alloc = allocator_type())
			: _size(count), _capacity(count), _alloc(alloc) {
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
		constexpr explicit vector(size_t count, const allocator_type &alloc = allocator_type())
			: _size(count), _capacity(count), _alloc(alloc) {
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
		constexpr vector(Iter first, Iter last, const allocator_type &alloc = allocator_type())
			requires(!std::is_integral_v<Iter>)
			: _alloc(alloc) {
			_data = _alloc.allocate(last - first); // TODO Use std::distance
			assert(_data);

			size_t i = 0;
			for (auto &item = first; item != last; item++) {
				_data[i++] = *item;
			}
			_size = i;
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to copy
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(const vector &other)
			: _size(other._size), _capacity(other._size), _alloc(other._alloc) {
			_data = _alloc.allocate(other._size);
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
		constexpr vector(const vector &other, const allocator_type &alloc)
			: _size(other._size), _capacity(other._size), _alloc(alloc) {
			_data = _alloc.allocate(other._size);
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
			: _size(other._size), _capacity(other._capacity), _alloc(other._alloc) {
			_data = other._data;
			other._data = nullptr;
			other._size = 0;
		}

		/**
		 * @brief Construct a new vector object
		 *
		 * @param other The vector to move
		 * @param alloc The allocator to use
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/vector @endlink
		 */
		constexpr vector(vector &&other, const allocator_type &alloc)
			: _size(other._size), _capacity(other._capacity), _alloc(alloc) {
			if (alloc == other._alloc) {
				_data = other._data;
				other._data = nullptr;
				other._size = 0;
			} else {
				_data = _alloc.allocate(other._capacity);
				assert(_data);
				std::move(other.begin(), other.end(), _data);
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
		constexpr vector(std::initializer_list<T> list, const allocator_type &alloc = allocator_type())
			: _size(list.size()), _capacity(list.size()), _alloc(alloc) {
			_data = _alloc.allocate(list.size());
			assert(_data);

			size_t i = 0;
			for (auto &item : list) {
				_data[i++] = item;
			}
		}
#pragma endregion

#pragma region Assignment Operators and Functions
		/**
		 * @brief Copies the contents of the given vector
		 *
		 * @param other The vector to copy
		 * @return A reference to this vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/operator%3D @endlink
		 */
		constexpr vector &operator=(const vector &other) {
			if (this == &other) {
				return *this;
			}

			__insert_space(_data, other._size - _size, false, false);
			_size = other._size;

			for (size_t i = 0; i < _size; i++) {
				_data[i] = other._data[i];
			}

			return *this;
		}

		/**
		 * @brief Moves the contents of the given vector
		 *
		 * @param other The vector to move
		 * @return A reference to this vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/operator%3D @endlink
		 */
		constexpr vector &operator=(vector &&other) {
			if (this == &other) {
				return *this;
			}

			clear();
			swap(other);

			return *this;
		}

		/**
		 * @brief Copies the contents of the given initializer list
		 *
		 * @param list The initializer list to copy
		 * @return A reference to this vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/operator%3D @endlink
		 */
		constexpr vector &operator=(std::initializer_list<T> list) {
			__insert_space(_data, list.size() - _size, false, false);
			_size = list.size();

			size_t i = 0;
			for (auto &item : list) {
				_data[i++] = item;
			}

			return *this;
		}

		/**
		 * @brief Assigns count copies of value to the vector
		 *
		 * @param count The number of copies to assign
		 * @param value The value to assign
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/assign @endlink
		 */
		constexpr void assign(size_t count, const T &value) {
			__insert_space(_data, count - _size, false, false);
			_size = count;

			for (size_t i = 0; i < count; i++) {
				_data[i] = value;
			}
		}

		/**
		 * @brief Assigns elements from the range [first, last) to the vector
		 *
		 * @tparam Iter The type of the iterators
		 * @param first The beginning of the range
		 * @param last The end of the range
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/assign @endlink
		 */
		template <typename Iter>
		constexpr void assign(Iter first, Iter last)
			requires(!std::is_integral_v<Iter>)
		{
			auto len = last - first; // TODO Use std::distance
			__insert_space(_data, len - _size, false, false);
			_size = len;

			size_t i = 0;
			for (auto &item = first; item != last; item++) {
				_data[i++] = *item;
			}
		}

		/**
		 * @brief Assigns elements from the initializer list to the vector
		 *
		 * @param list The initializer list to assign
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/assign @endlink
		 */
		constexpr void assign(std::initializer_list<T> list) {
			__insert_space(_data, list.size() - _size, false, false);
			_size = list.size();

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

		/**
		 * @brief Returns the allocator associated with the vector
		 *
		 * @return The allocator associated with the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/get_allocator @endlink
		 */
		[[nodiscard]] constexpr allocator_type get_allocator(void) const {
			return _alloc;
		}

#pragma region Capacity Functions
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
			std::move(begin(), end(), new_data);

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
			std::move(begin(), end(), new_data);

			_alloc.deallocate(_data, _capacity);
			_data = new_data;
			_capacity = _size;
		}
#pragma endregion

#pragma region Accessor Functions
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
		[[nodiscard]] constexpr const T &operator[](size_t index) const {
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
		[[nodiscard]] constexpr optional<T &> at(size_t index) {
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
		[[nodiscard]] constexpr optional<const T &> at(size_t index) const {
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
		[[nodiscard]] constexpr const T &front(void) const {
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
		[[nodiscard]] constexpr const T &back(void) const {
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
#pragma endregion

#pragma region Iterator Functions
		/**
		 * @brief Return an iterator to the beginning of the vector
		 *
		 * @return An iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr iterator begin(void) {
			return _data;
		}

		/**
		 * @brief Return an iterator to the beginning of the vector
		 *
		 * @return An iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr const_iterator begin(void) const {
			return _data;
		}

		/**
		 * @brief Return a const iterator to the beginning of the vector
		 *
		 * @return A const iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/begin @endlink
		 */
		[[nodiscard]] constexpr const_iterator cbegin(void) const {
			return _data;
		}

		/**
		 * @brief Return an iterator past the end of the vector
		 *
		 * @return An iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr iterator end(void) {
			return _data + _size;
		}

		/**
		 * @brief Return an iterator past the end of the vector
		 *
		 * @return An iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr const_iterator end(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a const iterator past the end of the vector
		 *
		 * @return A const iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/end @endlink
		 */
		[[nodiscard]] constexpr const_iterator cend(void) const {
			return _data + _size;
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the vector
		 *
		 * @return A reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr reverse_iterator rbegin(void) {
			return reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a reverse iterator to the beginning of the vector
		 *
		 * @return A reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr const_reverse_iterator rbegin(void) const {
			return const_reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a const reverse iterator to the beginning of the vector
		 *
		 * @return A const reverse iterator to the beginning of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rbegin @endlink
		 */
		[[nodiscard]] constexpr const_reverse_iterator crbegin(void) const {
			return const_reverse_iterator(_data + _size);
		}

		/**
		 * @brief Return a reverse iterator past the end of the vector
		 *
		 * @return A reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr reverse_iterator rend(void) {
			return reverse_iterator(_data);
		}

		/**
		 * @brief Return a reverse iterator past the end of the vector
		 *
		 * @return A reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr const_reverse_iterator rend(void) const {
			return const_reverse_iterator(_data);
		}

		/**
		 * @brief Return a const reverse iterator past the end of the vector
		 *
		 * @return A const reverse iterator past the end of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/rend @endlink
		 */
		[[nodiscard]] constexpr const_reverse_iterator crend(void) const {
			return const_reverse_iterator(_data);
		}
#pragma endregion

#pragma region Modifier Functions
		/**
		 * @brief Erases all elements from the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/clear @endlink
		 */
		constexpr void clear(void) {
			for (size_t i = 0; i < _size; i++) {
				std::destroy_at(&_data[i]);
			}
			_size = 0;
		}

		/**
		 * @brief Erases the element at the given position from the vector
		 *
		 * @param pos The position of the element to erase
		 * @return Pointer following the last removed element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/erase @endlink
		 */
		constexpr T *erase(const T *pos) {
			return erase(pos, pos + 1);
		}

		/**
		 * @brief Erases the elements in the given range from the vector
		 *
		 * @param first The beginning of the range
		 * @param last The end of the range
		 * @return Pointer following the last removed element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/erase @endlink
		 */
		constexpr T *erase(const T *first, const T *last) {
			for (auto item = first; item != last; item++) {
				std::destroy_at(&item);
			}

			auto ptr = const_cast<T *>(first);
			std::move(const_cast<T *>(last), end(), ptr);

			_size -= (last - first);
			return ptr;
		}

		/**
		 * @brief Erases the last element of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/pop_back @endlink
		 */
		constexpr void pop_back(void) {
			erase(_data + _size - 1);
		}

		/**
		 * @brief Inserts the given value before the given position
		 *
		 * @param pos The position to insert the value before
		 * @param value The value to insert
		 * @return Pointer to the inserted value
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/insert @endlink
		 */
		constexpr T *insert(const T *pos, const T &value) {
			auto ptr = __insert_space(const_cast<T *>(pos), 1);
			*ptr = value;
			_size++;
			return ptr;
		}

		/**
		 * @brief Inserts the given value before the given position
		 *
		 * @param pos The position to insert the value before
		 * @param value The value to insert
		 * @return Pointer to the inserted value
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/insert @endlink
		 */
		constexpr T *insert(const T *pos, T &&value) {
			auto ptr = __insert_space(const_cast<T *>(pos), 1);
			*ptr = std::move(value);
			_size++;
			return ptr;
		}

		/**
		 * @brief Inserts count copies of the given value before the given position
		 *
		 * @param pos The position to insert the values before
		 * @param count The number of copies to insert
		 * @param value The value to insert
		 * @return Pointer to the first inserted value
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/insert @endlink
		 */
		constexpr T *insert(const T *pos, size_t count, const T &value) {
			auto ptr = __insert_space(const_cast<T *>(pos), count);

			for (size_t i = 0; i < count; i++) {
				ptr[i] = value;
			}

			_size += count;
			return ptr;
		}

		/**
		 * @brief Inserts the elements in the given range before the given position
		 *
		 * @tparam Iter The type of the iterators
		 * @param pos The position to insert the values before
		 * @param first The beginning of the range
		 * @param last The end of the range
		 * @return Pointer to the first inserted value
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/insert @endlink
		 */
		template <typename Iter>
		constexpr T *insert(const T *pos, Iter first, Iter last)
			requires(!std::is_integral_v<Iter>)
		{
			auto ptr = __insert_space(const_cast<T *>(pos), last - first); // TODO Use std::distance

			size_t i = 0;
			for (auto item = first; item != last; item++) {
				ptr[i++] = *item;
			}
			_size += i;

			return ptr;
		}

		/**
		 * @brief Inserts the given elements before the given position
		 *
		 * @param pos The position to insert the values before
		 * @param list The initializer list to insert
		 * @return Pointer to the first inserted value
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/insert @endlink
		 */
		constexpr T *insert(const T *pos, std::initializer_list<T> list) {
			auto ptr = __insert_space(const_cast<T *>(pos), list.size());

			size_t i = 0;
			for (auto &item : list) {
				ptr[i++] = item;
			}
			_size += i;

			return ptr;
		}

		/**
		 * @brief Inserts the given value at the end of the vector
		 *
		 * @param value The value to insert
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/push_back @endlink
		 */
		constexpr void push_back(const T &value) {
			auto ptr = __insert_space(_data + _size, 1);
			*ptr = value;
			_size++;
		}

		/**
		 * @brief Inserts the given value at the end of the vector
		 *
		 * @param value The value to insert
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/push_back @endlink
		 */
		constexpr void push_back(T &&value) {
			auto ptr = __insert_space(_data + _size, 1);
			*ptr = std::move(value);
			_size++;
		}

		/**
		 * @brief Constructs an element in-place before the given position
		 *
		 * @tparam Args The types of the arguments to construct the element with
		 * @param pos The position to insert the element before
		 * @param args The arguments to construct the element with
		 * @return Pointer to the inserted element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/emplace @endlink
		 */
		template <typename... Args>
		constexpr T *emplace(const T *pos, Args &&...args) {
			auto ptr = __insert_space(const_cast<T *>(pos), 1);
			std::construct_at(ptr, std::forward<Args>(args)...);
			_size++;
			return ptr;
		}

		/**
		 * @brief Constructs an element in-place at the end of the vector
		 *
		 * @tparam Args The types of the arguments to construct the element with
		 * @param args The arguments to construct the element with
		 * @return Reference to the inserted element
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/emplace_back @endlink
		 */
		template <typename... Args>
		constexpr T &emplace_back(Args &&...args) {
			auto ptr = __insert_space(_data + _size, 1);
			std::construct_at(ptr, std::forward<Args>(args)...);
			_size++;
			return *ptr;
		}

		/**
		 * @brief Resizes the vector to the given size
		 *
		 * @param count The new size of the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/resize @endlink
		 */
		constexpr void resize(size_t count) {
			if (count < _size) {
				for (size_t i = count; i < _size; i++) {
					std::destroy_at(&_data[i]);
				}
			} else if (count > _size) {
				auto ptr = __insert_space(_data + _size, count - _size, false);

				for (size_t i = 0; i < count - _size; i++) {
					std::construct_at(&ptr[i]);
				}
			}

			_size = count;
		}

		/**
		 * @brief Resizes the vector to the given size
		 *
		 * @param count The new size of the vector
		 * @param value The value to initialize new elements with
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/resize @endlink
		 */
		constexpr void resize(size_t count, const T &value) {
			if (count < _size) {
				for (size_t i = count; i < _size; i++) {
					std::destroy_at(&_data[i]);
				}
			} else if (count > _size) {
				auto ptr = __insert_space(_data + _size, count - _size, false);

				for (size_t i = 0; i < count - _size; i++) {
					ptr[i] = value;
				}
			}

			_size = count;
		}

		/**
		 * @brief Swaps the contents of the vector with the given vector
		 *
		 * @param other The vector to swap with
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector/swap @endlink
		 */
		constexpr void swap(vector &other) {
			std::swap(_data, other._data);
			std::swap(_size, other._size);
			std::swap(_capacity, other._capacity);
		}
#pragma endregion
	};

	// Deduction guides
	// https://en.cppreference.com/w/cpp/container/vector/deduction_guides

	template <typename Iter, typename Alloc = std::allocator<typename std::iterator_traits<Iter>::value_type>>
	vector(Iter, Iter, Alloc = Alloc()) -> vector<typename std::iterator_traits<Iter>::value_type, Alloc>;

	template <typename T, typename A>
	[[nodiscard]] constexpr inline bool operator==(const vector<T, A> &lhs, const vector<T, A> &rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		if (lhs.data() == rhs.data()) {
			return true;
		}

		for (size_t i = 0; i < lhs.size(); i++) {
			if (lhs[i] != rhs[i]) {
				return false;
			}
		}

		return true;
	}

	template <typename T, typename A>
	[[nodiscard]] constexpr inline auto operator<=>(const vector<T, A> &lhs, const vector<T, A> &rhs) {
		// TODO lexographical comparison operators
		return 0;
	}

	/**
	 * @brief Swap the contents of two vectors
	 *
	 * @tparam T The type of the elements in the vectors
	 * @tparam A The allocator type used to allocate memory for the vectors
	 * @param lhs The first vector
	 * @param rhs The second vector
	 *
	 * @link https://en.cppreference.com/w/cpp/container/vector/swap2 @endlink
	 */
	template <typename T, typename A>
	constexpr void swap(vector<T, A> &lhs, vector<T, A> &rhs) {
		lhs.swap(rhs);
	}

	// TODO erase
	// TODO erase_if

	namespace pmr {
		/**
		 * @brief Class template encapsulating a dynamic-size array
		 *
		 * @tparam T The type of the elements in the vector
		 *
		 * @link https://en.cppreference.com/w/cpp/container/vector @endlink
		 */
		template <typename T>
		using vector = std::vector<T, std::pmr::polymorphic_allocator<T>>;
	}
}