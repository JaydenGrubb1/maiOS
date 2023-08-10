/**
 * @author Jayden Grubb (contact@jaydengrubb.com)
 * @date 2023-07-29
 * @brief // DOC
 *
 * Copyright (c) 2023, Jayden Grubb
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <lib/libc++/bits/reverse_iterator.h>
#include <lib/libc++/optional.h>

#include <stddef.h>

namespace kstd {
	template <typename T, size_t N>
	class array {
	  public:
		T _data[N];

		constexpr size_t size(void) const {
			return N;
		}

		constexpr size_t max_size(void) const {
			return N;
		}

		constexpr bool empty(void) const {
			return N == 0;
		}

		constexpr T &operator[](size_t index) {
			return _data[index];
		}

		constexpr const T &operator[](size_t index) const {
			return _data[index];
		}

		constexpr optional<T> at(size_t index) {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		constexpr optional<const T> at(size_t index) const {
			if (index >= size()) {
				return {};
			}
			return _data[index];
		}

		constexpr T &front(void) {
			return _data[0];
		}

		constexpr const T &front(void) const {
			return _data[0];
		}

		constexpr T &back(void) {
			return _data[N - 1];
		}

		constexpr const T &back(void) const {
			return _data[N - 1];
		}

		constexpr T *data(void) {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		constexpr const T *data(void) const {
			if (size() == 0) {
				return nullptr;
			}

			return _data;
		}

		constexpr T *begin(void) {
			return _data;
		}

		constexpr const T *begin(void) const {
			return _data;
		}

		constexpr const T *cbegin(void) const {
			return _data;
		}

		constexpr T *end(void) {
			return _data + size();
		}

		constexpr const T *end(void) const {
			return _data + size();
		}

		constexpr const T *cend(void) const {
			return _data + size();
		}

		constexpr kstd::reverse_iterator<T *> rbegin(void) {
			return kstd::reverse_iterator(end());
		}

		constexpr const kstd::reverse_iterator<T *> rbegin(void) const {
			return kstd::reverse_iterator(end());
		}

		constexpr const kstd::reverse_iterator<T *> crbegin(void) const {
			return kstd::reverse_iterator(end());
		}

		constexpr kstd::reverse_iterator<T *> rend(void) {
			return kstd::reverse_iterator(begin());
		}

		constexpr const kstd::reverse_iterator<T *> rend(void) const {
			return kstd::reverse_iterator(begin());
		}

		constexpr const kstd::reverse_iterator<T *> crend(void) const {
			return kstd::reverse_iterator(begin());
		}

		void fill(const T &value) {
			// VERIFY Should this use std::move()
			for (size_t i = 0; i < size(); i++) {
				_data[i] = value;
			}
		}

		void swap(array &other) {
			// VERIFY Should this use std::move()
			for (size_t i = 0; i < size(); i++) {
				T temp = _data[i];
				_data[i] = other._data[i];
				other._data[i] = temp;
			}
		}
	};
}